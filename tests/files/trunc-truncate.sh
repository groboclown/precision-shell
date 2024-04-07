#!/bin/bash

# desc: truncate a file to 0 length
# requires: +trunc

# Create the file, fill it with some data, and set its time to awhile ago.
echo "foo" > a.txt
touch -d 2000-12-13 a.txt || exit 1

# Get the date modified for the just-created file, in UTC since epoc.
# Note that even though the file is already at length 0, the way
#   truncate works, the last modified date will change.
before=$( date -r a.txt -u "+%s" )

"${FS}" trunc a.txt >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ ! -f a.txt ] ; then
    echo "Removed file a.txt"
    exit 1
fi

if [ -s a.txt ] ; then
    echo "Did not truncate file a.txt:"
    cat a.txt
    exit 1
fi

after=$( date -r a.txt -u "+%s" )

if [ "${before}" -ge "${after}" ] ; then
    echo "Current timestamp was not updated (was ${before}, now ${after})"
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
