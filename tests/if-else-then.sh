#!/bin/bash

# desc: if-else command running the then block
# requires: +trunc +echo +if-else

echo "a.txt" > a.txt
chmod +w a.txt
"${FS}" -c "if-else [trunc a.txt] [echo yes] [echo no]" >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
# -f : file exists
if [ ! -f a.txt] || [ -s a.txt ] ; then
    echo "Incorrectly removed or changed source file"
    exit 1
fi

if [ "$( printf "yes\\n" )" != "$( cat out.txt )" ] ; then
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

# should have: out.txt and err.txt and a.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
