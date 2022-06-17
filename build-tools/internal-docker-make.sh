#!/bin/bash
set -e

# File invoked through Dockerfile to run the build.
echo "Running in [${BUILD_MODE}]"

cd "$( dirname "$0" )/.."

if [ "${BUILD_MODE}" = "size-check" ] ; then
    build-tools/size-check/compile-size-test.sh
elif [ "${BUILD_MODE}" = "src" ] ; then
    ( cd src && make src )
    ls -lA out/fs-shell*
elif [ "${BUILD_MODE}" = "full-test" ] ; then
    build-tools/full-tests.sh
elif [ "${BUILD_MODE}" = "combos" ] ; then
    mkdir -p .tmp/combos
    make src tests || echo "Tests failed"
    cp out/fs-shell .tmp/combos/fs-shell-zero
    cp out/fs-shell-debug .tmp/combos/fs-shell-zero-debug

    make INCLUDE_ALL_COMMANDS=1 src tests || echo "Tests failed"
    cp out/fs-shell .tmp/combos/fs-shell-all
    cp out/fs-shell-debug .tmp/combos/fs-shell-all-debug

    make INCLUDE_MINIMAL_COMMANDS=1 src tests || echo "Tests failed"
    cp out/fs-shell .tmp/combos/fs-shell-min
    cp out/fs-shell-debug .tmp/combos/fs-shell-min-debug

    make INCLUDE_STANDARD_COMMANDS=1 src tests || echo "Tests failed"
    cp out/fs-shell .tmp/combos/fs-shell-std
    cp out/fs-shell-debug .tmp/combos/fs-shell-std-debug

    cp .tmp/combos/* out/.

    echo "fs-shell-zero" > out/build-inventory.txt
    echo "fs-shell-zero-debug" >> out/build-inventory.txt
    echo "fs-shell-all" >> out/build-inventory.txt
    echo "fs-shell-all-debug" >> out/build-inventory.txt
    echo "fs-shell-min" >> out/build-inventory.txt
    echo "fs-shell-min-debug" >> out/build-inventory.txt
    echo "fs-shell-std" >> out/build-inventory.txt
    echo "fs-shell-std-debug" >> out/build-inventory.txt

    ls -lAS out/fs-shell*
else
    commands=""
    if [ "${COMMANDS}" = "ALL" ] ; then
        cmdarg="INCLUDE_ALL_COMMANDS=1"
    else
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
    fi
    echo "make \"${cmdarg}\" all"
    make "${cmdarg}" all
    ls -lAS out/fs-shell*
fi
