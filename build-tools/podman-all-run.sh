#!/bin/bash

here="$( cd "$( dirname "$0" )" && pwd )" || exit 1
help=0
err=0
parallel=1
network=host
logdir="${here}/../out/logs"
for arg in "$@" ; do
  case "${arg}" in
    --help)
      help=1
      ;;
    -h)
      help=1
      ;;
    --network=*)
      network="${arg:10}"
      ;;
    --logdir=*)
      logdir="${arg:9}"
      ;;
    --serial)
      parallel=0
      ;;
    *)
      echo "Unknown argument '${arg}'"
      err=1
      help=1
      ;;
  esac
done

if [ "${help}" = "1" ] ; then
  echo "Run all the supported builds from the GitHub workflow."
  echo "This script was written to support using podman."
  echo "It runs all the build files in parallel."
  echo ""
  echo "Usage: $0 [--network=NETWORK_NAME] [--logdir=LOGDIR_NAME] [--help]"
  echo ""
  echo "Where:"
  echo "  --help | -h               This screen."
  echo "  --network=NETWORK_NAME    Run podman connected to the NETWORK_NAME."
  echo "                            By default, this uses 'host'."
  echo "  --logdir=LOGDIR_NAME      Place all log files in the directory."
  echo "                            By default, this uses the root 'out/logs' directory."
  echo "  --serial                  Run each dockerfile type serially."
  echo ""
  echo "Example:"
  echo "  Using an IPv6 network:"
  echo "    podman network create --ipv6 --subnet=fd00:0bca:85f3:17a0::/64 netv6"
  echo "    $0 --network=netv6"
fi

if [ "${err}" != "0" ] || [ "${help}" = "1" ] ; then
  exit "${err}"
fi


mkdir -p "${logdir}" || exit 1
echo "Sending logs to ${logdir}"

jobs=()

function runOne {
  local file="$1"
  local tag="$2"
  local arg="$3"
  local log="$4"

  (
    podman build --network "${network}" -f "${file}" -t "${tag}" --build-arg "${arg}" . ;
    echo "Exit code: $?" ;
  ) >"${log}" 2>&1

}

function db {
  local arch="$1"
  local file="build-${arch}.Dockerfile"
  local tag="${arch}:offline"

  podman rmi "${tag}-1" >/dev/null 2>&1 || true
  podman rmi "${tag}-2" >/dev/null 2>&1 || true
  podman rmi "${tag}-3" >/dev/null 2>&1 || true

  if [ "${parallel}" = 1 ] ; then
    runOne "${file}" "${tag}-1" COMMANDS=ALL "${logdir}/${arch}-build.log" &
    jobs+=($!)
    
    runOne "${file}" "${tag}-2" BUILD_MODE=individual-cmd-test "${logdir}/${arch}-test.log" &
    jobs+=($!)

    runOne "${file}" "${tag}-2" BUILD_MODE=cmd-size "${logdir}/${arch}-size.log" &
    jobs+=($!)
  else
    (
      runOne "${file}" "${tag}-1" COMMANDS=ALL "${logdir}/${arch}-build.log"
      runOne "${file}" "${tag}-2" BUILD_MODE=individual-cmd-test "${logdir}/${arch}-test.log"
      runOne "${file}" "${tag}-2" BUILD_MODE=cmd-size "${logdir}/${arch}-size.log"
    ) &
    jobs+=($!)
  fi
}

db glibc
db glibc-arch
db clang
db musl
db dietlibc

if [ "${#jobs[@]}" -gt 0 ] ; then
  echo "Waiting on processes: ${jobs[*]}"
  wait "${jobs[@]}"
fi

## podman build -t local/precision-shell-sample --build-arg FLAGS=INCLUDE_ALL_COMMANDS=1 . || exit 1
## podman run --rm -t local/precision-shell-sample version || exit 1
# CONTAINER_RUNNER=podman recipes/_build_all.sh || exit 1
# CONTAINER_RUNNER=podman recipes/tests/run_all.sh || exit 1
