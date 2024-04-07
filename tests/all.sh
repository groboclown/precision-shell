#!/bin/bash

cd "$( dirname "$0" )"

if [ "${DEBUG}" = "1" ] ; then
    exec bash -x ./run.sh */*.sh
else
    exec ./run.sh */*.sh
fi
