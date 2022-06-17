FROM docker.io/library/ubuntu:22.04

ARG BUILD_MODE=build
ARG COMMANDS="chmod ln-s"

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apt-get update \
    && apt-get -y install "build-essential=12.9ubuntu3" python3 \
    && rm -rf /tmp/* /var/cache/apt/*

COPY experiments/ experiments/
COPY Makefile Makefile.command-flags version.txt internal-docker-make.sh generate-command-template.py ./
COPY src/ src/
COPY tests/ tests/

ENV \
#    DEBUG=1 \
    BUILD_MODE=$BUILD_MODE \
    COMMANDS=$COMMANDS \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN    echo 'LIBNAME=glibc' >> version.txt \
    && ./internal-docker-make.sh
