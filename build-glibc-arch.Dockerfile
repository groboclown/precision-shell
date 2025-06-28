FROM public.ecr.aws/docker/library/archlinux:base-devel

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

# Arch has rolling updates, so specific version numbers on
#   installed packages, and even on the label, aren't stable.
RUN set -x \
    && pacman -Sy --noconfirm python3 xxd \
    && rm -rf /tmp/* /var/cache/pacman/*

COPY build-tools/ build-tools/
COPY \
    Makefile \
    Makefile.command-flags \
    Makefile.compiler-flags \
    version.txt \
    ./
COPY src/ src/
COPY tests/ tests/
COPY compressed/ compressed/

# Change the list of commands to build with the "--build-arg COMMANDS='list' argument"
ARG BUILD_MODE=build
ARG COMMANDS="chmod ln-s"
ARG IPV6=""

ENV \
#    DEBUG=1 \
    BUILD_MODE=$BUILD_MODE \
    NO_GETADDRINFO=1 \
    COMMANDS=$COMMANDS \
    IPV6=$IPV6 \
    VIRTUAL_NETWORK=yes \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN    echo 'LIBNAME=glibc' >> version.txt \
    && ./build-tools/internal-docker-make.sh
