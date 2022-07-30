#!/bin/bash

# desc: write to an open fd
# requires: +write-fd

"${FS}" -c "write-fd 2 [text1] [text 2\\ntext 3]" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

# Note no trailing newline
if [ "$( printf "text1text 2\\ntext 3" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
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
