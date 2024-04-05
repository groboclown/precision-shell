#!/bin/bash

# desc: argument parsing with a partial escape sequence
# requires: +echo

"${FS}" -c "echo ab\\" > out.txt 2>err.txt
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

# the in-progress handling of the escape sequence is terminated early and the
#   backslash is not included in the parsed arguments.
if [ "$( printf "ab\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
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
