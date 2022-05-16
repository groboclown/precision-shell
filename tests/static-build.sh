#!/bin/bash

# desc: Compiled file is statically built.

objdump -p "${FS}" | grep NEEDED > needed-libraries.txt
count=$( cat needed-libraries.txt | wc -l )

if [ $? -ne 0 ] ; then
    echo "${FS} is dynamically linked."
    cat needed-libraries.txt
    exit 1
fi
exit 0
