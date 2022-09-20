#!/bin/bash

# desc: expect that a running local server generates an http code but it doesn't match.
# requires: +expect-http-get-response

if [ ! -x "${FS_SERVER}" ] ; then
    echo "?? SKIPPED because the test server was not compiled (${FS_SERVER})"
    exit 0
fi

printf "HTTP/1.1 200 OK\\r\\nContent-Type: text/plain\\r\\nContent-Length: 0\\r\\n\\r\\n" > server-response.txt
"${FS_SERVER}" http1 29446 server-response.txt >server-out.txt 2>server-err.txt &
server_pid=$!

"${FS}" -c "expect-http-get-response localhost 29446 / 210" > out.txt 2>err.txt
res=$?
kill -15 "${server_pid}"
# Ensure these files were added.
touch server-out.txt server-err.txt

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ]; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ "$( printf "ERROR expect-http-get-response: 210" )" != "$( cat err.txt )" ] ; then
    echo "Generated invalid output to stderr"
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
