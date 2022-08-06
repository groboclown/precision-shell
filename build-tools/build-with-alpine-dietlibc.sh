#!/bin/sh
set -e
set -x

# Build the software using Alpine + dietlibc library.

cd "$( dirname "$0" )/.."

BUILD_MODE="${BUILD_MODE:-build}"
UID1="${UID1:-1}"
GID1="${UID1:-1}"
UID2="${UID1:-2}"
GID2="${UID1:-2}"
export BUILD_MODE UID1 GID1 UID2 GID2

apk --no-cache update
apk add build-base=0.5-r1 curl tar xz python3 "bash=~5"
mkdir -p /tmp/dietlibc
curl -o /tmp/dietlibc.tar.xz https://www.fefe.de/dietlibc/dietlibc-0.34.tar.xz
xz -d /tmp/dietlibc.tar.xz
tar xf /tmp/dietlibc.tar -C /tmp/dietlibc/ --strip 1
( cd /tmp/dietlibc && make && install bin-x86_64/diet /usr/local/bin )
rm -rf /tmp/* /var/cache/apk/*

export CC="diet cc"

echo 'LIBNAME=dietlibc' >> version.txt
exec ./build-tools/internal-docker-make.sh
