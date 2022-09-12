#!/bin/bash

# desc: expect that a running local server generates an expected http code.
# requires: +expect-http-get-response

if [ ! -x "${FS_SERVER}" ] ; then
    echo "?? SKIPPED because the test server was not compiled (${FS_SERVER})"
    exit 0
fi

printf "HTTP/1.1 200 OK\\r\\nContent-Type: text/plain\\r\\nContent-Length: 0\\r\\n\\r\\n" > server-response.txt
"${FS_SERVER}" http1 29446 server-response.txt >server-out.txt 2>server-err.txt &
server_pid=$!

# Give a bit of time for the server to start
sleep 1

"${FS}" -c "expect-http-get-response localhost 29446 / 200" > out.txt 2>err.txt
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


# should have: out.txt and err.txt and server-response.txt and server-out.txt and server-err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 5 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
