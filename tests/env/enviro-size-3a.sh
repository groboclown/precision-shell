#!/bin/bash

# desc: Environment variable not format with large argument 1 over limit.
# requires: +echo +enviro +input

if [ -z "${PARSED_ARG_SIZE}" ] ; then
    PARSED_ARG_SIZE=1000
fi
echo -n "echo " > input-file.txt
expected=""
# Ensure the '$' is added as the last character.
#   -1 for the trailing \0 at the end (in the code)
# Use a file to store the command arguments to avoid shell escape issues.
while [ "${#expected}" -lt $(( PARSED_ARG_SIZE - 1 )) ] ; do
    echo -n "a" >> input-file.txt
    expected="${expected}a"
done
echo "\$" >> input-file.txt
expected="${expected}\$"
# echo "Total length: ${#long_arg}"
"${FS}" -f input-file.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "ERROR Argument length too long\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

# should have: out.txt and err.txt and input-file.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
