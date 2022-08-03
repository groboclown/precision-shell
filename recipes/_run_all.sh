#!/bin/bash

if [ "$1" = "-h" ] || [ "$1" = "--help" ] ; then
    echo "Usage: $0"
    echo ""
    echo "Runs all of these docker files to ensure they build and run without error."
    exit 0
fi


CONTAINER_RUNNER=${CONTAINER_RUNNER:-docker}

cd "$( dirname "$0" )/.."
this_dir="recipes"

errs=0
for name in ${this_dir}/*.Dockerfile ; do
    base="$( basename "${name}" .Dockerfile )"
    "${CONTAINER_RUNNER}" build -t "local/presch-${base}" -f "${name}" .
    if [ $? != 0 ] ; then
        errs=$(( errs + 1 ))
    fi
done

if [ ${errs} != 0 ] ; then
    echo "${errs} failures"
fi

exit ${errs}
