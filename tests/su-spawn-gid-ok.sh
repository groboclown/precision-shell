#!/bin/bash

# desc: su-spawn with valid group id
# requires: +su-spawn +wait-pid

if [ "${UID0}" -ne 0 ] ; then
    echo "?? SKIPPED because not enough permissions to run it."
    exit 0
fi
if [ "${UID0}" == "${UID1}" ] || [ "${GID0}" == "${GID1}" ] ; then
    echo "Setup error: UID0 and UID1 or GID0 and GID1 are the same."
    exit 1
fi

id_exec=$( which id )
"${FS}" -c "su-spawn [${UID1}] [${GID1}] [${id_exec} -g] PID && wait-pid \${PID}" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 0 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "${GID1}\n" )" != "$( cat out.txt )" ] ; then
    echo "Generated invalid stdout"
    cat out.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s err.txt ] ; then
    echo "Generated output to stderr"
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
