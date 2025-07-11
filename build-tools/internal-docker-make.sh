#!/bin/bash
set -e

# File invoked through Dockerfile to run the build.
echo "Running in [${BUILD_MODE}]"

cd "$( dirname "$0" )/.."

case "${BUILD_MODE}" in
    compile-flag-sizes)
        build-tools/size-check/compile-flag-sizes.sh
        ;;
    cmd-size)
        build-tools/cmd-sizes.sh
        ;;
    src)
        ( cd src && make src )
        ls -lA out/presh*
        ;;
    combinatorics-test)
        build-tools/combinatorics-test.sh
        ;;
    individual-cmd-test)
        build-tools/individual-cmds.sh
        ;;
    combos)
        mkdir -p .tmp/combos
        make src tests || echo "Tests failed"
        cp out/presh .tmp/combos/presh-zero
        cp out/presh-debug .tmp/combos/presh-zero-debug

        make INCLUDE_ALL_COMMANDS=1 src tests || echo "Tests failed"
        cp out/presh .tmp/combos/presh-all
        cp out/presh-debug .tmp/combos/presh-all-debug

        make INCLUDE_MINIMAL_COMMANDS=1 src tests || echo "Tests failed"
        cp out/presh .tmp/combos/presh-min
        cp out/presh-debug .tmp/combos/presh-min-debug

        make INCLUDE_STANDARD_COMMANDS=1 src tests || echo "Tests failed"
        cp out/presh .tmp/combos/presh-std
        cp out/presh-debug .tmp/combos/presh-std-debug

        cp .tmp/combos/* out/.

        echo "presh-zero" > out/build-inventory.txt
        echo "presh-zero-debug" >> out/build-inventory.txt
        echo "presh-all" >> out/build-inventory.txt
        echo "presh-all-debug" >> out/build-inventory.txt
        echo "presh-min" >> out/build-inventory.txt
        echo "presh-min-debug" >> out/build-inventory.txt
        echo "presh-std" >> out/build-inventory.txt
        echo "presh-std-debug" >> out/build-inventory.txt

        ls -lAS out/presh*
        ;;
    interactive)
        # Do nothing, to allow a shell to run with the image.

        ;;
    *)
        commands=""
        if [ "${COMMANDS}" = "ALL" ] ; then
            cmdarg="INCLUDE_ALL_COMMANDS=1"
        else
            for cmd in ${COMMANDS} ; do
                # see version.h for mapping the command name to the
                # command argument.
                if [ ${cmd} = "input" ] ; then
                    cmd_arg="-DUSE_STREAMING_INPUT"
                elif [ ${cmd} = "enviro" ] ; then
                    cmd_arg="-DUSE_ENVIRONMENT_INPUT"
                elif [ ${cmd} = "reqargs" ] ; then
                    cmd_arg="-DREQUIRE_FULL_CMD"
                else
                    cmd_arg="$( echo ${cmd^^} | tr - _ )"
                    cmd_arg="-DUSE_CMD_${cmd_arg}"
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
        exargs=""
        if [ "${NO_GETADDRINFO}" = 1 ] ; then
            exargs="NO_GETADDRINFO=1"
        fi
        echo "make \"${cmdarg}\" ${exargs} all"
        make "${cmdarg}" ${exargs} all
        if [ -f "out/presh" ] && [ -f "out/presh-zipped" ] ; then
            fz1=$( wc -c <"out/presh" )
            fz2=$( wc -c <"out/presh-zipped" )
            if [ ${fz1} -lt ${fz2} ] ; then
                cp out/presh out/presh-smallest
            else
                cp out/presh-zipped out/presh-smallest
            fi
        fi
        ls -lAS out/presh*

        ;;
esac
