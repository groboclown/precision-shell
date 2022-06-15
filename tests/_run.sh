#/bin/bash

# Make sure the tests are setup right.
test -d "${TEST_TMP_DIR}" || exit 99
test -x "${FS_SHELL}" || exit 99

RUNNER=/bin/bash
if [ "${DEBUG}" = "1" ] ; then
    RUNNER="${RUNNER} -x"
fi

get_abs_filename() {
  # $1 : relative filename
  filename=$1
  parentdir=$(dirname "${filename}")

  if [ -d "${filename}" ]; then
      echo "$(cd "${filename}" && pwd)"
  elif [ -d "${parentdir}" ]; then
    echo "$(cd "${parentdir}" && pwd)/$(basename "${filename}")"
  fi
}

myself="$( get_abs_filename $0 )"
here="$( dirname "${myself}" )"

export UID0=$( id -u )
export GID0=$( id -g )
export FS="$( get_abs_filename "${FS_SHELL}" )"

FAILED=0
for test_name in "$@" ; do
    test_script="$( get_abs_filename "${test_name}" )"
    export TEST_NAME="$( basename ${test_name} .sh )"

    # Don't run the helper "_" scripts.
    if [ -x "${test_script}" ] && [ "${TEST_NAME:0:1}" != "_" ] && [ "${TEST_NAME}" != README.md ] ; then
        # Set up per-test variables.
        export TEST_DIR="${TEST_TMP_DIR}/${TEST_NAME}.d"
        if [ "${QUIET}" != 1 ]; then
            echo "-------------------------------------------"
            echo ">> ${TEST_NAME}"
            cat "${test_script}" | sed -n 's/# desc: \(.*\)/\/\/ \1/p'
        fi
        # Check if the required version is being run.
        requirements="$( cat "${test_script}" | sed -n 's/# requires: \(.*\)/\1/p' )"
        # Note that this requires running the command version before it's tested...
        dorun=1
        if [ ! -z "${requirements}" ] ; then
            for req_name in ${requirements} ; do
                "${FS}" version | grep "${req_name}" >/dev/null 2>&1
                if [ $? -ne 0 ] ; then
                    if [ "${QUIET}" != 1 ]; then
                        echo "?? SKIPPED because ${FS} does not support ${requirements} (missing ${req_name})"
                    fi
                    dorun=0
                    break
                fi
            done
        fi
        if [ "${dorun}" == 1 ] ; then
            logs="${TEST_TMP_DIR}/run.log"
            ${RUNNER} "${here}/_before-each.sh" > "${logs}" 2>&1
            res=$?
            if [ ${res} -ne 0 ] ; then
                FAILED=$(( FAILED + 1 ))
                if [ "${QUIET}" = 1 ]; then
                    # Quiet mode, but report failures completely
                    echo ">> ${TEST_NAME}"
                fi
                if [ "${QUIET}" != 1 ]; then
                    cat "${logs}"
                fi
                echo "!! FAILED in before"
            else
                ( cd "${TEST_DIR}" && ${RUNNER} ${test_script} ) > "${logs}" 2>&1
                res=$?
                if [ ${res} -ne 0 ] ; then
                    FAILED=$(( FAILED + 1 ))
                    if [ "${QUIET}" = 1 ]; then
                        # Quiet mode, but report failures completely
                        echo ">> ${TEST_NAME}"
                    fi
                    cat "${logs}"
                    echo "!! FAILED"
                fi
            fi
            # Always run the after each, and ignore failures.
            ${RUNNER} "${here}/_after-each.sh" > "${logs}" 2>&1

        fi
        if [ ${res} = 0 ] && [ "${QUIET}" != 1 ]; then
            cat "${logs}"
        fi
        if [ "${QUIET}" != 1 ]; then
            echo ""
        fi
    fi
done
echo "${FS_SHELL} - ${FAILED} failure(s)"
exit ${FAILED}
