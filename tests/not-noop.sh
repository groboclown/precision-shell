#!/bin/bash

# desc: not run on a no-op instruction.
# requires: +noop +not

"${FS}" -c "not [noop]" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated stdout with contents"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR not: noop\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated output to stderr"
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
