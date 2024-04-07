#!/bin/bash

# desc: parse multiple commands using and and semicolons.
# requires: +echo +rm

"${FS}" -c "rm a.txt ; echo Continuing && echo Ending" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ "$( printf "ERROR rm: a.txt\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
    cat err.txt
    exit 1
fi

if [ "$( printf "Continuing\\nEnding\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
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
