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


outdir=/tmp/presh-$$
mkdir -p "${outdir}"
( cd "$( dirname "$0" )/../../src" && make src MINFLAGS="${minflags}" "${COMMAND_FLAGS}" OUTDIR="${outdir}" >/dev/null 2>&1 )
if [ $? != 0 ] || [ ! -f "${outdir}/presh" ] ; then
    echo "${minflags}" >> "${badfile}"
else
    filesize1=$( wc -c <"${outdir}/presh" )
    filesize2=$( wc -c <"${outdir}/presh-fd" )
    rm -r "${outdir}"
    echo "${filesize1}|${minflags}|std|${COMMAND_FLAGS}" >> "${sizefile}"
    echo "${filesize2}|${minflags}|compressed|${COMMAND_FLAGS}" >> "${sizefile}"
    echo "${filesize1} < ${minflags} (std)"
    echo "${filesize2} < ${minflags} (compressed)"
fi
