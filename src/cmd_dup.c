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

#ifdef USES_DUP
// All of these commands enable USES_DUP, so this ifdef is fine to
//   force inclusion of any of the commands.

#include <fcntl.h>
#include "output.h"
#include "globals.h"
#include "command_list.h"
#include "helpers.h"


#ifdef USE_CMD_DUP_A

int cmd_dup_a_setup() {
    LOG(":: setting up dup for append mode\n");
    // setup arg1; the first argument runner will copy arg1 into arg2.
    global_arg1_i = O_WRONLY | O_CREAT | O_APPEND;
    return COMMAND_INDEX__DUP__FD;
}

#endif /* USE_CMD_DUP_A */

#ifdef USE_CMD_DUP_W

int cmd_dup_w_setup() {
    LOG(":: setting up dup for write/truncate mode\n");
    // setup arg1; the first argument runner will copy arg1 into arg2.
    global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC;
    return COMMAND_INDEX__DUP__FD;
}

#endif /* USE_CMD_DUP_W */

#ifdef USE_CMD_DUP_R

int cmd_dup_r_setup() {
    LOG(":: setting up dup for read mode\n");
    // setup arg1; the first argument runner will copy arg1 into arg2.
    global_arg1_i = O_RDONLY;
    return COMMAND_INDEX__DUP__FD;
}

#endif /* USE_CMD_DUP_R */



// run the dup command
int cmd_dup_run() {
    // global_arg2_i is now the read/write permissions for the file dup.
    // global_arg1_i is now the new target fd to dup into.

    int source_fd;
    LOG(":: running dup on ");
    
    if (strequal("&2", global_arg)) {
        LOG(" stderr\n");
        source_fd = STDERR_FILENO;
    } else if (strequal("&1", global_arg)) {
        LOG(" stdout\n");
        source_fd = STDOUT_FILENO;
    } else if (strequal("&0", global_arg)) {
        LOG(" stdin\n");
        source_fd = STDIN_FILENO;
    } else {
        LOGLN(global_arg);
        source_fd = open(
            global_arg, global_arg2_i, global_fmode
        );
    }

    if (source_fd == -1) {
        return 1;
    } else {
        if (dup2(source_fd, global_arg1_i) == -1) {
            return 1;
        }
    }

    // This should only run with a single argument.
    global_cmd = COMMAND_INDEX__ERR;
    return 0;
}


#else
// disable pedantic warning
typedef int iso_translation_unit_X;
#endif /* USES_DUP */
