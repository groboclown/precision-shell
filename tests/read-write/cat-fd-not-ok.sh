#!/bin/bash

# desc: cat with the fd argument not a valid fd
# requires: +cat-fd

echo "a.txt" > a.txt
"${FS}" -c "cat-fd asdf a.txt" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 2 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# Note extra spaces are intentional.
if [ "$( printf "ERROR cat-fd: asdf\\nERROR cat-fd: a.txt\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
    cat err.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
