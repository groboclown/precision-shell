FROM docker.io/library/alpine:3.19

# This is controlled through the docker build argument "--build-arg FLAGS=(value)"
# Use this to list the commands to include.  See Makefile.command-flags for
# the complete list of command flags.
ARG FLAGS="INCLUDE_ALL_COMMANDS=1"

WORKDIR /opt/code

COPY Makefile.command-flags Makefile.compiler-flags version.txt ./
COPY build-tools/generate-command-template.py build-tools/
COPY src/ src/

ENV FLAGS=$FLAGS

RUN set -x \
    && apk --no-cache add "build-base=~0" \
    && rm -rf /tmp/* /var/cache/apk/* \
    && echo 'LIBNAME=musl' >> version.txt \
    && cd src && make ${FLAGS}

# You can include "python3" in the pulled packages
# if you want the command file headers rebuilt.

# ---------------------------------
# The real image.
FROM scratch
LABEL name="local/precision-shell" \
      version="4.6.0"

# Set the executable under the file "/bin/sh", so that
#   docker will use it as the default shell when it encounters
#   the "RUN" instruction.
# This also chooses the features to include.  In this case, it's the
#   all-feature version.
COPY --from=0 \
    /opt/code/out/presh \
    /bin/sh

ENTRYPOINT ["/bin/sh"]
CMD ["/bin/sh"]
