#!/bin/bash

# desc: exporting environment variables is passed to child processes
# requires: +while-error +dup-w +chmod

touch a.txt
chmod 000 a.txt
"${FS}" -c "while-error [dup-w 1 a.txt] [chmod 666 a.txt ; echo [modified a]]" >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "modified a\\n" )" !=  "$( cat out.txt )" ] ; then
    echo "Generated unexpected output to stdout"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ "$( printf "ERROR dup-w: a.txt\\n" )" !=  "$( cat err.txt )" ] ; then
    echo "Generated unexpected output to stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
