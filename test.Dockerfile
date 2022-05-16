FROM docker.io/library/alpine:3.10

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apk --no-cache update \
    && apk add "build-base=~0" "bash=~5" \
    && rm -rf /tmp/* /var/cache/apk/*

COPY src ./src
COPY Makefile .

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
#    && ls -lA /opt/code/out \
    && FS_SHELL=/opt/code/out/fs-shell tests/_all.sh \
    && FS_SHELL=/opt/code/out/fs-shell-signal tests/_all.sh \
    && FS_SHELL=/opt/code/out/fs-shell-input tests/_all.sh \
    && FS_SHELL=/opt/code/out/fs-shell-signal-input tests/_all.sh \
    && FS_SHELL=/opt/code/out/fs-shell-exec tests/_all.sh \
    && FS_SHELL=/opt/code/out/fs-shell-signal-exec tests/_all.sh \
    && FS_SHELL=/opt/code/out/fs-shell-input-exec tests/_all.sh \
    && FS_SHELL=/opt/code/out/fs-shell-signal-input-exec tests/_all.sh
