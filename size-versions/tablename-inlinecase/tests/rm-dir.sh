#!/bin/bash

# desc: rm against a directory
# requires: +rm

mkdir b
"${FS}" rm b >out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e : file exists (regardless of type or contents)
if [ ! -e b ] ; then
    echo "Incorrectly removed directory"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR rm: b\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated unexpected stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and b
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
