#!/bin/bash

# desc: is-eq with two strings, off by whitespace
# requires: +is-eq

"${FS}" -c "is-eq [bad] [ bad ]" >out.txt 2>err.txt
res=$?

if [ ${res} -eq 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated unexpected stdout"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ "$( printf "ERROR is-eq:  bad \n" )" != "$( cat err.txt )" ] ; then
    echo "Generated unexpected stderr"
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
