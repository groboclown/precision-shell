#!/bin/bash

# Run the test suite for each command that can be enabled.
cd "$( dirname "$0" )"

command_list_raw="$( egrep "^CMD_" ../Makefile.command-flags | cut -f 3 -d ' ' )"
streaming_arg="$( egrep "^STREAMING_INPUT = " ../Makefile.command-flags | cut -f 3 -d ' ' )"
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
    for use_stream in 0 1 ; do
        exe_name="fs-shell-${cmd_name}"
        cmdarg="COMMAND_FLAGS=${cmd}"
        if [ "${use_stream}" = 1 ] ; then
            cmdarg="${cmdarg} ${streaming_arg}"
            exe_name="${exe_name}-input"
        fi
        echo "${cmdarg}"

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
