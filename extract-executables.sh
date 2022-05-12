#!/bin/bash

if [ -z "$1" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ] ; then
    echo "Usage: $0 (image name) [-o outdir] [-s suffix] [-b basename] [-d] [-i]"
    echo ""
    echo "This will extract the built executables out of the image created by one of"
    echo "the build-*.Dockerfile files, into the output directory (defaults to the"
    echo "current directory)."
    echo ""
    echo "If the suffix is provided, then the file names extracted include the given"
    echo "suffix.  So, for example, if '.glibc.o' is the suffix, then 'fs-shell-debug.o'"
    echo "will be copied out as 'fs-shell-debug.glibc.o'."
    echo ""
    echo "If the basename is provided, then the files will use that instead"
    echo "of 'fs-shell'."
    echo ""
    echo "If the '-d' argument is given, then the debug versions of the executables are"
    echo "also extracted."
    echo ""
    echo "If the '-i' argument is given, then the command will not overwrite existing"
    echo "files."
    echo ""
    echo "By default, this script will use the container executable declared in the"
    echo "environment variable 'CONTAINER_EXE'.  If this is unset, it will look for,"
    echo "in order, 'docker' then 'podman'"
    exit 0
fi

outdir=.
base=fs-shell
suffix=.o
force=1
image_name="$1"
all_files=". \
    .-signal .-input .-signal-input \
    .-exec .-signal-exec .-input-exec .-signal-input-exec \
    .-fat .-lean"
shift

while [ ! -z "$1" ] ; do
    arg="$1"
    shift
    case "${arg}" in
        -o)
            outdir="$1"
            shift
            ;;
        -s)
            suffix="$1"
            shift
            ;;
        -d)
            # Note: using -d twice causes an error.
            parts="${all_files}"
            all_files=""
            for n in ${parts} ; do
                all_files="${all_files} ${n} ${n}-debug"
            done
            ;;
        -b)
            base="$1"
            shift
            ;;
        -i)
            force=0
            ;;
        *)
            echo "Unknown argument '${arg}'"
            exit 1
            ;;
    esac
done
if [ -z "${outdir}" ] ; then
    echo "output directory cannot be empty."
    exit 1
fi
if [ -z "${base}" ] ; then
    echo "basename cannot be empty."
    exit 1
fi
# no suffix is okay

container_exe=""
for n in "${CONTAINER_EXE}" docker podman ; do
    if [ ! -z "${n}" ]; then
        if [ -x "${n}" ] ; then
            container_exe="${n}"
            break
        fi
        n="$( which "${n}" 2>/dev/null )"
        res=$?
        if [ ${res} == 0 ] && [ -x "${n}" ] ; then
            container_exe="${n}"
            break
        fi
    fi
done
if [ -z "${container_exe}" ] ; then
    echo "Could not find a container program to run (docker or podman)."
    exit 1
fi


container=$( "${container_exe}" create "${image_name}" ) || exit 1
for part in ${all_files} ; do
    part="${part:1}"
    src="${container}:/opt/code/fs-shell${part}.o"
    tgt="${outdir}/${base}${part}${suffix}"
    if [ ${force} = 0 ] && [ -e "${tgt}" ] ; then
        echo "${tgt} already exists"
    else
        # Keep going on failure.
        "${container_exe}" cp "${src}" "${tgt}"
    fi
done
"${container_exe}" rm "${container}" >/dev/null
