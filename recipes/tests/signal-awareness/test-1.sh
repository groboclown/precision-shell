#!/bin/bash


for signal in 1 2 9 15 17 ; do
    echo "Trying signal ${signal}"

    # Start up the container.  Run it in interactive mode so
    #   the shell can send it signals.
    "${CONTAINER_RUNNER}" run \
        --name "signal-awareness-${signal}" \
        --network "signal-awareness-1" \
        -p 33000:3000 \
        -d --rm "local/presh-signal-awareness:build"

    # Give it a chance to start, and ensure it's running.
    sleep 1

    # Check if it started.
    is_running="$( "${CONTAINER_RUNNER}" inspect "signal-awareness-${signal}" | jq -r ".[0].State.Status" )"
    if [ "${is_running}" != "running" ] ; then
        echo "Did not start container."
        "${CONTAINER_RUNNER}" kill "signal-awareness-${signal}"
        exit 1
    fi
    if curl -s http://localhost:33000 > /dev/null 2>&1 ; then
        # It's running.
        echo -n "."
    else
        echo "Failed to start server."
        exit 1
    fi

    # Stop the server by passing a signal via the kill --signal command
    "${CONTAINER_RUNNER}" kill --signal ${signal} "signal-awareness-${signal}" || exit 1

    # give it some time to stop
    sleep 1

    if curl -s http://localhost:33000 > /dev/null 2>&1 ; then
        # It's still running.
        echo "Failed to stop the server with signal ${signal}."
        exit 1
    fi

done
