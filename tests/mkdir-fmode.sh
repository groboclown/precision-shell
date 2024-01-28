#!/bin/bash

# desc: mkdir with an explicit fmode
# requires: +mkdir

"${FS}" fmode 200 "&&" mkdir a12 > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

# -d : directory exists
if [ ! -d a12 ] ; then
    echo "Directory not created."
    exit 1
fi

# mkdir auto-adds execute permission to all the fields.
flags=$( stat -c '%A' a12 )
if [ "${flags}" != "d-wx--x--x" ] ; then
    echo "Incorrect permissions: ${flags}"
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

# should have: out.txt and err.txt and a12
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
