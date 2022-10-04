FROM docker.io/library/alpine:3.10

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apk --no-cache update \
    && apk add build-base=0.5-r1 curl tar xz python3 "bash=~5" \
    && mkdir -p /opt/dietlibc \
    && curl -o /tmp/dietlibc.tar.xz https://www.fefe.de/dietlibc/dietlibc-0.34.tar.xz \
    && xz -d /tmp/dietlibc.tar.xz \
    && tar xf /tmp/dietlibc.tar -C /opt/dietlibc/ --strip 1 \
    && ( cd /opt/dietlibc && make && install bin-x86_64/diet /usr/local/bin ) \
    && rm -rf /tmp/* /var/cache/apk/*

ENV CC="diet cc"

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
    UID1=405 \
    GID1=100 \
    UID2=65534 \
    GID2=65534

RUN    echo 'LIBNAME=dietlibc' >> version.txt \
    && ./build-tools/internal-docker-make.sh
