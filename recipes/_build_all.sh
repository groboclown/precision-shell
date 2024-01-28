#!/bin/bash

if [ "$1" = "-h" ] || [ "$1" = "--help" ] ; then
    echo "Usage: $0"
    echo ""
    echo "Runs all of these docker files to ensure they build and run without error."
    exit 0
fi


CONTAINER_RUNNER="${CONTAINER_RUNNER:-docker}"

# ipv6 is required for some of the networking tests.
BUILD_CMD=("${CONTAINER_RUNNER}" build)

cd "$( dirname "$0" )/.."
this_dir="recipes"

errs=0
declare -a err_names
for name in ${this_dir}/*.Dockerfile ; do
    base="$( basename "${name}" .Dockerfile )"
    if
        ! "${BUILD_CMD[@]}" -t "local/presh-${base}:build" -f "${name}" .
    then
        errs=$(( errs + 1 ))
        err_names+=("${name}")
    fi
done

if [ ${errs} != 0 ] ; then
    echo "Failed: ${err_names[*]}"
    echo "${errs} failures"
else
    echo "Completed without errors."
fi


exit ${errs}
