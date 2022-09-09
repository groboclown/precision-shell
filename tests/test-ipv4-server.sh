#!/bin/bash

# desc: test-connect with a running server using ipv4.
# requires: +test-connect

if [ ! -x "${FS_SERVER}" ] ; then
    echo "?? SKIPPED because the test server was not compiled (${FS_SERVER})"
    exit 0
fi

"${FS_SERVER}" echo 29446 >server-out.txt 2>server-err.txt &
server_pid=$!

"${FS}" -c "test-connect [127.0.0.1] 29446 3" > out.txt 2>err.txt
res=$?
kill -15 "${server_pid}"

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
