#!/bin/bash

# desc: attempt an HTTP request against a port that has nothing running on it.
# requires: +expect-http-get-response +spawn +signal +enviro +echo

# If running with musl in a docker container, this can fail due to IPv6
#   reporting a connection without error when there is nothing
#   listening on the port.  This causes the write to the socket to
#   fail with a broken pipe (SIGPIPE, 13).

FS_ABS="$( cd "$( dirname "${FS}" )" ; pwd )/$( basename "${FS}" )" || exit 1

"${FS}" -c "\
    signal *SIG 13 && \
    expect-http-get-response localhost 0 / 200 ; \
    echo [exit:\${?};sig:\${SIG}]" \
 > out.txt 2>err.txt
res=$?

# There are two possible good solutions here.
# 1. Exit code != 0, which means expect-http-get-response failed.
#    The signal should not be set.
# 2. Exit code == 0 but signal == 13.

if [ ${res} -ne 0 ] ; then
    # Exit code here can be very high, for several reasons.
    echo "Bad exit code: ${res}"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

# -s : file exists and not empty
if \
    [ "$( printf "exit:0;sig:13" )" != "$( cat out.txt )" ] \
    && [ "$( printf "exit:1;sig:0" )" != "$( cat out.txt )" ] \
; then
    echo "Generated bad output to stdout"
    cat out.txt
    echo "stderr:"
    cat err.txt
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
