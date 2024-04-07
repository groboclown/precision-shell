FROM docker.io/library/alpine:3.19
FROM docker.io/library/alpine:3.19

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN set -x \
    && apk --no-cache add build-base=0.5-r3 "bash=~5" "python3=~3.11"

# Change the list of commands to build with the "--build-arg COMMANDS='list' argument"
ARG BUILD_MODE=build
ARG COMMANDS="chmod ln-s"
ARG IPV6=""

ENV \
#    DEBUG=1 \
    BUILD_MODE=$BUILD_MODE \
    COMMANDS=$COMMANDS \
    IPV6=$IPV6 \
    VIRTUAL_NETWORK=yes \
    UID1=405 \
    GID1=100 \
    UID2=65534 \
    GID2=65534

COPY build-tools/ build-tools/
COPY \
    Makefile \
    Makefile.command-flags \
    Makefile.compiler-flags \
    version.txt \
    ./
COPY src/ src/
COPY tests/ tests/

RUN    echo 'LIBNAME=musl' >> version.txt \
    && ./build-tools/internal-docker-make.sh
