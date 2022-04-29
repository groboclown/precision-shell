#!/bin/bash

# desc: ln-h for an existing file to a non-existing file.

echo "a.txt" > a.txt
"${FS}" ln-h a.txt linked-a.txt >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ ! -s a.txt ] ; then
    echo "Incorrectly removed or changed source file"
    exit 1
fi

# -ef : file1 is a hard link to file2
if [ ! linked-a.txt -ef a.txt ] ; then
    echo "Did not create destination as a hard link to expected file."
    ls -lA
    exit 1
fi

if [ "a.txt" != $( cat linked-a.txt ) ] ; then
    echo "Link does not contain original contents."
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] || [ -s err.txt ] ; then
    echo "Generated output to stdout or stderr"
    exit 1
fi

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
