#!/bin/bash

# desc: chown against a file with existing user id + group id with permissions to change them.
# requires: +chown

if [ "${UID0}" -ne 0 ] ; then
    echo "?? SKIPPED because not enough permissions to run it."
    exit 0
fi
if [ "${UID0}" == "${UID1}" ] || [ "${GID0}" == "${GID1}" ] ; then
    echo "Setup error: UID0 and UID1 or GID0 and GID1 are the same."
    exit 1
fi

echo "a.txt" > a.txt
chown "${UID0}:${GID0}" a.txt || exit 1
owned=$( stat -c '%u:%g' a.txt )
if [ "${owned}" != "${UID0}:${GID0}" ] ; then
    echo "Setup error: failed to setup initial file ownership (${owned})."
    exit 1
fi

"${FS}" chown "${UID1}" "${GID1}" a.txt > out.txt 2>err.txt
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

owned=$( stat -c '%u:%g' a.txt )
if [ "${owned}" != "${UID1}:${GID1}" ] ; then
    echo "Incorrect permissions: ${owned}"
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
