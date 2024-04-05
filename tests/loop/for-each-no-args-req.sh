#!/bin/bash

# desc: for-each with no arguments
# requires: +for-each +reqargs

"${FS}" -c "for-each" >out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ "$( printf "ERROR for-each: requires another argument\\n" )" !=  "$( cat err.txt )" ] ; then
    echo "Generated unexpected output to stderr"
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
