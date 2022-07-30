#!/bin/bash

# desc: cat-fd with one argument
# requires: +cat-fd

echo "from a" > a.txt
"${FS}" -c "cat-fd 1 a.txt" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# Note extra spaces are intentional.
if [ "$( printf "from a\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
    echo "Generated output to stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
