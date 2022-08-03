#!/bin/sh
set -e
set -x

# Build the software using Alpine + Musl library.

cd "$( dirname "$0" )/.."

apk --no-cache update
apk add build-base=0.5-r1 "bash=~5" "python3=~3.7"
rm -rf /tmp/* /var/cache/apk/*
echo 'LIBNAME=musl' >> version.txt
exec ./build-tools/internal-docker-make.sh
