#!/bin/bash

if [ "$1" = "-h" ] || [ "$1" = "--help" ] ; then
    echo "Usage: $0"
    echo ""
    echo "Runs all of these docker files to ensure they build and run without error."
    exit 0
fi


CONTAINER_RUNNER="${CONTAINER_RUNNER:-docker}"

mk_network=0
network_args=()
if [ -z "${NETWORK}" ] ; then
    if [ "${IPV6}" != "no" ] ; then
        # Random, private ipv6 subnet.
        IPV6_PRIVATE_SUBNET="${IPV6_PRIVATE_SUBNET:-fd76:0bca:85f3:17a0::/64}"
        NETWORK=presh-net

        # ipv6 is required for some of the networking tests.
        if ! \
            "${CONTAINER_RUNNER}" network ls | grep "${NETWORK}" >/dev/null 2>&1 \
        ; then
            if ! \
                "${CONTAINER_RUNNER}" network create --ipv6 --subnet "${IPV6_PRIVATE_SUBNET}" "${NETWORK}" \
            ; then
                echo "Failed to create ipv6 network for the tests.  Skipping IPV6."
                IPV6=no
            else
                IPV6=yes
                mk_network=1
                network_args=(--network "${NETWORK}")
            fi
        fi
    fi
else
    network_args=(--network "${NETWORK}")
    IPV6=no
fi
export IPV6


cd "$( dirname "$0" )/.."
this_dir="recipes"

errs=0
declare -a err_names
for name in ${this_dir}/*.Dockerfile ; do
    base="$( basename "${name}" .Dockerfile )"
    "${CONTAINER_RUNNER}" buildx build "${network_args[@]}" -t "local/presh-${base}:build" -f "${name}" .
    if [ $? != 0 ] ; then
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

if [ "${mk_network}" = 1 ] ; then
    "${CONTAINER_RUNNER}" network rm "${NETWORK}"
fi

exit ${errs}
