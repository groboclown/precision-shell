#!/bin/bash

# This test fails when running inside docker.
# desc: test-connect with no running server on port 0 and ipv6.
# requires: +test-connect +ipv6

"${FS}" -c "test-connect ::0 0 3" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "ERROR test-connect: 3\n" )" != "$( cat err.txt )" ] ; then
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
