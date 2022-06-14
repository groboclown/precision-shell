/*
MIT License

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
SOFTWARE.
*/

#include "uses.h"

#if defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R)

#include <fcntl.h>

#include "output.h"
#include "globals.h"
#include "helpers.h"
#include "command_list.h"
#include "command_common.h"
#include "cmd_dup.h"

#ifdef USE_CMD_DUP_A
const char command_name_dup_a[] = NAME__CMD_DUP_A;

// The fd calls the common int base 10 function, which overwrites arg2.
// So we use arg3 to indicate the file open flags.
int cmd_dup_setup_a(int idx) {
    LOG(":: preparing dup-a\n");
    global_arg3_i = O_WRONLY | O_CREAT | O_APPEND;
    return COMMAND_INDEX__DUP_FD;
}

#endif /* USE_CMD_DUP_A */
#ifdef USE_CMD_DUP_W
const char command_name_dup_w[] = NAME__CMD_DUP_W;

int cmd_dup_setup_w(int idx) {
    LOG(":: preparing dup-w\n");
    global_arg3_i = O_WRONLY | O_CREAT | O_TRUNC;
    return COMMAND_INDEX__DUP_FD;
}

#endif /* USE_CMD_DUP_W */
#ifdef USE_CMD_DUP_R
const char command_name_dup_r[] = NAME__CMD_DUP_R;

int cmd_dup_setup_r(int idx) {
    LOG(":: preparing dup-w\n");
    global_arg3_i = O_RDONLY;
    return COMMAND_INDEX__DUP_FD;
}

#endif /* USE_CMD_DUP_R */


int cmd_dup_run_tgt() {
    // global_arg3_i is set to the file open flags
    // global_arg1_i is set to the source file descriptor
    // global_arg is the target file/fd
    int fd;

    // Do not allow another target; it doesn't make sense
    global_cmd = COMMAND_INDEX__ERR;

    LOG(":: dup ");
    if (strequal("&2", global_arg)) {
        LOG(" stderr\n");
        fd = STDERR_FILENO;
    } else if (strequal("&1", global_arg)) {
        LOG(" stdout\n");
        fd = STDOUT_FILENO;
    } else if (strequal("&0", global_arg)) {
        LOG(" stdin\n");
        fd = STDIN_FILENO;
    } else {
        LOGLN(global_arg);
        fd = open(
            global_arg, global_arg3_i, global_fmode
        );
    }
    if (fd == -1) {
        return 1;
    } else {
        if (dup2(fd, global_arg1_i) == -1) {
            return 1;
        }
    }
    return 0;
}


#else
// disable pedantic warning
typedef int iso_translation_unit__dup;
#endif /* defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R) */
