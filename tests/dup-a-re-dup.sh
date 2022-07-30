#!/bin/bash

# desc: dup-w into a file twice
# requires: +dup-a +echo

# This makes sure that the dup commands correctly close the file stream.

"${FS}" \
    dup-a 1 a.txt \
    "&&" echo tuna \
    "&&" dup-a 1 b.txt \
    "&&" echo [is yummy] \
    "&&" dup-a 1 a.txt \
    "&&" echo [is salty] > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "tuna\\nis salty\\n" )" != "$( cat a.txt )" ] ; then
    echo "Generated unexpected a.txt"
    cat a.txt
    exit 1
fi

if [ "$( printf "is yummy\\n" )" != "$( cat b.txt )" ] ; then
    echo "Generated unexpected b.txt"
    cat a.txt
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

# should have: out.txt and err.txt and a.txt and b.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 4 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
