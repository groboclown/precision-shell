#!/bin/bash

# desc: cat + dup use case
# requires: +cat-fd +dup-w +dup-a +echo

echo "a b c" > a.txt
"${FS}" -c "\
    echo [first] && \
    dup-w 1 b.txt && \
    cat-fd 1 a.txt && \
    dup-w 1 &1 && \
    echo second && \
    dup-a 1 b.txt && \
    cat-fd 1 a.txt" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "a b c\\n" )" != "$( cat a.txt )" ] ; then
    echo "Incorrectly modified a.txt:"
    cat a.txt
    exit 1
fi

if [ "$( printf "a b c\\na b c\\n" )" != "$( cat b.txt )" ] ; then
    echo "Generated b.txt not as expected:"
    cat out.txt
    exit 1
fi

if [ "$( printf "first\\nsecond\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
    echo "Generated output to stderr"
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
