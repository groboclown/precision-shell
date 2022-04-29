FROM docker.io/library/alpine:3.10

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apk --no-cache update \
    && apk add build-base=0.5-r1 bash=5.0.0-r0 \
    && rm -rf /tmp/* /var/cache/apk/*

COPY *.c Makefile ./

RUN make

ENV \
#    DEBUG=1 \
    TEST_TMP_DIR=/tmp/tests \
    FS_SHELL=/opt/code/fs-shell \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

COPY tests/ tests/
RUN mkdir -p "${TEST_TMP_DIR}" && tests/_all.sh
