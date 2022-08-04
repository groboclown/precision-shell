
# ---------------------------------------------------------------------------
# Build the software.
FROM node:18 AS build-env

WORKDIR /opt/app
COPY recipes/support-files/ ./

WORKDIR /opt/app/hello_world
RUN npm ci --omit=dev


# ---------------------------------------------------------------------------
# Create precision shell
FROM docker.io/library/alpine:3.10 AS presh-builder

WORKDIR /opt/precision-shell

# Adjust this value during the image build with `--build-arg`
#   to alter which commands to include.
ARG COMMANDS="dup-w env-cat-fd cat-fd spawn kill-pid wait-pid exec echo signal noop enviro"

ENV COMMANDS=$COMMANDS

COPY build-tools/ build-tools/
COPY Makefile Makefile.command-flags version.txt ./
COPY src/ src/
COPY tests/ tests/

RUN build-tools/build-with-alpine-musl.sh

# ---------------------------------------------------------------------------
# The real image, using what was just built.
FROM gcr.io/distroless/nodejs:18
LABEL name="local/precision-shell-example"

COPY --from=build-env /opt/app /opt/app

# To debug the presh script, this can use the presh-debug instead.
# COPY --from=presh-builder /opt/precision-shell/out/presh-debug /bin/sh
COPY --from=presh-builder /opt/precision-shell/out/presh /bin/sh

WORKDIR /opt/app/hello_world

ENV LISTEN_PORT 9000

# Because Node doesn't install signal handlers on its own,
#   have presh listen for user signals.
# This allows for the container to stop with Ctrl-C when
#   the user runs the container with "-it" arguments.
ENTRYPOINT \
    noop [Use dup-w and env-cat-fd to update config.json] \
       [based on environment variables.] \
    && dup-w 8 config.json \
    && env-cat-fd 8 ../config.json.template \
    && dup-w 8 /tmp/closed.txt \
    && noop [Show the config to stdout for testing purposes] \
    && noop [cat-fd 1 config.json] \
    && noop [Launch the server] \
    && spawn [/nodejs/bin/node server.js] NODE \
    && noop [Wait for an OS terminate signal] \
    && signal 1 2 9 15 wait && \
    && noop [Kill the server and wait for it to end.] \
    && echo [\nTerminating the server...] \
    && kill-pid 15 ${NODE} \
    && wait-pid ${NODE} \
    && echo [Terminated.]
