
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
ARG COMMANDS="elapsed-time-under export-elapsed-time expect-http-get-response expect-http-get-response-not sleep while-no-error subcmd spawn kill-pid wait-pid exit signal enviro echo"

ENV COMMANDS=$COMMANDS

RUN build-tools/build-with-alpine-musl.sh

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

ENV LISTEN_PORT 3000
ENV DEPENDENT_SERVICE dep9000

# Delay start until a dependent service is running, or up to 5 minutes (300 seconds)
# This service must be accessible as a sidecar container running on localhost
#   port 9000.
# The service start is put into a "subcmd" to add in signal handling, which is explained
#   in the example recipe "signal-awareness.Dockerfile".  Here, it's necessary to put
#   into a subcmd so that it only runs when the child service is accessible.
ENTRYPOINT \
    start-timer \
    && while-no-error \
      [ \
        elapsed-time-under 300 \
        && expect-http-get-response-not ${DEPENDENT_SERVICE} 9000 / 200 \
        && export-elapsed-time DURATION \
        && echo [Waited ${DURATION} seconds for ${DEPENDENT_SERVICE}:9000 to come alive] \
      ] \
      [sleep 2] \
    ; expect-http-get-response ${DEPENDENT_SERVICE} 9000 / 200 \
        && subcmd [ \
          spawn [/nodejs/bin/node server.js] NODE \
          && signal 1 2 9 15 17 wait \
          && echo [\nTerminating the server...] \
          && kill-pid 15 ${NODE} \
          && wait-pid ${NODE} *EXIT \
          && echo [Terminated.] \
          && exit ${EXIT} \
        ]

# To test this out:

#  $ docker build -t presh/delay-until-ready -f recipes/delay-until-ready.Dockerfile .
#  $ docker build -t presh/env-config-file -f recipes/env-config-file.Dockerfile .
#  $ docker network create delay-net
#  $ docker run --rm -d -p 3000:3000 --network delay-net presh/delay-until-ready
#  (the server is waiting for the dependent server to start, so requests will fail:)
#  (other shell:
#      $ curl http://127.0.0.1:3000
#      curl: (56) Recv failure: Connection reset by peer
#  )
#  $ docker run --rm -d -p 9000:9000 --network delay-net --name dep9000 presh/env-config-file
#  (now the first server will start accepting commands.)
#  (other shell:
#    $ curl http://127.0.0.1:3000
#    Hello World
#    $ curl http://127.0.0.1:3000/halt
#    Goodbye World
#    $ curl http://127.0.0.1:9000/halt
#    Goodbye World
#  )
#  (docker containers should stop)
#  $ docker network rm delay-net
