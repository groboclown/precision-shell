FROM --platform=linux/i386 docker.io/i386/alpine:3.19 AS iso

RUN set -x \
    # Basic required tools for ISO creation.
    && apk update \
    && apk add \
        alpine-sdk \
        alpine-conf \
        syslinux \
        xorriso \
        squashfs-tools \
        grub \
        doas \
    # EFI support.
        mtools dosfstools grub-efi \
    # precision-shell build support \
        build-base bash python3 \
    # copy/v86 requirements
        curl clang make openjdk8 npm \
    # iso tools
        xorriso \
    # extra tools
       patchelf nasm m4 byacc libuuid perl flex bc bash \
    # Set up the build user.
    && adduser build -G abuild -D \
    && mkdir -p /etc/doas.d \
    && printf 'permit nopass :abuild\n' > /etc/doas.d/doas.conf

COPY config-3.17.8 /tmp/
COPY isolinux.cfg /tmp/

RUN set -x \
    && mkdir -p /opt/work/CD_root \
    && mkdir -p /opt/work/kernel \
    && cd /opt/work/kernel \
    && curl https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.1.39.tar.xz | unxz | tar x \
    && cd linux-4.1.39 \
    && cp /tmp/config-3.17.8 .config \
    && make oldconfig ARCH=i386 \
    &&  make ARCH=i386 PATH=$CCBIN:$TOOLSBIN:$PATH \
    &&  ln arch/x86/boot/bzImage /opt/work/CD_root/bzImage \
