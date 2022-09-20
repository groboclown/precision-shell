#!/bin/bash

# desc: attempt an HTTP request against a port that has nothing running on it.
# requires: +expect-http-get-response

# If running with musl in a docker container, this can fail due to IPv6
#   reporting a connection without error when there is nothing
#   listening on the port.  This causes the write to the socket to
#   fail with a broken pipe (SIGPIPE).

"${FS}" -c "expect-http-get-response localhost 0 / 200" > out.txt 2>err.txt
res=$?

if [ ${res} -eq 0 ] ; then
    # Exit code here can be very high, for several reasons.
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
if [ "$( printf "ERROR expect-http-get-response: 200\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated invalid output to stderr"
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
