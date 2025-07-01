#!/bin/bash

# desc: cat a broken symbolic link.
# requires: +cat-fd +ln-s +rm

# This ensures that cat-fd outputs the contents of the file, not the file pointed by the link.

echo "contents" > a.txt
"${FS}" -c "ln-s a.txt b.txt && rm a.txt && cat-fd 1 b.txt" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "ERROR opening file b.txt\\nERROR cat-fd: b.txt" )" != "$( cat err.txt )" ] ; then
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

# should have: out.txt and err.txt and b.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
