#!/bin/bash

# desc: expect that a running local server generates an expected http code.
# requires: +expect-http-get-response

if [ ! -x "${FS_SERVER}" ] ; then
    echo "?? SKIPPED because the test server was not compiled (${FS_SERVER})"
    exit 0
fi

printf "HTTP/1.1 200 OK\\r\\nContent-Type: text/plain\\r\\nContent-Length: 0\\r\\n\\r\\n" > server-response.txt
if ! \
    "${UTIL_DIR}/launch_tcp_server.sh" \
    server-port.txt server-pid.txt \
    server-out.txt server-err.txt \
    http1 server-response.txt \
; then
    echo "Server launch failed."
    exit 1
fi
port="$( cat server-port.txt )" || exit 1
server_pid="$( cat server-pid.txt )" || exit 1

"${FS}" -c "expect-http-get-response localhost ${port} / 200" > out.txt 2>err.txt
res=$?

# The dietlibc seems to always exit with a 141 (SIGPIPE) here, when run from Docker.
# Try with the debug version, in order to better understand what's causing
# the failure.
if [ ${res} -eq 141 ] && [ -x "${FS}-debug" ] ; then
    "${FS}-debug" -c "expect-http-get-response localhost ${port} / 200" >> out.txt 2>>err.txt
    echo "Debug Exit: $?" >>err.txt
fi

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


# should have: out.txt and err.txt and server-response.txt and server-out.txt and server-err.txt and server-port.txt and server-pid.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 7 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
