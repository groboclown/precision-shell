#!/bin/bash

# desc: parse -c arguments which includes a newline.
# requires: +echo

# This reuses the echo-args test but with required explicit string parsing.

# With v3.0, the newline in the -c will be considered a command separator.
# Before v3.0, only the input mode would allow this.

if [ ! -x "${TEST_TMP_DIR}/arg_runner" ] ; then
    echo "!!! SKIPPED because arg_runner not compiled."
    exit 0
fi

echo -e "echo a b\\necho \"a b c\"" > to_run.txt
"${TEST_TMP_DIR}/arg_runner" \
    "${FS}" \
    to_run.txt > out.txt 2>err.txt
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
if [ "$( printf "a\\nb\\na b c\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt and to_run.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
