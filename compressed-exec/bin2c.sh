#!/bin/bash
set -e

if [ ! -f "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ] ; then
    echo "Usage: $0 (binary file) (constant name) (output c file)"
    echo "Creates a c file that contains a single array storing the binary data file contents."
    exit 0
fi

echo "// ${1} contents" > "$3"
echo "static const uint8_t ${2}[] = {" >> "$3"

has_first=0
for k in $( od -t x1 "$1" ) ; do
    if [ ${#k} -gt 2 ] ; then
        echo "" >> "$3"
        echo "    // ${k}" >> "$3"
        echo -n "    " >> "$3"
    else
        if [ ${has_first} = 1 ] ; then
            echo -n ", " >> "$3"
        fi
        has_first=1
        echo -n "0x${k}" >> "$3"
    fi
done
echo "" >> "$3"
echo "}" >> "$3"
