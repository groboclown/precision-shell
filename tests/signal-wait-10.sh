#!/bin/bash

# desc: signal wait with signal numbers.
# requires: +signal

# make sure we use the kill program, not the Bash built-in kill.
kill_exec="$( which kill )"

"${FS}" signal 10 wait >out.txt 2>err.txt &
childpid=$!

# ensure it's still running
"${kill_exec}" -0 ${childpid} >/dev/null 2>&1
res=$?
if [ ${res} -ne 0 ] ; then
    echo "Did not stay alive waiting for signal."
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

"${kill_exec}" -10 ${childpid} >/dev/null 2>&1
res=$?
if [ ${res} -ne 0 ] ; then
    echo "Did not stay alive waiting for signal."
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    exit 1
fi

# Give the process a bit of time to die.
sleep 1
"${kill_exec}" -0 ${childpid} >/dev/null 2>&1
res=$?
if [ ${res} == 0 ] ; then
    echo "Did not terminate on signal."
    echo "stdout:"
    cat out.txt
    echo "stderr:"
    cat err.txt
    # clean up our mess
    "${kill_exec}" -9 ${childpid}
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
