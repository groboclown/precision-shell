#!/bin/bash
set -e

test -d "${TEST_DIR}" && rm -rf "${TEST_DIR}" || true
mkdir -p "${TEST_DIR}"
