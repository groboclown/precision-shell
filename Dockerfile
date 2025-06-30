# An example Dockerfile for building the precision shell and using it as the shell.
# It requires the presh files copied locally.  To see a version that builds off of
# github, see the `sample-github.Dockerfile` file.  The two work differently, in
# that this one only needs the 'src' directory plus 3 files at the top directory.
# It does not rebuild the headers or run tests.
FROM public.ecr.aws/docker/library/alpine:3.22

# This is controlled through the docker build argument "--build-arg FLAGS=(value)"
# Use this to list the commands to include.  See Makefile.command-flags for
# the complete list of command flags.
ARG FLAGS="INCLUDE_ALL_COMMANDS=1"

WORKDIR /opt/code

COPY Makefile.command-flags Makefile.compiler-flags version.txt ./
COPY src/ src/

ENV FLAGS=$FLAGS

RUN set -x \
    && apk --no-cache add "build-base=~0" \
    && rm -rf /tmp/* /var/cache/apk/* \
    && echo 'LIBNAME=musl' >> version.txt \
    && cd src && make ${FLAGS}

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
