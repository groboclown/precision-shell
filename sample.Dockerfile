FROM scratch

# COPY fs-shell-debug /bin/sh
COPY fs-shell /bin/sh
COPY tests/ /opt/tests/

RUN ["/bin/sh", "echo", "Test"]

RUN ["/bin/sh", "-c", "echo", "Startup", "&&", "rm", "/opt/tests/_all.sh", "&&", "echo", "Complete"]

RUN "echo" "Test2"

RUN echo Test3
