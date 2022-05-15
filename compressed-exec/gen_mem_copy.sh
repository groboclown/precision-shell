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
echo "#include <sys/mman.h>" >> "$2"
echo "#define START_ADDRESS 0x${start_address}" >> "$2"
echo "typedef const int (*EntryPoint) (const int, const char **, const char **)" >> "$2"
echo "" >> "$2"
echo "EntryPoint copyMemory(void *rawData) {" >> "$2"
echo "   Entrypoint ret = NULL;"
echo "   void *data = NULL;"

objdump --wide --private-headers "$1" > "${tmpfile}"
state=0
index=0
while read -r line; do
    if [ ${state} = 0 ] ; then
        # Search for the header
        state=$( cat "${line}" | egrep "^Program Header:" > /dev/null 2>&1 && echo 1 || echo 0 )
    elif [ ${state} = 1 ] ; then
        # Start of a block line
        # Format:
        #     LOAD off    0x0000000000000000 vaddr 0x0000000000400000 paddr 0x0000000000400000 align 2**21
        offset=$( echo "${line}" | awk 'match($0, /\s+off\s+(0x\w+)\s/, a){print a[1]}' )
        vaddr=$( echo "${line}" | awk 'match($0, /\s+vaddr\s+(0x\w+)\s/, a){print a[1]}' )
        paddr=$( echo "${line}" | awk 'match($0, /\s+paddr\s+(0x\w+)\s/, a){print a[1]}' )
        align_bits=$( echo "${line}" | awk 'match($0, /\s+align\s+2\*\*(\d+)/, a){print a[1]}' )
        state=2
    elif [ ${state} = 2 ] ; then
        # Format:
        #          filesz 0x0000000000002ee0 memsz 0x0000000000002ee0 flags rwx
        file_size=$( echo "${line}" | awk 'match($0, /\s+filesz\s+(0x\w+)\s/, a){print a[1]}' )
        mem_size=$( echo "${line}" | awk 'match($0, /\s+memsz\s+(0x\w+)\s/, a){print a[1]}' )
        flags=$( echo "${line}" | awk 'match($0, /\s+flags\s+([a-z-]+)/, a){print a[1]}' )

        # TODO:
        #   - use mmap to allocate the memory with the "vaddr" starting position and writable protection and size == mem size
        #       - mem size should already be aligned, but do it anyway, just to be sure.
        #   - use memcpy to copy the memory from rawData at postion $offset, of $file_size bytes.
        #   - use mprotect to change the allocated memory to be the right permissions based on $flags.
        #   - somehow, set 'ret' to START_ADDRESS and return it.

#    data = mmap(NULL, fileSize + 1, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0); 

        state=1
        index=$(( index + 1 ))
    fi
    echo -e "$line\n"
done <"${tmpfile}"

echo "}" >> "$2"

test -f "${tmpfile}" && rm "${tmpfile}"
