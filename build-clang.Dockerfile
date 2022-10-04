FROM docker.io/library/ubuntu:22.04

# This file is broken up to make rebuilds fast
# by reusing previous layers that take a while to run.

WORKDIR /opt/code

# Installation environment variables.
ENV \
    GPG_KEYS="09C4E7007CB2EFFB A2C794A986419D8A B4468DF4E95C63DC D23DD2C20DD88BA2 8F0871F202119294 0FC3042E345AD05D D574BD5D1D0E98895E3BF90044F2485E45D59042" \
    LLVM_RELEASE=15 \
    LLVM_VERSION=15.0.1

# This builds clang and llvm.  This can take a very long time.
RUN \
       apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        autoconf automake cmake dpkg-dev file gcc g++ make patch python3 \
        libc-dev libc++-dev libgcc-10-dev libstdc++-10-dev  \
        dirmngr gnupg2 lbzip2 wget xz-utils libtinfo5 \
    && gpg --batch --keyserver keyserver.ubuntu.com --recv-keys ${GPG_KEYS} \
    && wget -O /tmp/llvm.src.tar.xz "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/llvm-${LLVM_VERSION}.src.tar.xz" \
    && wget -O /tmp/llvm.src.tar.xz.sig "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/llvm-${LLVM_VERSION}.src.tar.xz.sig" \
    && gpg --batch --verify "/tmp/llvm.src.tar.xz.sig" "/tmp/llvm.src.tar.xz" \
    && wget -O /tmp/cmake.src.tar.xz "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/cmake-${LLVM_VERSION}.src.tar.xz" \
    && wget -O /tmp/cmake.src.tar.xz.sig "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/cmake-${LLVM_VERSION}.src.tar.xz.sig" \
    && gpg --batch --verify "/tmp/cmake.src.tar.xz.sig" "/tmp/cmake.src.tar.xz" \
    && wget -O /tmp/clang.src.tar.xz "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/clang-${LLVM_VERSION}.src.tar.xz" \
    && wget -O /tmp/clang.src.tar.xz.sig "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/clang-${LLVM_VERSION}.src.tar.xz.sig" \
    && gpg --batch --verify "/tmp/clang.src.tar.xz.sig" "/tmp/clang.src.tar.xz" \
    && mkdir -p /tmp/build/llvm \
    && tar --strip-components=1 -C /tmp/build/llvm -Jxf /tmp/llvm.src.tar.xz \
    && mkdir -p /tmp/build/cmake \
    && tar --strip-components=1 -C /tmp/build/cmake -Jxf /tmp/cmake.src.tar.xz \
    && mkdir -p /tmp/build/clang \
    && tar --strip-components=1 -C /tmp/build/clang -Jxf /tmp/clang.src.tar.xz

# Split here so that compile can be its own layer, to make all that above stuff
# not need to run again while trying to get the compile to pass.  If this
# is solid, then join it with the run section above.
RUN \
    mkdir -p /tmp/build/third-party/benchmark \
    && touch /tmp/build/third-party/benchmark/CMakeLists.txt \
    && ( cd /tmp/build && cmake -S llvm -B build-llvm -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_PREFIX_PATH=/tmp/build ) \
    && ( cd /tmp/build/build-llvm && make -j3 && make install ) \
    && ( cd /tmp/build && cmake -S clang -B build-clang -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX=/usr/local ) \
    && ( cd /tmp/build/build-clang && make -j3 && make install ) \
    && rm -rf /var/lib/apt/lists/* /tmp/*


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

ENV \
#    DEBUG=1 \
    CC=/usr/local/bin/clang \
    BUILD_MODE=$BUILD_MODE \
    COMMANDS=$COMMANDS \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN    echo 'LIBNAME=llvm' >> version.txt \
#    && ./build-tools/internal-docker-make.sh
