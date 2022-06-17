#!/bin/bash

# Run the build with the flag arguments and gather the results.
# Designed to run in parallel.

sizefile="$1"
shift
badfile="$1"
shift
minflags="$@"

if [ -z "${COMMAND_FLAGS}" ] ; then
    COMMAND_FLAGS="INCLUDE_ALL_COMMANDS=1"
fi

# Test compilation to get the smallest size possible.


outdir=/tmp/fs-shell-$$
mkdir -p "${outdir}"
( cd "$( dirname "$0" )/../../src" && make src MINFLAGS="${minflags}" "${COMMAND_FLAGS}" OUTDIR="${outdir}" >/dev/null 2>&1 )
if [ $? != 0 ] || [ ! -f "${outdir}/fs-shell" ] ; then
    echo "${minflags}" >> "${badfile}"
else
    filesize=$( wc -c <"${outdir}/fs-shell" )
    rm -r "${outdir}"
    echo "${filesize}|${minflags}|${COMMAND_FLAGS}" >> "${sizefile}"
    echo "${filesize} < ${minflags}"
fi
