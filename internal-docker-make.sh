#!/bin/bash
set -e

# File invoked through Dockerfile to run the build.
echo "Running in [${BUILD_MODE}]"

if [ "${BUILD_MODE}" = "size-check" ] ; then
    experiments/size-check/compile-size-test.sh
elif [ "${BUILD_MODE}" = "src" ] ; then
    ( cd src && make src )
    ls -lA out/fs-shell*
elif [ "${BUILD_MODE}" = "full-test" ] ; then
    experiments/full-tests.sh
else
    commands=""
    for cmd in ${COMMANDS} ; do
        if [ ${cmd} = "input" ] ; then
            cmd_arg="-DUSE_STREAMING_INPUT=1"
        else
            cmd_arg="$( echo ${cmd^^} | tr - _ )"
            cmd_arg="-DUSE_CMD_${cmd_arg}=1"
        fi
        if [ -z "${commands}" ] ; then
            commands="${cmd_arg}"
        else
            commands="${commands} ${cmd_arg}"
        fi
    done
    cmdarg=""
    if [ ! -z "${commands}" ] ; then
        cmdarg="COMMAND_FLAGS=${commands}"
    fi
    echo "make \"${cmdarg}\" all"
    make "${cmdarg}" all
    ls -lAS out/fs-shell*
fi
