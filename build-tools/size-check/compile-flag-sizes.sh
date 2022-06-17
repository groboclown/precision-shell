#!/bin/bash

# Test compilation across different compile flags to get the smallest size possible.

if [ ! -z "$1" ] ; then
    MAX_JOB_COUNT=$1
elif [ -z "${MAX_JOB_COUNT}" ] ; then
    proc_count=$( getconf _NPROCESSORS_ONLN )
    MAX_JOB_COUNT=$(( proc_count * 4 / 3 ))
fi

cd "$( dirname "$0" )"

# Stuff that should always be on.
# By not including these, we dramatically reduce the combination space.
base_flags="-Os -s -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables"

# These always make things bigger
#  -O2

# All possible flags to try
all_flags=(
    -g0 \
    -Oz \
    -fno-strict-aliasing \
    -fno-align-functions \
    -fno-align-jumps \
    -fno-align-loops \
    -fno-align-labels \
	-finline-limit=0 \
	-fno-builtin-strlen \
	-ffunction-sections \
	-fdata-sections \
	-fno-guess-branch-probability \
	-funsigned-char \
	-falign-functions=1 \
	-falign-jumps=1 \
	-falign-labels=1 \
	-falign-loops=1 \
	-fno-builtin-printf \
	-fvisibility=hidden \
    -fno-string-plus-int \
    -fno-constant-logical-operand \
    -fsanitize=address \
    -fsanitize=leak \
    -fsanitize=undefined \
)

# First pass is to find which flags cause a failure.  Don't include these
#   in our list to try
valid_flags=()
tmpout=/tmp/fs-shell-$$
for val in "${all_flags[@]}" ; do
    echo "Trying ${val}"
    ( cd "../../src" && make src MINFLAGS="${val}" OUTDIR="${tmpout}" )
    if [ $? = 0 ]; then
        valid_flags+=("${val}")
    fi
done
rm -rf "${tmpout}"


flag_count="${#valid_flags[@]}"

# Basically, we want each flag to be a bit, either on or off.
active_flags=()
for val in "${valid_flags[@]}" ; do
    active_flags+=(0)
done

# Now, keep going until the final bit is set.
test -f bad-flags.txt && rm bad-flags.txt
test -f size-check.txt && rm size-check.txt

job_count=0
while [ 1 = 1 ] ; do
    MINFLAGS="${base_flags}"
    i=0
    while [ $i -lt "${flag_count}" ] ; do
        if [ "${active_flags[$i]}" = 1 ] ; then
            MINFLAGS="${MINFLAGS} ${valid_flags[$i]}"
        fi
        i=$(( i + 1 ))
    done

    # Kick off the build in the background with a maximum number
    #   of background processes.
    if [ ${job_count} -ge ${MAX_JOB_COUNT} ] ; then
        # Wait for the next child job to finish.
        wait -n
        job_count=${MAX_JOB_COUNT}
    fi
    job_count=$(( job_count + 1 ))

    ./gather-compile-size.sh size-check.txt bad-flags.txt ${MINFLAGS} &

    # change flags
    remainder=1
    i=0
    while [ $i -lt "${flag_count}" ] ; do
        val=$(( active_flags[$i] + remainder ))
        if [ ${val} = 2 ]; then
            val=0
            remainder=1
        else
            remainder=0
        fi
        active_flags[$i]=${val}
        i=$(( i + 1 ))
    done
    if [ ${remainder} = 1 ]; then
        # end of flag check
        break
    fi
done

cat size-check.txt | sort | head
