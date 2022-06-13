#!/bin/bash

# File invoked through Dockerfile to run the build.

if [ "${BUILD_MODE}" = "size-check" ] ; then
    experiments/size-check/compile-size-test.sh
elif [ "${BUILD_MODE}" = "src" ] ; then
    ( cd src && make src )
    ls -lA out/fs-shell*
else
    make all
    ls -lAS out/fs-shell*
fi
