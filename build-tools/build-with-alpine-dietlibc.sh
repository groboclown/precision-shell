#!/bin/sh
set -e
set -x

# Build the software using Alpine + dietlibc library.

cd "$( dirname "$0" )/.."

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
