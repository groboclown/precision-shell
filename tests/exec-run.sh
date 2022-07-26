#!/bin/bash

# desc: exec with a real executable.
# requires: +exec

touch_exec="$( which touch )"
"${FS}" exec "${touch_exec}" a.txt >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -f : is a file
if [ ! -f a.txt ] ; then
    echo "Did not touch a.txt"
    exit 1
fi

# -s : file exists and not empty
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
