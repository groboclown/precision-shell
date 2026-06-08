#!/bin/bash

# desc: use dup-r to pipe into into a program.
# requires: +dup-r +exec

echo "456" > contents.txt
echo "123" >> contents.txt

"${FS}" -c "dup-r 0 contents.txt && exec $( which sort )" > out.txt 2> err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ ! -f contents.txt ] ; then
    echo "Incorrectly removed contents.txt"
    exit 1
fi

if [ "$( printf "123\\n456\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated contents.txt incorrect"
    cat contents.txt
    exit 1
fi

if [ -s err.txt ] ; then
    echo "Generated output to stdout or stderr"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
