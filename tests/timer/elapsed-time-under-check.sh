#!/bin/bash

# desc: ensure elapsed time works for both before and after conditions
# requires: +sleep +elapsed-time-under +start-timer +echo

"${FS}" -c "start-timer \
    && elapsed-time-under 2 \
    && echo A \
    && sleep 1 \
    && elapsed-time-under 2 \
    && echo B \
    && sleep 1 \
    && elapsed-time-under 2 \
    && echo C" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ printf "A\\nB\\n" != "$( cat out.txt )" ] ; then
    echo "Generated bad output to stdout:"
    cat out.txt
    exit 1
fi

if [ printf "ERROR elapsed-time-under: 2\\nFAIL &&\\n" != "$( cat err.txt )" ] ; then
    echo "Generated bad output to stderr:"
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
