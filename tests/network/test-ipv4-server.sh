#!/bin/bash

# desc: test-connect with a running server using ipv4.
# requires: +test-connect

if [ ! -x "${FS_SERVER}" ] ; then
    echo "?? SKIPPED because the test server was not compiled (${FS_SERVER})"
    exit 0
fi

if ! \
    "${UTIL_DIR}/launch_tcp_server.sh" \
    server-port.txt server-pid.txt \
    server-out.txt server-err.txt \
    echo \
; then
    echo "Server launch failed."
    exit 1
fi
port="$( cat server-port.txt )" || exit 1
server_pid="$( cat server-pid.txt )" || exit 1

"${FS}" -c "test-connect [127.0.0.1] ${port} 3" > out.txt 2>err.txt
res=$?
kill -15 "${server_pid}" || true
# Ensure these files were added.
touch server-out.txt server-err.txt server-port.txt server-pid.txt

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    echo "server-stdout:"
    cat server-out.txt
    echo "server-stderr:"
    cat server-err.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] || [ -s err.txt ] ; then
    echo "Generated output to stdout or stderr"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    echo "server-stdout:"
    cat server-out.txt
    echo "server-stderr:"
    cat server-err.txt
    exit 1
fi

# should have: out.txt and err.txt and server-out.txt and server-err.txt and server-port.txt and server-pid.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 6 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
