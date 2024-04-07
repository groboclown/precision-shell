#!/bin/bash

# desc: Environment capture of the last exit code.
# requires: +echo +is-eq +enviro

"${FS}" -c 'is-eq 1 2 ; echo [a:${?}] ; is-eq 1 1 ; echo [b:${?}] ;' > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "ERROR is-eq: 2\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
    cat err.txt
    exit 1
fi

if [ "$( printf "a:1\nb:0\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
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
