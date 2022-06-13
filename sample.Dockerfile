FROM scratch
LABEL name="local/fs-shell-example"

# fs-shell must be built before this image.
# The build.Dockerfile will create it, but you need to extract it
# yourself.  Alternatively, you can use the Dockerfile formula to
# build the fs-shell within the same Dockerfile that it's used.
COPY out/fs-shell /bin/sh

# Example setting up files and performing some file manipulations
# on them.
COPY extract-executables.sh CHANGES.md /tmp/

RUN echo Test3
RUN chmod 755 /tmp/extract-executables.sh \
    && rm /tmp/CHANGES.md \
    && mkdir 777 /opt/special \
    && echo "All done."
