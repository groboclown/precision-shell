#!/bin/bash
set -e

# Use our own network.
"${CONTAINER_RUNNER}" network create "non-root-1"

# Start up the container.
"${CONTAINER_RUNNER}" run \
    --name "non-root-1" \
    --network "non-root-1" \
    -p 33000:3000 \
    -d --rm "local/presh-non-root:build"

# Give it a chance to start, and ensure it's running.
sleep 1
is_running="$( "${CONTAINER_RUNNER}" inspect non-root-1 | jq -r ".[0].State.Status" )"
if [ "${is_running}" != "running" ] ; then
    echo "Did not start container."
    exit 1
fi
