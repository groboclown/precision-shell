#!/bin/bash

# desc: run mknod but with a bad node char.
# requires: +mkdev

if [ "${UID1}" = 0 ] || [ -z "${UID1}" ] ; then
    echo "?? SKIPPED because UID1 is zero or not set"
    exit 0
fi

user1="$( getent passwd "${UID1}" | cut -f 1 -d ':' )"

if [ -z "${user1}" ] ; then
    echo "?? SKIPPED because UID1 (${UID1}) is missing"
    exit 0
fi

su -s "${FS}" "${user1}" mkdev 1 3 c dev-null >out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e : file exists
if [ -e a.txt ] ; then
    echo "File incorrectly created"
    exit 1
fi

if [ "$( printf "ERROR: mkdev dev-null\\n" )" != $( cat err.txt ) ] ; then
    echo "Incorrect stderr output"
    cat err.txt
    exit 1
fi

if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
