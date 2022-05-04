#!/bin/bash

# desc: signal to ignore a system signal.
# requires: +signals

# This tests the intended behavior of a mildly interesting setup.

# make sure we use the kill program, not the Bash built-in kill.
kill_exec="$( which kill )"

# 15 == ctrl-c
# 10 == user signal

"${FS}" -c "signal 15 && signal 10 wait" >out.txt 2>err.txt &
childpid=$!

# ensure it's still running
"${kill_exec}" "-0" "${childpid}"
res=$?
if [ ${res} -ne 0 ] ; then
    echo "Did not stay alive one wait (0)."
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

"${kill_exec}" "-15" "${childpid}"
res=$?
if [ ${res} -ne 0 ] ; then
    echo "Did not stay alive on wait (1)."
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

# ensure it's still running
sleep 1
"${kill_exec}" "-0" "${childpid}"
res=$?
if [ ${res} -ne 0 ] ; then
    echo "Did not stay alive on wait (2)."
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

sleep 1
"${kill_exec}" "-10" "${childpid}"
res=$?
if [ ${res} -ne 0 ] ; then
    echo "Did not stay alive on wait (3)."
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

# Give the process a bit of time to die.
sleep 1
"${kill_exec}" "-0" "${childpid}"
res=$?
if [ ${res} == 0 ] ; then
    echo "Did not terminate on signal."
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    # clean up our mess
    "${kill_exec}" -9 "${childpid}"
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
