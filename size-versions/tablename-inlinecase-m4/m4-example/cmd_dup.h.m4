WithLicense(`MIT License

Copyright (c) 2022 groboclown

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.')
AsOptionalUnion(`dup', `

AsOptional(`dup-a', `

    WithNamedStep(DUP_A__SETUP, "dup-a", `
        OnCmd(
            LOG(":: preparing dup-a\n");
            global_arg1_i = O_WRONLY | O_CREAT | O_APPEND;
            global_cmd = COMMAND_INDEX__DUP__FD;
        )
    ')

')


AsOptional(`dup-w', `

    WithNamedStep(DUP_W__SETUP, "dup-w", `
        OnCmd(
            LOG(":: preparing dup-w\n");
            global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC;
            global_cmd = COMMAND_INDEX__DUP__FD;
        )
    ')

')


AsOptional(`dup-r', `

    WithNamedStep(DUP_R__SETUP, "dup-r", `
        OnCmd(
            LOG(":: preparing dup-w\n");
            global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC;
            global_cmd = COMMAND_INDEX__DUP__FD;
        )
    ')

')


WithAuxFor(`DUP_A, DUP_W, DUP_R', `

#include <sys/stat.h>
#include <fcntl.h>

#include "output.h"
#include "globals.h"
#include "helpers.h"

    WithVirtualStep(DUP__FD, `
        OnArg(
            LOG(":: dup fd read\n");
            global_arg2_i = helper_arg_to_uint(global_arg, 10, 0xffff);
            global_cmd = COMMAND_INDEX__DUP__TGT;
            if (global_arg2_i < 0) {
                global_err = 1;
                global_cmd = COMMAND_INDEX__ERR;
            }
        )
    ')
    WithVirtualStep(DUP__TGT, `
        OnArg(
            LOG(":: dup ");
            if (strequal("&2", global_arg)) {
                LOG(" stderr\n");
                global_arg3_i = STDERR_FILENO;
            } else if (strequal("&1", global_arg)) {
                LOG(" stdout\n");
                global_arg3_i = STDOUT_FILENO;
            } else if (strequal("&0", global_arg)) {
                LOG(" stdin\n");
                global_arg3_i = STDIN_FILENO;
            } else {
                LOGLN(global_arg);
                global_arg3_i = open(
                    global_arg, global_arg1_i, global_fmode
                );
            }
            if (global_arg3_i == -1) {
                global_err = -1;
            } else {
                if (dup2(global_arg3_i, global_arg2_i) == -1) {
                    global_err = 1;
                }
            }
        )
    ')

')
')
