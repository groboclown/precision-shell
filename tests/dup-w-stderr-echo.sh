#!/bin/bash

# desc: dup-w to redirect stdout to stderr for the echo command
# requires: +dup-w

"${FS}" dup-w 1 "&2" "&&" echo hello everyone >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "hello\\neveryone\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated unexpected err.txt"
    cat a.txt
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
