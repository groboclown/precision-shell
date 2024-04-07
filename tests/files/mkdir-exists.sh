#!/bin/bash

# desc: mkdir with 1 directory that already exists.
# requires: +mkdir

mkdir a12
"${FS}" mkdir a12 > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -d : directory exists
if [ ! -d a12 ] ; then
    echo "Directory no longer exists."
    exit 1
fi

flags=$( stat -c '%A' a12 )
if [ "${flags}" != "drwxr-xr-x" ] ; then
    echo "Incorrect permissions: ${flags}"
    exit 1
fi


if [ "$( printf "ERROR mkdir: a12" )" != "$( cat err.txt )" ] ; then
    echo "Incorrect stderr contents"
    cat err.txt
    exit 1
fi


if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt and a12
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
