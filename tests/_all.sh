#!/bin/bash

cd "$( dirname "$0" )"
if [ "${DEBUG}" = "1" ] ; then
    exec bash -x ./_run.sh *.sh
else
    exec ./_run.sh *.sh
fi
