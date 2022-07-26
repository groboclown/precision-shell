#!/bin/bash

# desc: argument parsing testing the buffer size

# max buffer size isn't changed by any of the official builds.
# If you change PARSED_ARG_SIZE then this will fail, so you
# will need to set the variable yourself.
if [ -z "${PARSED_ARG_SIZE}" ] ; then
    PARSED_ARG_SIZE=1000
fi
long_arg=""
while [ "${#long_arg}" -le ${PARSED_ARG_SIZE} ] ; do
    long_arg="${long_arg}a"
done
# Make it longer
long_arg="${long_arg}terminated"

"${FS}" -c "${long_arg}" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
    cat err.txt
    exit 1
fi

# the in-progress handling of the escape sequence is terminated early and the
#   backslash is not included in the parsed arguments.
if [ "$( printf "ERROR Argument length too long\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
    cat err.txt
    exit 1
fi

# should have: out.txt and err.txt
count="$( ls -1A | wc -l )"
if [ ${count} != 2 ] ; then
    echo "Generated unexpected files:"
    ls -lA
    exit 1
fi
