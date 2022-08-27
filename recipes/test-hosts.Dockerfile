
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
ARG COMMANDS="cat-fd"

ENV COMMANDS=$COMMANDS

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

# See the "signal-awareness.Dockerfile" to see why the
#   spawn / signal trapping is added.  All of that is added in the "subcmd";
#   which isn't necessary, but added here for clairty about the separation
#   of example code.
ENTRYPOINT \
    cat-fd 1 /etc/hosts \
    ; cat-fd 1 /etc/resolv.conf
