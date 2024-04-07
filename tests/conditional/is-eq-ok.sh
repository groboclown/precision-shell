#!/bin/bash

# desc: is-eq with two identical strings.
# requires: +is-eq

"${FS}" -c "is-eq [a b] [a b]" >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated unexpected stdout"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
    echo "Generated unexpected stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
