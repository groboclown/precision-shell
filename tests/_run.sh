#/bin/bash

# Make sure we're in the right directory.
test -x _before-each.sh || exit 99
test -x _after-each.sh || exit 99

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
        echo "-------------------------------------------"
        echo "${TEST_NAME}"
        cat "${test_script}" | sed -n 's/# desc: \(.*\)/>> \1/p'
        
        ${RUNNER} ./_before-each.sh
        if [ $? -ne 0 ] ; then
            FAILED=$(( FAILED + 1 ))
        else
            ( cd "${TEST_DIR}" && ${RUNNER} ${test_script} )
            if [ $? -ne 0 ] ; then
                FAILED=$(( FAILED + 1 ))
            fi
        fi
        # Always run the after each, and ignore failures.
        ${RUNNER} ./_after-each.sh
        echo ""
    fi
done
exit ${FAILED}
