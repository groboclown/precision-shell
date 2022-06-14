#!/bin/bash

# desc: run mkdev but with a bad node char.
# requires: +mkdev

"${FS}" mkdev 3 4 q a.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 2 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e : file exists
if [ -e a.txt ] ; then
    echo "File incorrectly created"
    exit 1
fi

if [ "$( printf "ERROR mkdev: q\\nERROR mkdev: a.txt\\n" )" != "$( cat err.txt )" ] ; then
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
