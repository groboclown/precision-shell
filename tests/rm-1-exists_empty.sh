#!/bin/bash

# desc: rm run with one file that exists and has no contents.

touch a.txt
echo "b.txt" > b.txt
"${FS}" rm a.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e: exists (doesn't matter it's type or contents)
if [ -e a.txt ] ; then
    echo "Did not remove file."
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] || [ -s err.txt ] ; then
    echo "Generated output to stdout or stderr"
    exit 1
fi
if [ ! -s b.txt ] ; then
    echo "Command removed incorrect file"
    exit 1
fi

# should have: out.txt and err.txt and b.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
