#!/bin/bash

# desc: ls with a single argument
# requires: +ls

mkdir other-dir
touch other-dir/file-a.txt
touch other-dir/file-b.txt
mkdir other-dir/dir-a
touch other-dir/dir-a/file-a-a.txt
"${FS}" ls other-dir >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# sort output to ensure ordering doesn't mess up the
# test.
# -s : file exists and not empty
if [ "$( printf "dir-a\\nfile-a.txt\\nfile-b.txt\\n" )" != "$( cat out.txt | sort )" ] ; then
    echo "Generated invalid output to stdout"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
    echo "Generated output to stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and other-dir
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
