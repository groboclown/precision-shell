#!/bin/bash

# desc: an unknown command with arguments.

"${FS}" not-a-command a.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 2 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR not-a-command: not-a-command\\nERROR not-a-command: a.txt\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
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
