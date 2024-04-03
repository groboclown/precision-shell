FROM docker.io/library/alpine:3.19

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN set -x \
    && apk --no-cache add \
        build-base=0.5-r3 curl tar xz "bash=~5" "python3=~3.11" \
    && mkdir -p /opt/dietlibc \
    && curl -o /tmp/dietlibc.tar.xz https://www.fefe.de/dietlibc/dietlibc-0.34.tar.xz \
    && xz -d /tmp/dietlibc.tar.xz \
    && tar xf /tmp/dietlibc.tar -C /opt/dietlibc/ --strip 1 \
    && rm /tmp/dietlibc.tar \
    && ( cd /opt/dietlibc && make && install bin-x86_64/diet /usr/local/bin )

ENV CC="diet cc"

COPY build-tools/ build-tools/
COPY \
    Makefile \
    Makefile.command-flags \
    Makefile.compiler-flags \
    version.txt \
    ./
COPY src/ src/
COPY tests/ tests/

# Change the list of commands to build with the "--build-arg COMMANDS='list' argument"
ARG BUILD_MODE=build
ARG COMMANDS="chmod ln-s"
ARG IPV6=""

ENV \
#    DEBUG=1 \
    BUILD_MODE=$BUILD_MODE \
    COMMANDS=$COMMANDS \
    IPV6=$IPV6 \
    VIRTUAL_NETWORK=yes \
    UID1=405 \
    GID1=100 \
    UID2=65534 \
    GID2=65534

RUN    echo 'LIBNAME=dietlibc' >> version.txt \
    && ./build-tools/internal-docker-make.sh
