#!/bin/bash

# desc: Environment variable replacement miss with large argument.
# requires: +echo +enviro

if [ -z "${PARSED_ARG_SIZE}" ] ; then
    PARSED_ARG_SIZE=1000
fi
export VAL=x
unset VAL
export VAL
long_arg=""
# we'll tack on ${VAL} at the end, but have that be longer than the
# allowed argument length by 1 character.
while [ "${#long_arg}" -le $(( PARSED_ARG_SIZE - 5 )) ] ; do
    long_arg="${long_arg}a"
done
long_arg="${long_arg}\${VAL}"
export long_arg
"${FS}" -c "echo '${long_arg}'" > out.txt 2>err.txt
res=$?

if [ ${res} -ne 1 ] ; then
    echo "Bad exit code: ${res}"
    exit 1
fi

if [ "$( printf "ERROR Argument length too long\\n" )" != "$( cat err.txt )" ] ; then
    echo "Generated stderr not as expected:"
    cat err.txt
    exit 1
fi

# -s : file exists and not empty
if [ -s out.txt ] ; then
    echo "Generated output to stdout"
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
