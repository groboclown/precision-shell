#!/bin/bash

# desc: rmdir against a directory that exists

mkdir a
mkdir b
"${FS}" rmdir a >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e : file exists (regardless of type or contents)
if [ -e a ] ; then
    echo "Did not remove directory"
    exit 1
fi

# -e : file exists (regardless of type or contents)
if [ ! -e b ] ; then
    echo "Incorrectly removed directory"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] || [ -s err.txt ] ; then
    echo "Generated output to stdout or stderr"
    exit 1
fi

# should have: out.txt and err.txt and b
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
