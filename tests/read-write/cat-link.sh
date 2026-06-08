#!/bin/bash

# desc: cat a symbolic link's contents.
# requires: +cat-fd +ln-s

# This ensures that cat-fd outputs the contents of the file, not the file pointed by the link.

echo "contents" > a.txt
"${FS}" -c "ln-s a.txt b.txt && cat-fd 1 b.txt" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "contents\\n" )" != "$( cat out.txt )" ] ; then
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

# should have: out.txt and err.txt and a.txt and b.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 4 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
