#!/bin/sh
set -e
set -x

# Build the software using Alpine + Musl library.

cd "$( dirname "$0" )/.."

BUILD_MODE="${BUILD_MODE:-build}"

# guest
UID1="${UID1:-405}"
GID1="${GID1:-100}"

# nobody
UID2="${UID2:-65534}"
GID2="${GID2:-65534}"

export BUILD_MODE UID1 GID1 UID2 GID2

# Explicitly not installing python.
# Add it if you want to rebuild the generated commands.
apk --no-cache add "build-base=~0.5" "clang-dev=~20" "bash=~5" xz zstd
echo 'LIBNAME=musl' >> version.txt

# This can build with LLVM, which can make smaller executables.
# Set CC=clang in the ENV to do so.
if [ "${CC}" = "clang" ] ; then
    LIBRARY_PATH=/usr/bin/lld.ld
    export LIBRARY_PATH
fi

exec ./build-tools/internal-docker-make.sh
