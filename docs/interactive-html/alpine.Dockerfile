# Create an Alpine ISO image to allow interaction with the tool.
#   This was taken from the guide:
#       https://wiki.alpinelinux.org/wiki/How_to_make_a_custom_ISO_image_with_mkimage
# Build:
#   docker build -f docs/interactive-html/alpine.Dockerfile .
FROM --platform=linux/i386 docker.io/i386/alpine:3.19 AS iso

ENV RELEASE v3.19

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
    # Set up the build user.
    && adduser build -G abuild -D \
    && mkdir -p /etc/doas.d \
    && printf 'permit nopass :abuild\n' > /etc/doas.d/doas.conf

USER build

ENV PROFILENAME=html
WORKDIR /home/build

RUN set -x \
    # Create signing keys, and use defaults.
    && abuild-keygen -i -a -n \
    # Download the scripts and APK sources.
    && git clone --depth=1 https://gitlab.alpinelinux.org/alpine/aports.git

# Bring over precision shell tools and custom files.
COPY --chown=build:abuild build-tools/ precision-shell/build-tools/
COPY --chown=build:abuild recipes/ precision-shell/recipes/
COPY --chown=build:abuild src/ precision-shell/src/
COPY --chown=build:abuild tests/ precision-shell/tests/
COPY --chown=build:abuild \
        *.Dockerfile \
        *.md \
        Makefile* \
        version.txt \
        LICENSE \
    precision-shell/

# Build precision shell
#   Select various combinations of flags to pre-build.
RUN set -x \
    && cd precision-shell \
    # For the moment, ignore errors.
    && ( BUILD_MODE=combos build-tools/internal-docker-make.sh || : ) \
    # Clean out test results; they aren't needed.
    && doas rm -rf out/tests

COPY --chown=build:abuild docs/interactive-html/ interactive-html/

# Build the ISO image.
# It is placed into alpine-html-v3.19-x86.iso
RUN set -x \
    && cp interactive-html/mkimg.html.sh aports/scripts/ \
    && chmod +x aports/scripts/mkimg.html.sh \
    && mkdir -p iso \
    && sh aports/scripts/mkimage.sh \
        --arch x86 \
        --tag "${RELEASE}" \
        --hostkeys \
        --repository "https://dl-cdn.alpinelinux.org/alpine/${RELEASE}/main" \
        --profile "${PROFILENAME}" \
    && mkdir -p iso-files \
    && xorriso -osirrox on -indev alpine-html-v3.19-x86.iso -extract / iso-files \
    && ( cd iso-files && tar cf ../alpine-html-v3.19-x86.tar * )


# ---------------------------------------------------------------------
# Construct the HTML page support.
# This needs to run in a separate container, because it is compiled with an x86-64 platform.
# That's just rust; it seems to only like musl x86_x64.
FROM --platform=x86_64 docker.io/library/alpine:3.19

COPY --from=iso /home/build/alpine-html-v3.19-x86.tar /opt/images/alpine-html-v3.19-x86.tar
COPY docs/interactive-html/interactive.html /opt

#RUN set -x \
#    # Basic required tools for ISO creation.
#    && apk update \
#    && apk add \
#        git xz \
#        curl clang make openjdk8 npm python3 \
#    # Install rust.
#    && curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y \
#    # Install the wasm32 rust package.
#    && PATH="${HOME}/.cargo/bin:${PATH}" rustup target add wasm32-unknown-unknown \
#    # Prepare v86
#    && mkdir -p /opt \
#    && git clone --depth=1 https://github.com/copy/v86.git /opt/v86 \
#    && mv /opt/images /opt/v86/.
#
## Build the HDD
#RUN set -x \
#    && cd /opt/v86/images \
#    && 
#
## Construct the interactive html page
#RUN set -x \
#    && . $HOME/.cargo/env \
#    && cd /opt/v86 \
#    && make all \
#    && rm -rf \
#        closure-compiler gen \
#        tools .cargo Makefile \
#        examples tests docs Readme.md \
#        .git .github .gitattributes .gitignore \
#        .devcontainer .jshint.json .rustfmt.toml .vscode .editorconfig \
#        Cargo.lock Cargo.toml \
#        build/release build/.rustc_info.json \
#        build/softfloat.o build/zstddeclib.o \
#        bios/fetch-and-build-seabios.sh \
#        index.html debug.html \
#    && mv /opt/interactive.html . \
#    && tar zcf /opt/presh-html.tar.gz *
#
## TODO add the out directory to the iso profile path.
## TODO zstdlib compress the iso.
