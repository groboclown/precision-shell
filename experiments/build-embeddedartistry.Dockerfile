FROM docker.io/library/ubuntu:22.04

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

RUN \
       apt-get update \
    && apt-get -y install "build-essential=12.9ubuntu3" "meson=0.61.2-1" "git=1:2.34.1-1ubuntu1.2" \
    && git clone --depth 1 --branch 1.1.39 --recursive https://github.com/embeddedartistry/libc.git /opt/libc \
    && ( cd /opt/libc && make ) \
    && rm -rf /tmp/* /var/cache/apt/*

COPY Makefile version.txt .
COPY src/ src/
COPY tests/ tests/

ENV \
#    DEBUG=1 \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN    echo 'LIBNAME=embeddedartistry' >> version.txt \
    && make LFLAGS="\
        -static -nostdinc -Wunknown-pragmas \
        -L/opt/libc/buildresults/src/libc.a -lc \
        -I/opt/libc/include \
        -I/opt/libc/arch/$(uname -m)/include \
        -I/opt/libc/printf/src \
        " \
    && ls -lA out/fs-shell*
