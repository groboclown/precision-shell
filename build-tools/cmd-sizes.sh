#!/bin/bash

# Compile with different common command settings to see the sizes.

cd "$( dirname "$0" )"

command_set_names=(
    "all commands" \
    "standard commands" \
    "minimal commands" \
    "no commands"
)
command_sets=(
    "INCLUDE_ALL_COMMANDS=1" \
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
    outdir=/tmp/fs-shell-$$
    mkdir -p "${outdir}"
    ( cd "../src" && make src "${cmds}" OUTDIR="${outdir}" >/dev/null 2>&1 )
    if [ $? != 0 ] || [ ! -f "${outdir}/fs-shell" ] ; then
        echo "Build failed for: make src ${cmds} OUTDIR=${outdir}" >&2
    else
        filesize=$( wc -c <"${outdir}/fs-shell" )
        echo "${filesize}|${name}" >> "${sizefile}"
        # echo "${filesize} < ${name}"
    fi
    rm -rf "${outdir}"

    i=$(( i + 1 ))
done

cat "${sizefile}" | sort | head -n 30
rm "${sizefile}"
