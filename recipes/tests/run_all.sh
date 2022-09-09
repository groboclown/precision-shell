#!/bin/bash

cd "$( dirname "$0" )"

CONTAINER_RUNNER="${CONTAINER_RUNNER:-docker}"
base_tmpdir="/tmp/presh-recipe-test-$$"

# Random, private ipv6 subnet.
IPV6_PRIVATE_SUBNET="${IPV6_PRIVATE_SUBNET:-fd76:0bca:85f3:17a0::/64}"

export CONTAINER_RUNNER IPV6_PRIVATE_SUBNET

run=0
fail=0
for testdir in * ; do
    if [ -d "${testdir}" ] ; then
        echo "=========================================="
        echo "${testdir}"
        chmod +x "${testdir}/"*.sh
        do_run=1
        echo "--- Setup"
        TEST_TMP_DIR="${base_tmpdir}/${testdir}"
        export TEST_TMP_DIR
        mkdir -p "${TEST_TMP_DIR}"
        before_start_containers="${TEST_TMP_DIR}/init-containers.txt"
        "${CONTAINER_RUNNER}" ps -a -q | sort > "${before_start_containers}" || exit 1
        before_start_networks="${TEST_TMP_DIR}/init-networks.txt"
        "${CONTAINER_RUNNER}" network ls -q | sort > "${before_start_networks}" || exit 1
        before_start_volumes="${TEST_TMP_DIR}/init-volumes.txt"
        "${CONTAINER_RUNNER}" volume ls -q | sort > "${before_start_volumes}" || exit 1
        for s in "${testdir}/"_setup-*.sh ; do
            if [ "${do_run}" = 1 ] ; then
                "${s}"
                if [ $? != 0 ] ; then
                    fail=$(( fail + 1 ))
                    do_run=0
                    echo "FAIL:: ${s}"
                fi
            fi
        done
        echo "--- Tests"
        for s in "${testdir}/"test*.sh ; do
            n="$( basename "${s}" .sh )"
            if [ "${do_run}" = 1 ] ; then
                run=$(( run + 1 ))
                echo "--- :: ${n}"
                "${s}"
                if [ $? != 0 ] ; then
                    fail=$(( fail + 1 ))
                    do_run=0
                    echo "FAIL:: ${s}"
                fi
            fi
        done
        echo "--- Teardown"
        for s in "${testdir}/"_teardown-*.sh ; do
            if [ "${do_run}" = 1 ] ; then
                "${s}"
            fi
        done
        after_end_containers="${TEST_TMP_DIR}/final-containers.txt"
        "${CONTAINER_RUNNER}" ps -a -q | sort > "${after_end_containers}" || exit 1
        after_end_networks="${TEST_TMP_DIR}/final-networks.txt"
        "${CONTAINER_RUNNER}" network ls -q | sort > "${after_end_networks}" || exit 1
        after_end_volumes="${TEST_TMP_DIR}/final-volumes.txt"
        "${CONTAINER_RUNNER}" volume ls -q | sort > "${after_end_volumes}" || exit 1

        # Remove all containers that were created after the test started and are still around.
        new_containers="${TEST_TMP_DIR}/new-containers.txt"
        comm -1 "${before_start_containers}" "${after_end_containers}" > "${new_containers}"
        if [ "$( wc -l < "${new_containers}" )" != 0 ] ; then
            "${CONTAINER_RUNNER}" kill $( cat "${new_containers}" ) >/dev/null 2>&1
            "${CONTAINER_RUNNER}" rm $( cat "${new_containers}" ) >/dev/null 2>&1
        fi

        # ... and networks
        new_networks="${TEST_TMP_DIR}/new-networks.txt"
        comm -1 "${before_start_networks}" "${after_end_networks}" > "${new_networks}"
        if [ "$( wc -l < "${new_networks}" )" != 0 ] ; then
            "${CONTAINER_RUNNER}" network rm $( cat "${new_networks}" ) >/dev/null 2>&1
        fi

        # ... and volumes
        new_volumes="${TEST_TMP_DIR}/new-volumes.txt"
        comm -1 "${before_start_volumes}" "${after_end_volumes}" > "${new_volumes}"
        if [ "$( wc -l < "${new_volumes}" )" != 0 ] ; then
            "${CONTAINER_RUNNER}" volume rm $( cat "${new_volumes}" ) >/dev/null 2>&1
        fi
    fi
done
echo "=========================================="
echo "Summary"
echo "Run total: ${run}"
echo "Fail total: ${fail}"
exit "${fail}"
