#!/bin/bash

# Run the test suite for each command that can be enabled.
cd "$( dirname "$0" )"

command_list_raw="$( egrep "^CMD_" ../Makefile.command-flags | cut -f 3 -d ' ' )"

# Extra Flag Combos - see "gen-extra-flag-combos.sh"
streaming_arg="$( egrep "^STREAMING_INPUT = " ../Makefile.command-flags | cut -f 3 -d ' ' )"
reqargs_arg="$( egrep "^REQUIRE_FULL_CMD = " ../Makefile.command-flags | cut -f 3 -d ' ' )"
enviro_arg="$( egrep "^ENVIRO_INPUT = " ../Makefile.command-flags | cut -f 3 -d ' ' )"

# Slimmed down flag combinations, for ones that matter.
extra_flag_combos=( \
    "" \
    "${streaming_arg}" \
    "${enviro_arg}" \
    "${streaming_arg} ${reqargs_arg} ${enviro_arg}" \
)
extra_name_combos=( \
    "" \
    "-input" \
    "-enviro" \
    "-input-reqargs-enviro"
)
extra_flag_count="${#extra_flag_combos[@]}"

command_list=(${command_list_raw})

echo "Complete command variation list: ${command_list[@]} + ${streaming_arg}"

bindir=/tmp/fs-shell-bin-$$
mkdir -p "${bindir}"
failures=/tmp/failures-$$.txt
touch "${failures}"
fail_count=0
for cmd in "${command_list[@]}" ; do
    cmd_name="${cmd:10}"
    cmd_name=$( echo "${cmd_name}" | tr '[:upper:]' '[:lower:]' | tr _ - )
    flag_index=0
    while [ ${flag_index} -lt ${extra_flag_count} ] ; do
        exe_name="fs-shell-${cmd_name}${extra_name_combos[${flag_index}]}"
        cmdarg="COMMAND_FLAGS=${cmd} ${extra_flag_combos[${flag_index}]}"
        echo "${cmdarg}"
        flag_index=$(( flag_index + 1 ))

        mkout=/tmp/fs-shell-$$.txt
        ( cd ../src && make "${cmdarg}" >"${mkout}" 2>&1 )
        if [ $? != 0 ] ; then
            # compile failure is just bad
            echo "Build failure with:"
            echo "${cmdarg}"
            cat "${mkout}"
            rm "${mkout}"
            exit 1
        fi
        cp ../out/fs-shell "${bindir}/${exe_name}"
        rm "${mkout}"
        touch "${mkout}"

        # could also check if +input is in version...

        ../out/fs-shell version | grep " +${cmd_name}" >/dev/null 2>&1
        if [ $? != 0 ]; then
            echo "===================================================" >> "${failures}"
            echo "${cmdarg}" >> "${failures}"
            ../out/fs-shell version >> "${failures}"
            echo "No command flag '${cmd_name}' in version listed." >> "${failures}"
        else
            echo "===================================================" > "${mkout}"
            echo "${cmdarg}" >> "${mkout}"
            ( cd ../tests && QUIET=1 make tests >> "${mkout}" 2>&1 )
            if [ $? != 0 ] ; then
                t=/tmp/tmp.$$.txt
                cat "${failures}" "${mkout}" > "${t}"
                rm "${failures}"
                mv "${t}" "${failures}"
                fail_count=$(( fail_count + 1 ))
            fi
        fi
        rm "${mkout}"
    done
done
echo "==================================================="
cat "${failures}"
for n in "${bindir}/"* ; do
    if [ -x "${n}" ] ; then
        filesize=$( wc -c <"${n}" )
        nn="$( basename "${n}" )"
        echo "${nn} : ${filesize}"
    fi
done 
rm -r "${bindir}"
rm "${failures}"
exit ${fail_count}
