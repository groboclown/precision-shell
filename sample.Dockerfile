# Builds the precision shell tool in one container, then adds that built version into the
# derived container.  This compiles with the musl library.
FROM public.ecr.aws/docker/library/alpine:3.22 AS builder
WORKDIR /opt/code

# Adjust this value during the image build to alter which commands
#   to include.
ARG COMMANDS="chmod ln-s"


ARG PRESH_BRANCH="v4.6.0"

ENV \
#    DEBUG=1 \
    BUILD_MODE=build \
    COMMANDS=$COMMANDS \
    BRANCH=$PRESH_BRANCH \
    UID1=1 \
    UID2=2 \
    GID1=1 \
    GID2=2

RUN set -x \
    && apk --no-cache add build-base=0.5-r3 "bash=~5" "python3=~3.12" git \
    && git clone --depth=1 --branch=$BRANCH https://github.com/groboclown/precision-shell.git /opt/code/precision-shell \
    && cd /opt/code/precision-shell \
    && echo 'LIBNAME=musl' >> version.txt \
    && ./build-tools/internal-docker-make.sh


# The real image, using what was just built.
FROM scratch
LABEL name="local/precision-shell-example"

# Copy the image built above and set it as the shell.
COPY --from=builder /opt/code/precision-shell/out/presh-smallest /bin/sh

ENTRYPOINT ["/bin/sh"]
