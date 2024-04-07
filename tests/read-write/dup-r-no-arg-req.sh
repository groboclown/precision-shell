#!/bin/bash

# desc: dup-r with no arguments, and they are required
# requires: +dup-r +reqargs

"${FS}" dup-r > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "ERROR dup-r: requires another argument\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated unexpected stderr"
    cat err.txt
    exit 1
fi

if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi



# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
