#!/bin/sh
set -e
set -x

# Build the software using Alpine + Musl library.

cd "$( dirname "$0" )/.."

BUILD_MODE="${BUILD_MODE:-build}"
UID1="${UID1:-1}"
GID1="${UID1:-1}"
UID2="${UID1:-2}"
GID2="${UID1:-2}"
export BUILD_MODE UID1 GID1 UID2 GID2

apk --no-cache update
apk add build-base=0.5-r1 "bash=~5" "python3=~3.7"
rm -rf /tmp/* /var/cache/apk/*
echo 'LIBNAME=musl' >> version.txt
exec ./build-tools/internal-docker-make.sh
