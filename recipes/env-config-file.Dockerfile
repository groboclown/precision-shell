
# ---------------------------------------------------------------------------
# Build the software.
FROM node:20 AS build-env

WORKDIR /opt/app
COPY recipes/support-files/ ./

WORKDIR /opt/app/hello_world
RUN npm ci --omit=dev


# ---------------------------------------------------------------------------
# Create precision shell
FROM docker.io/library/alpine:3.19 AS presh-builder

WORKDIR /opt/precision-shell

COPY build-tools/ build-tools/
COPY Makefile Makefile.command-flags Makefile.compiler-flags version.txt ./
COPY src/ src/
COPY tests/ tests/


# Adjust this value during the image build with `--build-arg`
#   to alter which commands to include.
ARG COMMANDS="dup-w env-cat-fd cat-fd spawn kill-pid wait-pid exit signal echo subcmd noop enviro"
ARG IPV6=""

ENV \
    COMMANDS=$COMMANDS \
    IPV6=$IPV6

RUN set -x \
    && build-tools/build-with-alpine-musl.sh \
    && ls -l /opt/precision-shell/out/presh

# ---------------------------------------------------------------------------
# The real image, using what was just built.
FROM gcr.io/distroless/nodejs20-debian12
LABEL name="local/precision-shell-example"

COPY --from=build-env /opt/app /opt/app

# To debug the presh script, this can use the presh-debug instead.
# COPY --from=presh-builder /opt/precision-shell/out/presh-debug /bin/sh
COPY --from=presh-builder /opt/precision-shell/out/presh /bin/sh

WORKDIR /opt/app/hello_world

ARG LISTEN_PORT="9000"

ENV \
    LISTEN_PORT=$LISTEN_PORT

# See the "signal-awareness.Dockerfile" to see why the
#   spawn / signal trapping is added.  All of that is added in the "subcmd";
#   which isn't necessary, but added here for clairty about the separation
#   of example code.
ENTRYPOINT \
    noop [Use dup-w and env-cat-fd to update config.json] \
       [based on environment variables.] \
    && dup-w 8 config.json \
    && env-cat-fd 8 ../config.json.template \
    && dup-w 8 /tmp/closed.txt \
    && noop [Show the config to stdout for testing purposes] \
    && noop [cat-fd 1 config.json] \
    && subcmd [ \
      && noop [Launch the server] \
      && spawn [/nodejs/bin/node server.js] NODE \
      && noop [Wait for an OS terminate signal] \
      && signal 1 2 9 15 17 wait \
      && noop [Kill the server and wait for it to end.] \
      && echo [\nTerminating the server...] \
      && kill-pid 15 ${NODE} \
      && wait-pid ${NODE} *EXIT \
      && echo [Terminated.] \
      && exit ${EXIT} \
    ]
