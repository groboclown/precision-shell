#!/bin/bash

# desc: change the time of an existing, empty file
# requires: +input

# Use real touch to create the file, and set its time to earlier.
touch a.txt
touch -d 2000-12-13 a.txt || exit 1

# Get the date modified for the just-created file, in UTC since epoc
before=$( date -r a.txt -u "+%s" )

"${FS}" trunc a.txt >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

after=$( date -r a.txt -u "+%s" )

if [ "${before}" -ge "${after}" ] ; then
    echo "Current timestamp was not updated (was ${before}, now ${after})"
    exit 1
fi

# the file should exist and have 0 length.
if [ ! -f a.txt ] ; then
    echo "Removed a.txt"
    exit 1
fi

# -s : file exists and not empty
if [ -s a.txt ] ; then
    echo "Altered the contents of a.txt:"
    cat a.txt
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

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
