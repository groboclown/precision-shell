#!/bin/bash

# desc: dup-w into stdout and restore it.
# requires: +dup-w +echo

"${FS}" dup-w 1 a.txt "&&" echo tuna "&&" dup-w 1 "&1" "&&" echo [is yummy] > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "tuna\\n" )" != "$( cat a.txt )" ] ; then
    echo "Generated unexpected a.txt"
    cat a.txt
    exit 1
fi

if [ "$( printf "is yummy\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated unexpected out.txt"
    cat out.txt
    exit 1
fi

if [ -s err.txt ] ; then
    echo "Generated output to stderr"
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
