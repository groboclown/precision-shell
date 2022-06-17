FROM docker.io/library/alpine:3.10

ARG FLAGS="INCLUDE_ALL_COMMANDS=1"

WORKDIR /opt/code

COPY Makefile.command-flags version.txt ./
COPY build-tools/generate-command-template.py build-tools/
COPY src/ src/

ENV FLAGS=$FLAGS

RUN \
       apk --no-cache update \
    && apk add "build-base=~0" "python3=~3.7" \
    && rm -rf /tmp/* /var/cache/apk/* \
    && echo 'LIBNAME=musl' >> version.txt \
    && cd src && make ${FLAGS}


# ---------------------------------
# The real image.
FROM scratch
LABEL name="local/fs-shell" \
      version="2.0.0"

# Set the executable under the file "/bin/sh", so that
#   docker will use it as the default shell when it encounters
#   the "RUN" instruction.
# This also chooses the features to include.  In this case, it's the
#   all-feature version.
COPY --from=0 \
    /opt/code/out/fs-shell \
    /bin/sh

ENTRYPOINT ["/bin/sh"]
CMD ["/bin/sh"]
