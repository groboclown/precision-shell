#!/bin/bash

# desc: version with arguments

"${FS}" version abc > out.txt 2>err.txt
res=$?

# Should print version, print an error message, then exit with an error.

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

cat out.txt | perl -e 'while (<STDIN>) { if (/fs-shell\s+\d+\.\d+(\s+(\+\w+)+)?\s*$/) { exit 0; } else { exit 1; }} exit 2;'
res=$?
if [ ${res} -ne 0 ] ; then
    echo "Generated invalid version string:"
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
