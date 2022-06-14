FROM docker.io/library/alpine:3.10

ARG BUILD_MODE=build
ARG COMMANDS="chmod ln-s"

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apk --no-cache update \
    && apk add build-base=0.5-r1 curl tar xz "bash=~5" \
    && mkdir -p /opt/dietlibc \
    && curl -o /tmp/dietlibc.tar.xz https://www.fefe.de/dietlibc/dietlibc-0.34.tar.xz \
    && xz -d /tmp/dietlibc.tar.xz \
    && tar xf /tmp/dietlibc.tar -C /opt/dietlibc/ --strip 1 \
    && ( cd /opt/dietlibc && make && install bin-x86_64/diet /usr/local/bin ) \
    && rm -rf /tmp/* /var/cache/apk/*

ENV CC="diet cc"

COPY experiments/ experiments/
COPY Makefile Makefile.command-flags version.txt internal-docker-make.sh ./
COPY src/ src/
COPY tests/ tests/

ENV \
    BROKEN_DATA_POINTERS=1 \
#    DEBUG=1 \
    BUILD_MODE=$BUILD_MODE \
    COMMANDS=$COMMANDS \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN    echo 'LIBNAME=dietlibc' >> version.txt \
    && ./internal-docker-make.sh \
