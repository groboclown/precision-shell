FROM public.ecr.aws/docker/library/alpine:3.22

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN set -x \
    && apk --no-cache add \
        "python3=~3.12" "bash=~5.2" \
        "make=~4.4" "clang=~20.1" "clang-dev=~20.1" "lld=~20.1" \
        "musl-dev=~1.2" "gcc=~14"

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
    CC=clang \
    LIBRARY_PATH=/usr/bin/lld.ld \
    BUILD_MODE=$BUILD_MODE \
    COMMANDS=$COMMANDS \
    IPV6=$IPV6 \
    VIRTUAL_NETWORK=yes \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN    echo 'LIBNAME=llvm' >> version.txt \
    && ./build-tools/internal-docker-make.sh
