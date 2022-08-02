FROM docker.io/library/alpine:3.10

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

ARG COMMANDS="chmod ln-s"

WORKDIR /opt/code

RUN \
       apk --no-cache update \
    && apk add build-base=0.5-r1 "bash=~5" "python3=~3.7" \
    && rm -rf /tmp/* /var/cache/apk/*

ENV \
#    DEBUG=1 \
    BUILD_MODE=$BUILD_MODE \
    COMMANDS=$COMMANDS \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

COPY build-tools/ build-tools/
COPY Makefile Makefile.command-flags version.txt ./
COPY src/ src/
COPY tests/ tests/

RUN    echo 'LIBNAME=musl' >> version.txt \
    && ./build-tools/internal-docker-make.sh
