#!/bin/bash

# desc: Environment variable not format with large argument that fits exactly.
# requires: +echo +enviro +input

if [ -z "${PARSED_ARG_SIZE}" ] ; then
    PARSED_ARG_SIZE=1000
fi
echo -n "echo " > input-file.txt
expected=""
# Ensure the '$' is added as the last character.
#   -1 for the "$" appended at the end,
#   -1 for the trailing \0 at the end (in the code)
# Use a file to store the command arguments to avoid shell escape issues.
while [ "${#expected}" -lt $(( PARSED_ARG_SIZE - 2 )) ] ; do
    echo -n "a" >> input-file.txt
    expected="${expected}a"
done
echo "\$" >> input-file.txt
expected="${expected}\$"
# echo "Total length: ${#long_arg}"
"${FS}" -f input-file.txt > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "${expected}\\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated stdout not as expected:"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
    echo "Generated output to stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt and input-file.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 3 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
