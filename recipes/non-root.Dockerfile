
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

COPY build-tools/ build-tools/
COPY Makefile Makefile.command-flags version.txt ./
COPY src/ src/
COPY tests/ tests/


# Adjust this value during the image build with `--build-arg`
#   to alter which commands to include.
#   Adding in "ls" and "file-stat" for exploring permissions.
ARG COMMANDS="su-spawn kill-pid wait-pid signal chown exit enviro noop ls file-stat echo"

ENV COMMANDS=$COMMANDS

RUN build-tools/build-with-alpine-musl.sh

# ---------------------------------------------------------------------------
# The real image, using what was just built.
FROM gcr.io/distroless/nodejs:18
LABEL name="local/precision-shell-example"

COPY --from=build-env /opt/app /opt/app

# Set up the pre-built configuration file
COPY recipes/support-files/config-complete.json /opt/app/hello_world/config.json

# To debug the presh script, this can use the presh-debug instead.
# COPY --from=presh-builder /opt/precision-shell/out/presh-debug /bin/sh
COPY --from=presh-builder /opt/precision-shell/out/presh /bin/sh

WORKDIR /opt/app/hello_world

ENV LISTEN_PORT 9000

VOLUME /opt/logs

# Because Node doesn't install signal handlers on its own,
#   have presh listen for user signals.
# This allows for the container to stop with Ctrl-C when
#   the user runs the container with "-it" arguments.
ENTRYPOINT \
    noop [The image has user "nonroot" set to uid 65532] \
      [and group "nonroot" set to gid 65532] \
      [In this example, the /opt/logs directory has the wrong permissions] \
      [because of the VOLUME command above.] \
    && chown 65532 65532 /opt/logs \
    && noop [Debug the changed ownership] \
    && file-stat /opt/logs \
    && noop [Launch the server as the non-root user.] \
    && su-spawn 65532 65532 [/nodejs/bin/node server.js] NODE \
    && noop [Wait for an OS terminate signal] \
    && signal 1 2 9 15 wait && \
    && noop [Kill the server and wait for it to end.] \
    && echo [\nTerminating the server...] \
    && kill-pid 15 ${NODE} \
    && wait-pid ${NODE} *EXIT \
    && echo [Terminated.] \
    && exit ${EXIT}
