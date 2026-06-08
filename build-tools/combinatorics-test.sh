#!/bin/bash

# Run the full test suite, over all combination of commands.
# This can take a while to run.
# At initial time of writing, there were 39 commands + 3 other flags, for a total of 42 combinations.
# That's 2^42 combinations to try (4,398,046,511,104).
# If this runs 1 combination per second, it will take 139,461 years.
# So, a while.
cd "$( dirname "$0" )"

command_list_raw="$( egrep "^CMD_" ../Makefile.command-flags | cut -f 3 -d ' ' )"
streaming_arg="$( egrep "^STREAMING_INPUT = " "${flag_file}" | cut -f 3 -d ' ' )"
reqargs_arg="$( egrep "^REQUIRE_FULL_CMD = " "${flag_file}" | cut -f 3 -d ' ' )"
enviro_arg="$( egrep "^ENVIRO_INPUT = " "${flag_file}" | cut -f 3 -d ' ' )"
command_list=(${command_list_raw} ${streaming_arg} ${reqargs_arg} ${enviro_arg})

echo "Complete command variation list: ${command_list[@]}"


cmd_count="${#command_list[@]}"

# Basically, we want each flag to be a bit, either on or off.
active_cmds=()
for val in "${command_list[@]}" ; do
    active_cmds+=(0)
done

running=1
while [ ${running} = 1 ] ; do
    commands=""
    i=0
    while [ $i -lt "${cmd_count}" ] ; do
        if [ "${active_cmds[$i]}" = 1 ] ; then
            if [ -z "${commands}" ] ; then
                commands="${command_list[$i]}"
            else
                commands="${commands} ${command_list[$i]}"
            fi
        fi
        i=$(( i + 1 ))
    done

    cmdarg="COMMAND_FLAGS=${commands}"

    # This could switch to running the tests in the background.
    # However, if we go that way, this should instead use Makefile,
    # as that has very good built-in job controls.

    echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
    echo "${cmdarg}"

    mkout=/tmp/presh-$$.txt
    ( cd ../src && make "${cmdarg}" >"${mkout}" 2>&1 )
    if [ $? != 0 ] ; then
        # compile failure is just bad
        echo "Build failure with:"
        echo "${cmdarg}"
        cat "${mkout}"
        rm "${mkout}"
        exit 1
    fi
    rm "${mkout}"

    ( cd ../tests && QUIET=1 make tests CMDS=presh )

    # change commands
    remainder=1
    i=0
    while [ $i -lt "${cmd_count}" ] ; do
        val=$(( active_cmds[$i] + remainder ))
        if [ ${val} = 2 ]; then
            val=0
            remainder=1
        else
            remainder=0
        fi
        active_cmds[$i]=${val}
        i=$(( i + 1 ))
    done
    if [ ${remainder} = 1 ]; then
        # end of permutations.  Quit.
        running=0
    fi
done
