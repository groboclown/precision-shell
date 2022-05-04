#!/bin/bash

# desc: version with no arguments

"${FS}" version > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ -s err.txt ] ; then
    echo "Generated output to stderr"
    echo "stderr:"
    cat err.txt
    exit 1
fi

cat out.txt | perl -e 'while (<STDIN>) { if (/fs-shell\s+\d+\.\d+(\s+(\+\w+)+)?\s*$/) { exit 0; } else { exit 1; }} exit 2;'
res=$?
if [ ${res} -ne 0 ] ; then
    echo "Generated invalid version string:"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
