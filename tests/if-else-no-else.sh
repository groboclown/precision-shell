#!/bin/bash

# desc: if-else with no arguments
# requires: +echo +if-else

"${FS}" -c "if-else [echo a] [echo b] ; echo c" >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "a\\nb\\nc\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated unexpected stdout"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
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
