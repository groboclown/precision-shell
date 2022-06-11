#!/bin/bash

# desc: run mknod to create a socket
# requires: +device

"${FS}" mknod s a.sock > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -S : socket type
if [ ! -S a.sock ] ; then
    echo "Did not create socket file"
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

# should have: out.txt and err.txt and a.sock
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
