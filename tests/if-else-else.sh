#!/bin/bash

# desc: if-else command running the else block
# requires: +trunc +echo +if-else

echo -n "a.txt" > a.txt
chmod 000 a.txt
"${FS}" -c "if-else [trunc a.txt] [echo yes] [echo no]" >out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

chmod 666 a.txt
# -s : file exists and not empty
if [ "$( printf "a.txt" )" != "$( cat a.txt )" ] ; then
    echo "Incorrectly removed or changed source file"
    exit 1
fi

if [ "a.txt" != $( cat a.txt ) ] ; then
    echo "Altered contents incorrectly"
    exit 1
fi

if [ "$( printf "no\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated unexpected stdout"
    cat out.txt
    exit 1
fi

if [ "$( printf "ERROR trunc: a.txt" )" != "$( cat err.txt )" ] ; then
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
