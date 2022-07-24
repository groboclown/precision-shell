#!/bin/bash

# desc: arguments as-is must not be escaped.
# requires: +echo

# This reuses the echo-args test but with required explicit string parsing.

"${FS}" echo "a \\n b c" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
    echo "Generated output to stderr"
    cat err.txt
    exit 1
fi
# bash will turn \\ into \, but then printf will do additional unescaping.
if [ "$( printf "a \\\\n b c" )" != "$( cat out.txt )" ] ; then
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
