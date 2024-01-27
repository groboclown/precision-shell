FROM docker.io/library/ubuntu:22.04

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apt-get update \
    && apt-get -y install build-essential python3.10-minimal \
    && ln -s /usr/bin/python3.10 /usr/bin/python3 \
    && rm -rf /tmp/* /var/cache/apt/*

COPY build-tools/ build-tools/
COPY \
    Makefile \
    Makefile.command-flags \
    Makefile.compiler-flags \
    version.txt \
    ./
COPY src/ src/
COPY tests/ tests/

# Change the list of commands to build with the "--build-arg COMMANDS='list' argument"
ARG BUILD_MODE=build
ARG COMMANDS="chmod ln-s"

ENV \
#    DEBUG=1 \
    BUILD_MODE=$BUILD_MODE \
    COMMANDS=$COMMANDS \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN    echo 'LIBNAME=glibc' >> version.txt \
    && ./build-tools/internal-docker-make.sh
