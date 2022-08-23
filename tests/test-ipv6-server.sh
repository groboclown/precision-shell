#!/bin/bash

# desc: test-ipv6 with no running server on port 0.
# requires: +test-ipv6

if [ ! -x "${FS_SERVER}" ] ; then
    echo "?? SKIPPED because the test server was not compiled (${FS_SERVER})"
    exit 0
fi

"${FS_SERVER}" 29446 >server-out.txt 2>server-err.txt &
server_pid=$!

"${FS}" -c "test-ipv6 ::0 29446 3" > out.txt 2>err.txt
res=$?

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

kill -15 "${server_pid}"


# should have: out.txt and err.txt and server-out.txt and server-err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 4 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
