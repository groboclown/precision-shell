#!/bin/bash

# desc: Environment variable replacement with a non-existent variable..
# requires: +echo +enviro

export A_Test_Value=MyValue
unset A_Test_Value
export A_Test_Value
"${FS}" -c "echo [x \${A_Test_Value} y]" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
    echo "Generated output to stderr"
    cat err.txt
    exit 1
fi

# Note extra spaces are intentional.
if [ "$( printf "x \${A_Test_Value} y\\n" )" != "$( cat out.txt )" ] ; then
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
