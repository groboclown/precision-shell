#!/bin/bash

# Make a call into the server and ensure it's on the right UID and GID.

# curl -s : silent mode.
#      -L : follow redirects

server_pid="$( curl -sL http://localhost:33000/uid )"
server_gid="$( curl -sL http://localhost:33000/gid )"

if [ "${server_pid}" != 65532 ] || [ "${server_gid}" != 65532 ] ; then
    echo "Error: expected pid/gid 65532/65532, found ${server_pid}/${server_gid}"
    exit 1
fi
