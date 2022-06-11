#!/bin/bash

# desc: pipe output using dup-w and dup-r into a program
# requires: +input +exec

echo "?? SKIPPED BECAUSE IT REQUIRES DEBUGGING"
exit 0

echo "dup-w 1 contents.txt" > script.txt
echo "echo foo bar baz" >> script.txt
echo "dup-w 1 sorted.txt" >> script.txt
echo "dup-r 0 contents.txt" >> script.txt
echo "exec $( which sort )" >> script.txt

"${FS}" -f script.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ ! -f contents.txt ] || [ ! -f sorted.txt ] ; then
    echo "Did not create contents.txt or sorted.txt"
    exit 1
fi

if [ "$( printf "foo\\nbar\\baz\\n" )" != "$( cat contents.txt )" ] ; then
    echo "Generated contents.txt incorrect"
    cat contents.txt
    exit 1
fi

if [ "$( printf "bar\\nbaz\\foo\\n" )" != "$( cat sorted.txt )" ] ; then
    echo "Generated sorted.txt incorrect"
    cat sorted.txt
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

# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
