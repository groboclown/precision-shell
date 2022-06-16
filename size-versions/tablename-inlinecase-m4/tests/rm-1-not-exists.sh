#!/bin/bash

# desc: rm run with one file that doesn't exist
# requires: +rm

echo "b.txt" > b.txt
"${FS}" rm a.txt >out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR rm: a.txt\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated unexpected stderr"
    cat err.txt
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
