#!/bin/bash

# desc: for-each with just the variable name and list, no runnable loop
# requires: +for-each +reqargs

"${FS}" -c "for-each abc def" >out.txt 2>err.txt
res=$?

# For an unknown reason, for-each loop with reqargs
#   does not trigger an error when the loop is skipped.
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

# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
