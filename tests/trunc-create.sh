#!/bin/bash

# desc: create the given files
# requires: +input

"${FS}" trunc a.txt b.txt >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

for name in a.txt b.txt ; do
    if [ ! -f "${name}" ] ; then
        echo "Did not create file ${name}"
        exit 1
    fi
    if [ -s "${name}" ] ; then
        echo "File ${name} created with contents"
        cat "${name}"
        exit 1
    fi
    flags=$( stat -c '%A' "${name}" )
    if [ "${flags}" != "-rw-r--r--" ] ; then
        echo "File ${name} created with wrong permissions: ${flags}"
        exit 1
    fi
done

# -s : file exists and not empty
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
