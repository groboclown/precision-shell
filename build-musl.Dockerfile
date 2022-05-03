FROM docker.io/library/alpine:3.10

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apk --no-cache update \
    && apk add build-base=0.5-r1 \
    && rm -rf /tmp/* /var/cache/apk/*

COPY *.c *.h Makefile ./

RUN make
