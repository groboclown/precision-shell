FROM docker.io/library/archlinux:base-devel

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

# Arch has rolling updates, so specific version numbers on
#   installed packages, and even on the label, aren't stable.
RUN \
       pacman -Sy --noconfirm python3 \
    && rm -rf /tmp/* /var/cache/pacman/*

COPY build-tools/ build-tools/
COPY Makefile Makefile.command-flags version.txt ./
COPY src/ src/
COPY tests/ tests/

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
