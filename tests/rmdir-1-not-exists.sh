#!/bin/bash

# desc: rmdir against a directory that does not exist

mkdir a
mkdir b
"${FS}" rmdir c >out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e : file exists (regardless of type or contents)
if [ ! -e a ] || [ ! -e b ] ; then
    echo "Incorrectly removed directory"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR rmdir: c\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated unexpected stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and a and b
count="$( ls -1A | wc -l )"
if [ ${count} != 4 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
