#!/bin/bash

# desc: rmdir against some directories that exist and some that don't.

mkdir aa
mkdir dd
mkdir ee
"${FS}" rmdir aa bb cc dd >out.txt 2>err.txt
res=$?

if [ ${res} -ne 2 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -e : file exists (regardless of type or contents)
if [ -e aa ] || [ -e dd ] ; then
    echo "Incorrectly removed directory"
    exit 1
fi

# -e : file exists (regardless of type or contents)
if [ ! -d ee ] ; then
    echo "Incorrectly removed directory"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    exit 1
fi

if [ "$( printf "ERROR rmdir: bb\\nERROR rmdir: cc\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated unexpected stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and ee
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
