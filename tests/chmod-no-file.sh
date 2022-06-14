#!/bin/bash

# desc: chmod run with incorrect argument count.
# requires: +chmod

echo "a.txt" > a.txt
chmod 000 a.txt
flags=$( stat -c '%A' a.txt )
if [ "${flags}" != "----------" ] ; then
    echo "Failed to setup initial file permissions."
    exit 1
fi

echo "b.txt" > b.txt
chmod 000 b.txt
flags=$( stat -c '%A' b.txt )
if [ "${flags}" != "----------" ] ; then
    echo "Failed to setup initial file permissions."
    exit 1
fi

# Note the edge cases here.
# The first one sets the mod value when operating
# on the file, and the second call has just one argument.
# This ensures that the program doesn't attempt to reuse
# the permissions from the first call.
"${FS}" chmod 666 a.txt \; chmod b.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ ! -s a.txt ] ; then
    echo "File changed"
    exit 1
fi

flags=$( stat -c '%A' a.txt )
if [ "${flags}" != "-rw-rw-rw-" ] ; then
    echo "Changed permissions to unexpected value: ${flags}"
    exit 1
fi

flags=$( stat -c '%A' b.txt )
if [ "${flags}" != "----------" ] ; then
    echo "Incorrectly changed permissions: ${flags}"
    exit 1
fi

if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR chmod: b.txt\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated unexpected stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 4 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
