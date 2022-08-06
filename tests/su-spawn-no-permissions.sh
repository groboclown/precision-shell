#!/bin/bash

# desc: su-spawn when run without the right permissions
# requires: +su-spawn

id_exec=$( which id )
if [ -z "${UID1}" ] || [ -z "${UID2}" ] || [ -z "${GID1}" ] || [ -z "${GID2}" ] ; then
    echo "?? SKIPPED due to missing UID1, UID2, GID1, or GID2"
    exit 0
fi
# Note: 'id' on BusyBox does not look up from ID, just from name.
user1="$( getent passwd "${UID1}" | cut -f 1 -d ':' )"
user2="$( getent passwd "${UID2}" | cut -f 1 -d ':' )"
group1="$( getent group "${GID1}" | cut -f 1 -d ':' )"
group2="$( getent group "${GID2}" | cut -f 1 -d ':' )"

if [ -z "${user1}" ] || [ -z "${user2}" ] || [ -z "${group1}" ] || [ -z "${group2}" ] ; then
    echo "?? SKIPPED because UID1 (${UID1}), UID2 (${UID2}), GID1 (${GID1}), or GID2 (${GID2}) are missing"
    exit 0
fi

if [ "$( id -u )" != "0" ] ; then
    if [ "$( id -u )" != "${UID1}" ] ; then
        "${FS}" -c "su-spawn [${UID1}] [${GID1}] [${id_exec} -u]" > out.txt 2>err.txt
        res=$?
    else
        "${FS}" -c "su-spawn [${UID2}] [${GID2}] [${id_exec} -u]" > out.txt 2>err.txt
        res=$?
    fi
else
    su -s "${FS}" "${user2}" -c "su-spawn [${UID1}] [${GID1}] [${id_exec} -u]" > out.txt 2>err.txt
    res=$?
fi

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat out.txt
    exit 1
fi

if [ "$( printf "Failed to su-exec ${id_exec}\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated invalid stderr"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
