#!/bin/bash

# desc: rm run with some files existing some not; last item exists.

echo "b.txt" > b.txt
"${FS}" rm a.txt b.txt >out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e : file exists (regardless of type or contents)
if [ -e b.txt ] ; then
    echo "Did not remove existing file"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    exit 1
fi

if [ "$( printf "ERROR rm: a.txt\\n" )" != "$( cat err.txt )" ] ; then
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
