#!/bin/bash

# desc: sleep with 1 argument

# UTC time, seconds since epoc
before=$( date -u "+%s" )
"${FS}" sleep 3 >out.txt 2>err.txt
res=$?
after=$( date -u "+%s" )

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# 1 second of slack
# -lt : less than
if [ $after -lt $(( before + 3 - 1 )) ] ; then
    echo "sleep from ${before} to ${after} exceeded 3 seconds"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] || [ -s err.txt ] ; then
    echo "Generated output to stdout or stderr"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
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
