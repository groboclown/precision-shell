#!/bin/bash

# desc: mv an existing file to a new name.

echo "a.txt" > a.txt

"${FS}" mv a.txt renamed-a.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e : file exists
if [ -e a.txt ] ; then
    echo "File still exists"
    exit 1
fi

if [ ! -f renamed-a.txt ] ; then
    echo "Did not create new file."
    exit 1
fi

if [ "a.txt" != $( cat renamed-a.txt ) ] ; then
    echo "Moved file does not contain original contents."
    exit 1
fi

if [ -s out.txt ] || [ -s err.txt ] ; then
    echo "Generated output to stdout or stderr"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
