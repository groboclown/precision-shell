#!/bin/bash

# This test fails when running inside docker.
# desc: test-connect with no running server on localhost, port 0 and ipv6.
# requires: +test-connect +ipv6 +echo

# At the moment, a virtual network environment (such as with Docker) causes the
#    localhost (::1) address to act like it's listening to all ports - connecting
#    does not cause a failure.  A change is in the works to allow a connection to
#    send data to allow for triggering the SIGPIPE (13) signal.

"${FS}" -c '
    signal *SIG 13 &&
    test-connect ::1 0 3 &&
    exit ${SIG}
' > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "ERROR test-connect: 3\\nFAIL &&\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated invalid stderr"
    cat err.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
