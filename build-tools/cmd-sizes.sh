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

exargs=""
if [ "${NO_GETADDRINFO}" = 1 ] ; then
    exargs="NO_GETADDRINFO=1"
fi

sizefile=/tmp/sized.txt
test -f "${sizefile}" && rm "${sizefile}"

i=0
while [ $i -lt ${set_count} ] ; do
    name="${command_set_names[$i]}"
    cmds="${command_sets[$i]}"
    # echo "$i : ${name} - ${cmds}"
    outdir=/tmp/presh-$$
    mkdir -p "${outdir}"
    ( cd "../src" && make src "${cmds}" ${exargs} OUTDIR="${outdir}" >/dev/null 2>&1 )
    if [ $? != 0 ] ; then
        >&2 echo "Build failed for: make src ${cmds} ${exargs} OUTDIR=${outdir}"
        exit 1
    fi
    ( cd "../compressed" && make build OUTDIR="${outdir}" >/dev/null 2>&1 )
    if [ $? != 0 ] ; then
        >&2 echo "Build failed for: make compressed OUTDIR=${outdir}"
        exit 1
    fi
    if [ ! -f "${outdir}/presh" ] ; then
        echo "Build failed for: make ${cmds} OUTDIR=${outdir}" >&2
        exit 1
    else
        filesize=$( wc -c <"${outdir}/presh" )
        echo "${filesize}|${name}" >> "${sizefile}"
        # echo "${filesize} < ${name}"

        # See internal-docker-make.sh
        fz1=0
        cmp=""
        # no lk at the moment, as it doesn't work.
        for f in fd-tinflate fd-tinyzzz-lzma fd-tinyzzz-zstd ; do
            if [ -f "${outfile}/presh-${f}" ] ; then
                if [ -z "${cmp}" ] ; then
                    cmp="${f}"
                    fz1=$( wc -c <"out/presh-${f}" )
                else
                    fz2=$( wc -c <"out/presh-${f}" )
                    if [ ${fz2} -lt ${fz1} ] ; then
                        cmp="${f}"
                        fz1=${fz2}
                    fi
                fi
            fi
        done

        if [ ! -z "${cmp}" ] ; then
            echo "${fz1}|${name} (${cmp} compressed)" >> "${sizefile}"
        fi
    fi
    rm -rf "${outdir}"

    i=$(( i + 1 ))
done

cat "${sizefile}"
# rm "${sizefile}"
