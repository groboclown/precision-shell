FROM docker.io/library/alpine:3.10

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apk --no-cache update \
    && apk add build-base=0.5-r1 "bash=~5" \
    && rm -rf /tmp/* /var/cache/apk/*

COPY Makefile version.txt .
COPY src/ src/
COPY tests/ tests/

ENV \
#    DEBUG=1 \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN    echo 'LIBNAME=musl' >> version.txt \
    && make
