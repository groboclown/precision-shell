#!/bin/bash

set -e

if [ -z "$1" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ] ; then
    echo "$0 OUT_TARFILE [IMAGE_NAME] [CONTAINER_NAME]"
    echo "Builds and extracts the interactive HTML site for precision-shell."
    echo "It writes the generated .tar.gz file to the argument."
    exit 0
fi

mkdir -p "$( dirname "$1" )"
outdir="$( cd "$( dirname "$1" )" && pwd )"
fname="$( basename "$1" )"
outfile="${outdir}/${fname}"

cd "$( dirname "$0" )/../.."

if [ -n "$2" ] ; then
    IMG="$2"
else
    IMG=local-presh/html-$$
fi
if [ -n "$3" ] ; then
    CNT="$3"
else
    CNT=local-presh-html-$$
fi

docker build -t ${IMG} -f docs/interactive-html/alpine.Dockerfile .
docker container rm ${CNT} || true
docker create --name ${CNT} ${IMG} ls
docker cp ${CNT}:/opt/presh-html.tar.gz "${outfile}"
docker container rm ${CNT} || true

if [ -z "$2" ] ; then
    docker rmi ${IMG}
fi
