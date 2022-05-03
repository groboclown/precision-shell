FROM docker.io/library/alpine:3.10

WORKDIR /opt/code

COPY *.c *.h Makefile ./

RUN \
       apk --no-cache update \
    && apk add "build-base=~0" \
    && rm -rf /tmp/* /var/cache/apk/* \
    && make


# ---------------------------------
# The real image.
FROM scratch
LABEL name="local/fs-shell" \
      version="1.0"

# Set the executable under the file "/bin/sh", so that
# docker will use it as the default shell when it encounters
# the "RUN" instruction.
COPY --from=0 \
    /opt/code/fs-shell \
    /bin/sh

ENTRYPOINT ["/bin/sh"]
CMD ["/bin/sh"]
