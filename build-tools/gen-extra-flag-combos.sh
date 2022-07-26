#!/bin/bash

# Generates all extra flag combos.
# One combo per line.

cd "$( dirname "$0" )"

flag_file="${1:-../Makefile.command-flags}"

# All the extra, non-command toggle flags
#   Does not include "debug".
streaming_arg="$( egrep "^STREAMING_INPUT = " "${flag_file}" | cut -f 3 -d ' ' )"
reqargs_arg="$( egrep "^REQUIRE_FULL_CMD = " "${flag_file}" | cut -f 3 -d ' ' )"
enviro_arg="$( egrep "^ENVIRO_INPUT = " "${flag_file}" | cut -f 3 -d ' ' )"
extra_flags=("${streaming_arg}" "${reqargs_arg}" "${enviro_arg}")
flag_count=${#extra_flags[@]}

declare -a include_flag
index=0
while [ ${index} -lt ${flag_count} ] ; do
    include_flag+=(0)
    index=$(( index + 1 ))
done

# declare -a _flag_combos
do_it=1
combo=""
while [ ${do_it} = 1 ] ; do
    # use as-is, first
    index=0
    combo=""
    while [ ${index} -lt ${flag_count} ] ; do
        if [ ${include_flag[${index}]} = 1 ] ; then
            if [ -z "${combo}" ] ; then
                combo="${extra_flags[${index}]}"
            else
                combo="${combo} ${extra_flags[${index}]}"
            fi
        fi
        index=$(( index + 1 ))
    done
    # _flag_combos+=("${combo}")
    echo "${combo}"

    # at the end, increment the counter.
    find_next=1
    index=0
    while [ ${index} -lt ${flag_count} ] && [ ${find_next} = 1 ] ; do
        if [ ${include_flag[${index}]} = 1 ] ; then
            include_flag[${index}]=0
            index=$(( index + 1 ))
        else
            include_flag[${index}]=1
            find_next=0
        fi
    done
    if [ ${find_next} = 1 ] && [ ${index} -ge ${flag_count} ] ; then
        # finished looking through the list.
        do_it=0
    fi
done
