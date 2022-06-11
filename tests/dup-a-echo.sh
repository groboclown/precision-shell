#!/bin/bash

# desc: dup-a echoing to a file
# requires: +input

echo "tuna" > a.txt
"${FS}" dup-a 1 a.txt "&&" echo is yummy  > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "tuna\\nis\\nyummy\\n" )" != "$( cat a.txt )" ] ; then
    echo "Generated unexpected a.txt"
    cat a.txt
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
