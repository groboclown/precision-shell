#!/bin/bash

# desc: signal capturing with proper exit code
# requires: +signal +enviro +kill-pid +exit +spawn +sleep

# This tests the intended behavior of a mildly interesting setup.

FS_ABS="$( cd "$( dirname "${FS}" )" ; pwd )/$( basename "${FS}" )" || exit 1

"${FS}" -c '
    spawn ['"${FS_ABS}"' -c [signal *SIG 13 wait ; exit $${SIG}]] PID &&
    sleep 1 &&
    kill-pid 13 ${PID} &&
    wait-pid ${PID} *EXIT &&
    exit ${EXIT}' >out.txt 2>err.txt
res=$?

if [ ${res} -ne 13 ] ; then
    echo "Unexpected exit code: ${res}"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] || [ -s err.txt ] ; then
    echo "Generated output to stdout or stderr"
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
