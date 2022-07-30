#!/bin/bash

# desc: parse arguments explicitly
# requires: +echo

# This reuses the echo-args test but with required explicit string parsing.
# Ensure the right quoting format is supported.
# Also note extra whitespace

# this $'' performs escaping of special characters.
"${FS}" -c $'echo a   b\t123  \t \r  a123 [a b c] \"a\" \'b\'' > out.txt 2>err.txt
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
if [ "$( printf "a\\nb\\n123\\na123\\na b c\\n\"a\"\\n'b'\\n" )" != "$( cat out.txt )" ] ; then
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
