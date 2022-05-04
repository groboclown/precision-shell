#!/bin/bash

# desc: chmod run against a file

echo "a.txt" > a.txt
chmod 000 a.txt
flags=$( stat -c '%A' a.txt )
if [ "${flags}" != "----------" ] ; then
    echo "Failed to setup initial file permissions."
    exit 1
fi

"${FS}" chmod 777 a.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ ! -s a.txt ] ; then
    echo "File changed"
    exit 1
fi

flags=$( stat -c '%A' a.txt )
if [ "${flags}" != "-rwxrwxrwx" ] ; then
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

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
