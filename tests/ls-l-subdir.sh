#!/bin/bash

# desc: ls-l with a single argument
# requires: +ls-l

mkdir other-dir
touch other-dir/file-a.txt
touch other-dir/file-b.txt
mkdir other-dir/dir-a
touch other-dir/dir-a/file-a-a.txt
"${FS}" ls-l other-dir >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# sort output to ensure ordering doesn't mess up the test.
# Note that sorting has implementation differences; some
# implementations put the "d-" before "--".
line1="--rw-r--r-- 1 ${UID0} ${GID0} 0 0 0 other-dir/file-a.txt"
line2="--rw-r--r-- 1 ${UID0} ${GID0} 0 0 0 other-dir/file-b.txt"
line3="d-rwxr-xr-x 2 ${UID0} ${GID0} 0 0 4096 other-dir/dir-a"
actual="$( cat out.txt | sort )"
if \
        [ "$( printf -- "${line1}\\n${line2}\\n${line3}\\n" )" != "${actual}" ] \
     && [ "$( printf -- "${line3}\\n${line1}\\n${line2}\\n" )" != "${actual}" ] \
; then
    echo "Generated invalid output to stdout"
    echo "${actual}"
    echo "Expected:"
    printf -- "${line1}\\n${line2}\\n${line3}"
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
