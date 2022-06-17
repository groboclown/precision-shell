#!/bin/bash

# Run the build with the flag arguments and gather the results.
# Designed to run in parallel.

sizefile="$1"
shift
badfile="$1"
shift
minflags="$@"

# Test compilation to get the smallest size possible.


outdir=/tmp/fs-shell-$$
mkdir -p "${outdir}"
( cd "$( dirname "$0" )/../../src" && make src MINFLAGS="${minflags}" OUTDIR="${outdir}" >/dev/null 2>&1 )
if [ $? != 0 ] || [ ! -f "${outdir}/fs-shell" ] ; then
    echo "${minflags}" >> "${badfile}"
else
    filesize=$( wc -c <"${outdir}/fs-shell" )
    rm -r "${outdir}"
    echo "${filesize}|${minflags}" >> "${sizefile}"
    echo "${filesize} < ${minflags}"
fi
