#!/bin/bash

# desc: loop over an elapsed time to ensure the timer is global
# requires: +sleep +elapsed-time-under +start-timer +echo +while-no-error

"${FS}" -c "start-timer \
    && while-no-error [elapsed-time-under 2] [sleep 1 ; echo A] \
    && echo B" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ printf "A\\nA\\nB\\n" != "$( cat out.txt )" ] ; then
    echo "Generated bad output to stdout:"
    cat out.txt
    exit 1
fi

if [ printf "ERROR elapsed-time-under: 2\\n" != "$( cat err.txt )" ] ; then
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
