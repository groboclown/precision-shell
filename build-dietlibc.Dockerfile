FROM public.ecr.aws/docker/library/alpine:3.22

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

ENV DIETLIBC_VERSION=0.35

# As of May 26, 2025, the dietlibc certificate expired.
# Until this changes, this build requires the "--insecure" option.

RUN set -x \
    && apk --no-cache add \
        build-base=0.5-r3 curl tar xz linux-headers "bash=~5" "python3=~3.12" \
    && mkdir -p /opt/dietlibc \
    && curl --insecure -o /tmp/dietlibc.tar.xz https://www.fefe.de/dietlibc/dietlibc-${DIETLIBC_VERSION}.tar.xz \
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
COPY compressed/ compressed/

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
