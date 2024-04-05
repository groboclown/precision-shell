#!/bin/bash

# A small script that runs the ${FS_SERVER} on a randomly assigned port number.
# This takes arguments:
#   1: file to write the port number to.
#   2: file to write the server PID to.
#   3: stdout file
#   4: stderr file
#   5: server command (e.g. 'echo')
#   6+ server other commands
# This exits with non-zero exit code if the port could not be allocated.

port_file="$1"
shift
pid_file="$1"
shift
stdout_file="$1"
shift
stderr_file="$1"
shift
server_cmd="$1"
shift
# server_args=$@

test -f "${stdout_file}" && rm "${stdout_file}"
test -f "${stderr_file}" && rm "${stderr_file}"

# Allocate an ephemeral port, > 32767.
port=33$$
while [ "${#port}" -lt 5 ] ; do
  port="${port}1"
done
port="${port:0:5}"
port_incr="${port:3:1}"

alive=0
retries=15

# make sure this uses the kill program, not the Bash built-in kill.
kill_exec="$( which kill )" || exit 1

while [ ${alive} = 0 ] && [ ${retries} -gt 0 ] ; do
    echo "(with port: ${port}, remaining tries: ${retries})" >> "${stdout_file}"
    (
        "${FS_SERVER}" ${server_cmd} ${port} "$@" ;
        2>&1 echo "Exited with: $?" ;
     ) >>"${stdout_file}" 2>>"${stderr_file}" &
    server_pid=$!
    if "${kill_exec}" "-0" "${server_pid}" ; then
        alive=1
    else
        retries=$(( retries - 1 ))
        port=$(( port + port_incr ))
    fi
done
if [ ${alive} = 0 ] ; then
    2>&1 echo "Failed to launch server after 5 retries; final port ${port}"
    2>&1 echo "stdout:"
    2>&1 cat "${stdout_file}"
    2>&1 echo "stderr:"
    2>&1 cat "${stderr_file}"
    exit 1
fi

echo "${port}" > "${port_file}"
echo "${server_pid}" > "${pid_file}"

# Wait a touch for the server to start.
sleep 0.1

exit 0
