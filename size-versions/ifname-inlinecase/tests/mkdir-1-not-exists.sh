#!/bin/bash

# desc: mkdir with 1 directory that does not exist.
# requires: +mkdir

"${FS}" mkdir abc > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -d : directory exists
if [ ! -d abc ] ; then
    echo "Directory not created."
    exit 1
fi

flags=$( stat -c '%A' abc )
if [ "${flags}" != "drwxr-xr-x" ] ; then
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

# should have: out.txt and err.txt and abc
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
