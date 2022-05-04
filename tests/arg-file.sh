#!/bin/bash

# desc: argument parsing with commands from stdin
# requires: +input

printf "echo a b 123 a123\\necho \"a b c\" def" > test-script.txt
"${FS}" -f test-script.txt > out.txt 2>err.txt
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

if [ "$( printf "echo a b 123 a123\\necho \"a b c\" def" )" != "$( cat test-script.txt )" ] ; then
    echo "input script was changed."
    cat test-script.txt
    exit 1
fi

if [ "$( printf "a\\nb\\n123\\na123\\na b c\\ndef\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt and test-script.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
