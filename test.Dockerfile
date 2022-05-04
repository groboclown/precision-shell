FROM docker.io/library/alpine:3.10

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apk --no-cache update \
    && apk add "build-base=~0" "bash=~5" perl \
    && rm -rf /tmp/* /var/cache/apk/*

COPY *.c *.h Makefile ./

RUN make

ENV \
#    DEBUG=1 \
    TEST_TMP_DIR=/tmp/tests \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

COPY tests/ tests/
RUN \
    mkdir -p "${TEST_TMP_DIR}" \
    && FS_SHELL=/opt/code/fs-shell.o tests/_all.sh \
    && FS_SHELL=/opt/code/fs-shell-signal.o tests/_all.sh \
    && FS_SHELL=/opt/code/fs-shell-input.o tests/_all.sh \
    && FS_SHELL=/opt/code/fs-shell-signal-input.o tests/_all.sh
