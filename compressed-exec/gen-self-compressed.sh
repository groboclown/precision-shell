#!/bin/bash
set -e

if [ ! -f "$1" ] || [ -z "$2" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ] ; then
    echo "Usage: $0 (source exec) (output dir)"
    echo "Generates the source file for the self-compressed executable."
    echo "The compressor (compress_lzg) and bin2c must be in the PATH."
    exit 0
fi

compressed=$2/compressed.lzg
here=$( cd "$(dirname "$0" )" && pwd )
cp "${here}/lzg.h" "$2/."
cp "${here}/liblzg_mini.c" "$2/."

compress_lzg -9 "$1" "${compressed}"

base_size=$( stat --printf="%s" "$1" )
compressed_size=$( stat --printf="%s" "${compressed}" )
bin2c "${compressed}" "compressedData" "$2/compressed-data.c-part"

header="$2/header.c-part"
echo "// Generated main for self-compressed executable" > "${header}"
echo "#include <stdlib.h>" >> "${header}"
echo "#include <stdio.h>" >> "${header}"
echo "#include \"lzg.h\"" >> "${header}"
echo "#include <unistd.h>" >> "${header}"
echo "#pragma GCC diagnostic push" >> "${header}"
echo "#pragma GCC diagnostic ignored \"-Wunused-result\"" >> "${header}"

main="$2/base-main.c-part"
echo "int main(const int argc, const char *argv[], const char **envp) {" > "${main}"
echo "    lzg_uint32_t res = 0;" >> "${main}"
echo "    EntryPoint entry = NULL;" >> "${main}"
echo "    uint8_t *decompressed = malloc(sizeof(uint8_t) * ${base_size});" >> "${main}"
echo "    if (decompressed == NULL) {" >> "${main}"
echo "        write(STDOUT_FILENO, \"ERROR out of memory\\n\", 20);" >> "${main}"
echo "        return 1;" >> "${main}"
echo "    }" >> "${main}"
echo "    res = LZG_Decode(compressedData, sizeof(uint8_t) * ${compressed_size}, decompressed, sizeof(uint8_t) * ${base_size});" >> "${main}"
echo "    if (res == 0) {" >> "${main}"
echo "        write(STDOUT_FILENO, \"ERROR extraction failed\\n\", 24);" >> "${main}"
echo "        return 1;" >> "${main}"
echo "    }" >> "${main}"
echo "    entry = copyMemory(decompressed);" >> "${main}"
echo "    if (entry == NULL) {" >> "${main}"
echo "        return 1;" >> "${main}"
echo "    }" >> "${main}"
echo "    free(decompressed);" >> "${main}"
echo "    decompressed = NULL;" >> "${main}"
echo "    return (*entry)(argc, argv, envp);" >> "${main}"
echo "}" >> "${main}"

bash -x "${here}/gen-mem-copy.sh" "$1" "$2/mem-copy.c-part"
cat "$2/header.c-part" "$2/mem-copy.c-part" "$2/compressed-data.c-part" "$2/base-main.c-part" > "$2/main.c"
