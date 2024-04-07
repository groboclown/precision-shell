#!/bin/bash

# desc: if-else with no arguments
# requires: +touch +echo +if-else

"${FS}" -c "if-else [touch a.txt] [echo yes] [echo no] [echo true]" >out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "yes\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated unexpected stdout"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR if-else: echo true" )" != "$( cat err.txt )" ] ; then
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
