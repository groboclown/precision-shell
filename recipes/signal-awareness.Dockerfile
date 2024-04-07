
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
ARG COMMANDS="spawn kill-pid wait-pid exit signal echo noop enviro"
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

# Set up the pre-built configuration file
COPY recipes/support-files/config-complete.json /opt/app/hello_world/config.json

# To debug the presh script, this can use the presh-debug instead.
# COPY --from=presh-builder /opt/precision-shell/out/presh-debug /bin/sh
COPY --from=presh-builder /opt/precision-shell/out/presh /bin/sh

WORKDIR /opt/app/hello_world

ARG LISTEN_PORT=3000

ENV \
    LISTEN_PORT=$LISTEN_PORT

# Because Node doesn't install signal handlers on its own,
#   have presh listen for user signals.
# This allows for the container to stop with Ctrl-C when
#   the user runs the container with "-it" arguments.
# This also has the signal listener wait for signal 17, which is SIGCHLD.
#   This means that, if the server stops on its own (say, a bug or an
#   explicit, internal stop), then the shell will notice that.  Otherwise,
#   the shell will not stop when the server stops.
# If you only run the service in non-interactive mode, then none
#   of this is necessary.
ENTRYPOINT \
    noop [Launch the server] \
    && spawn [/nodejs/bin/node server.js] NODE \
    && noop [Wait for an OS terminate signal] \
    && signal 1 2 9 15 17 wait \
    && noop [Kill the server and wait for it to end.] \
    && echo [\nTerminating the server...] \
    && kill-pid 15 ${NODE} \
    && wait-pid ${NODE} *EXIT \
    && echo [Terminated.] \
    && exit ${EXIT}
