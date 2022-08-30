#!/bin/bash

# desc: attempt an HTTP request against a port that has nothing running on it.
# requires: +expect-http-get-response-not

"${FS}" -c "expect-http-get-response-not localhost 0 / 200" > out.txt 2>err.txt
res=$?

# Even if the server isn't running, this generates an okay response.
if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
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
