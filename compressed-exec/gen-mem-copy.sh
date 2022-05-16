#!/bin/bash
set -e

if [ ! -f "$1" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ] || [ -z "$2" ]; then
    echo "Usage: $0 (program to inspect) (output file name)"
    exit 1
fi

tmpfile=/tmp/$$.txt

start_address=$( objdump --wide --file-headers "$1" | egrep "^start address 0x" | cut -f 3 -d ' ' )

echo "// Generated Mem Copy From $1" > "$2"
echo "// $( date )" >> "$2"
echo "#include <stdlib.h>" >> "$2"
echo "#include <stdio.h>" >> "$2"
echo "#include <stdint.h>" >> "$2"
echo "#include <string.h>" >> "$2"
echo "#include <sys/mman.h>" >> "$2"
echo "#include <unistd.h>" >> "$2"
echo "typedef const int (*EntryPoint) (const int, const char **, const char **);" >> "$2"
echo "#define ENTRY_FUNCTION (EntryPoint)(${start_address})" >> "$2"
echo "" >> "$2"
echo "EntryPoint copyMemory(uint8_t *rawData) {" >> "$2"
echo "   void *data = NULL;" >> "$2"

objdump --wide --private-headers "$1" > "${tmpfile}"
state=0
index=0
line1=""
while read -r line; do
    if [ ${state} = 0 ] ; then
        # Search for the header
        state=$( echo "${line}" | egrep "^Program Header:" > /dev/null 2>&1 && echo 1 || echo 0 )
    elif [ ${state} = 1 ] ; then
        # Start of a block line
        # Format:
        #     LOAD off    0x0000000000000000 vaddr 0x0000000000400000 paddr 0x0000000000400000 align 2**21
        line1="${line}"
        offset=$( echo "${line}" | awk 'match($0, /\s+off\s+(0x\w+)\s/, a){print a[1]}' )
        vaddr=$( echo "${line}" | awk 'match($0, /\s+vaddr\s+(0x\w+)\s/, a){print a[1]}' )
        paddr=$( echo "${line}" | awk 'match($0, /\s+paddr\s+(0x\w+)\s/, a){print a[1]}' )
        align_bits=$( echo "${line}" | awk 'match($0, /\s+align\s+2\*\*(\d+)/, a){print a[1]}' )
        state=2
    elif [ ${state} = 2 ] ; then
        # Format:
        #          filesz 0x0000000000002ee0 memsz 0x0000000000002ee0 flags rwx
        file_size=$( echo "${line}" | awk 'match($0, /\s*filesz\s+(0x\w+)\s/, a){print a[1]}' )
        mem_size=$( echo "${line}" | awk 'match($0, /\s*memsz\s+(0x\w+)\s/, a){print a[1]}' )
        # Converts the hex representation to decimal.
        dec_file_size=$(( file_size ))
        dec_mem_size=$(( mem_size ))
        if [ ${dec_mem_size} -gt 0 ] ; then
            flags=$( echo "${line}" | awk 'match($0, /\s+flags\s+([a-z-]+)/, a){print a[1]}' )
            cflags="0"
            if [[ "${flags}" =~ r ]] ; then
                cflags="${cflags}|PROT_READ"
            fi
            if [[ "${flags}" =~ w ]] ; then
                cflags="${cflags}|PROT_WRITE"
            fi
            if [[ "${flags}" =~ x ]] ; then
                cflags="${cflags}|PROT_EXEC"
            fi

            # TODO:
            #   - mem size should already be aligned, but do it anyway, just to be sure.

            echo "" >> "$2"
            echo "    // ${line1}" >> "$2"
            echo "    // ${line}" >> "$2"
            if [ ${dec_file_size} -gt 0 ] ; then
                echo "    data = mmap((void *) ${vaddr}, ${mem_size}, PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);" >> "$2"
                echo "    if (data == NULL) {" >> "$2"
                echo "        write(STDOUT_FILENO, \"ERROR mmap failed\\n\", 18);" >> "$2"
                echo "        return NULL;" >> "$2"
                echo "    }" >> "$2"
                echo "    memcpy(data, &(rawData[${offset}]), ${file_size});" >> "$2"
                echo "    if (mprotect(data, ${mem_size}, ${cflags}) != 0) {" >> "$2"
                echo "        write(STDOUT_FILENO, \"ERROR mprotect failed\\n\", 22);" >> "$2"
                echo "        return NULL;" >> "$2"
                echo "    }" >> "$2"
            else
                # Nothing to copy
                echo "    data = mmap((void *) ${vaddr}, ${mem_size}, ${cflags}, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);" >> "$2"
            fi

        fi

#    data = mmap(NULL, fileSize + 1, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0); 

        state=1
        index=$(( index + 1 ))
    fi
    echo -e "$line\n"
done <"${tmpfile}"

echo "    return ENTRY_FUNCTION;" >> "$2"
echo "}" >> "$2"

test -f "${tmpfile}" && rm "${tmpfile}"
