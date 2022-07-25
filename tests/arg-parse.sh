#!/bin/bash

# desc: inline arguments are parsed as though they are separated by a single space.
# requires: +echo

"${FS}" echo \"a   \\n    b    c\" "&&" echo a   b   c > out.txt 2>err.txt
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

# Note that the \\n b has the space after the newline, which is expected.
if [ "$( printf "a \\n b c\\na\\nb\\nc\\n" )" != "$( cat out.txt )" ] ; then
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
