#!/bin/bash

# Compile with different common command settings to see the sizes.

cd "$( dirname "$0" )"

command_set_names=(
    "all commands" \
    "non-network commands" \
    "standard commands" \
    "minimal commands" \
    "no commands"
)
command_sets=(
    "INCLUDE_ALL_COMMANDS=1" \
    "INCLUDE_NON_NETWORK_COMMANDS=1" \
    "INCLUDE_STANDARD_COMMANDS=1"
    "INCLUDE_MINIMAL_COMMANDS=1" \
    "COMMAND_FLAGS=" \
)

set_count="${#command_sets[@]}"

sizefile=/tmp/$$-sized.txt
test -f "${sizefile}" && rm "${sizefile}"

i=0
while [ $i -lt ${set_count} ] ; do
    name="${command_set_names[$i]}"
    cmds="${command_sets[$i]}"
    # echo "$i : ${name} - ${cmds}"
    outdir=/tmp/presh-$$
    mkdir -p "${outdir}"
    ( cd "../src" && make src "${cmds}" OUTDIR="${outdir}" >/dev/null 2>&1 )
    if [ $? != 0 ] || [ ! -f "${outdir}/presh" ] ; then
        echo "Build failed for: make src ${cmds} OUTDIR=${outdir}" >&2
    else
        filesize=$( wc -c <"${outdir}/presh" )
        echo "${filesize}|${name}" >> "${sizefile}"
        # echo "${filesize} < ${name}"
    fi
    rm -rf "${outdir}"

    i=$(( i + 1 ))
done

cat "${sizefile}"
rm "${sizefile}"
