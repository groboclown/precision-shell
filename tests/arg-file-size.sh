#!/bin/bash

# desc: argument parsing from a file exceeding buffer size.
# requires: +input +echo

# Intended to test with the default buffer size
if [ -z "${INPUT_BUFFER_SIZE}" ] ; then
    INPUT_BUFFER_SIZE=4096
fi

touch test-file.txt
touch expected-contents.txt
actualsize=$(wc -c <test-file.txt)
while [ ${actualsize} -le ${INPUT_BUFFER_SIZE} ] ; do
    echo "echo a" >> test-file.txt
    echo "a" >> expected-contents.txt
    actualsize=$(wc -c <test-file.txt)
done
# Add more to ensure it exceeds the buffer size.
echo "echo termination" >> test-file.txt
echo "termination" >> expected-contents.txt

"${FS}" -f test-file.txt > out.txt 2>err.txt
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

if [ "$( cat expected-contents.txt )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt and test-file.txt and expected-contents.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 4 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
