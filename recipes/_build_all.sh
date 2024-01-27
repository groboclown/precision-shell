#!/bin/bash

if [ "$1" = "-h" ] || [ "$1" = "--help" ] ; then
    echo "Usage: $0"
    echo ""
    echo "Runs all of these docker files to ensure they build and run without error."
    exit 0
fi


CONTAINER_RUNNER="${CONTAINER_RUNNER:-docker}"

# Random, private ipv6 subnet.
IPV6_PRIVATE_SUBNET="${IPV6_PRIVATE_SUBNET:-fd76:0bca:85f3:17a0::/64}"

# ipv6 is required for some of the networking tests.
if
    ! "${CONTAINER_RUNNER}" network ls | grep presh-net >/dev/null 2>&1
then
    if
        ! "${CONTAINER_RUNNER}" network create --ipv6 --subnet "${IPV6_PRIVATE_SUBNET}" presh-net
    then
        echo "Failed to create ipv6 network for the tests.  Cannot proceed."
        exit 1
    fi
fi

# Note that, to use the ipv6 network during the build time, this requires some
#  special magic that's different between podman and docker.
BUILD_CMD=("${CONTAINER_RUNNER}" build --network=presh-net)
if [ $( "${CONTAINER_RUNNER}" --version | cut -f 1 -d ' ' ) != "podman" ] ; then
    if
        ! "${CONTAINER_RUNNER}" buildx create --name=presh-build --driver=docker-container --driver-opt="network=presh-net"
    then
        echo "Failed to create IPv6 network builder for the tests.  Cannot proceed."
        exit 1
    fi
    BUILD_CMD=("${CONTAINER_RUNNER}" buildx build --builder=presh-build)
else
    echo "Using:"
    ${CONTAINER_RUNNER} --version
    echo "Invoking it by the old build style."
fi

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

"${CONTAINER_RUNNER}" network rm presh-net

exit ${errs}
