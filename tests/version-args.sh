#!/bin/bash

# desc: version with arguments

"${FS}" version abc > out.txt 2>err.txt
res=$?

# Should print version, print an error message, then exit with an error.

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

count=$( awk '/^fs-shell\s+\d+\.\d+(\s+\+\w+)*$/{print 1}' out.txt | wc -l )
res=$?
if [ ${res} -ne 0 ] || [ ${count} -ne 1 ] ; then
    echo "Generated invalid version string (${count} / ${res}):"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR version: abc\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
