#!/bin/bash

# desc: elapsed time over sleep
# requires: +sleep +elapsed-time +start-timer

"${FS}" -c "start-timer ; sleep 1 ; elapsed-time ; sleep 1 ; elapsed-time" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ printf "1\\n2\\n" != "$( cat out.txt )" ] ; then
    echo "Generated bad output to stdout:"
    cat out.txt
    exit 1
fi

if [ -s err.txt ] ; then
    echo "Generated output to stderr"
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
