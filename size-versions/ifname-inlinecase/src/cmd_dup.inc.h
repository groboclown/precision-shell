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

#ifndef _FS_SHELL__CMD_DUP_

#define NAME__DUP_A "dup-a"
#define NAME__DUP_W "dup-w"
#define NAME__DUP_R "dup-r"
#define NAME__DUP__FD ""
#define NAME__DUP__TGT ""

// No startup for these
#define STARTUP__COMMAND_INDEX__DUP__FD
#define STARTUP__COMMAND_INDEX__DUP__TGT

// And the dup commands have no case because they fall directly into the fd.
#define CASE__COMMAND_INDEX__DUP_A
#define CASE__COMMAND_INDEX__DUP_W
#define CASE__COMMAND_INDEX__DUP_R


// These commands at startup set global_arg1 to the file open mode.
// The fd stores the file descriptor in global_arg2

// Setup prepares for the execution
#ifdef USE_CMD_DUP_A

#define STARTUP__COMMAND_INDEX__DUP_A \
else if (strequal(global_arg, NAME__DUP_A)) { \
    LOG(":: preparing dup-a\n"); \
    global_arg1_i = O_WRONLY | O_CREAT | O_APPEND; \
    global_cmd = COMMAND_INDEX__DUP__FD; \
}

#else /* USE_CMD_DUP_A */
#define STARTUP__COMMAND_INDEX__DUP_A
#endif /* USE_CMD_DUP_A */


#ifdef USE_CMD_DUP_W

#define STARTUP__COMMAND_INDEX__DUP_W \
else if (strequal(global_arg, NAME__DUP_W)) { \
     LOG(":: preparing dup-w\n"); \
    global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC; \
    global_cmd = COMMAND_INDEX__DUP__FD; \
}


#else /* USE_CMD_DUP_W */
#define STARTUP__COMMAND_INDEX__DUP_W
#endif /* USE_CMD_DUP_W */


#ifdef USE_CMD_DUP_R

#define STARTUP__COMMAND_INDEX__DUP_R \
else if (strequal(global_arg, NAME__DUP_R)) { \
    LOG(":: preparing dup-r\n"); \
    global_arg1_i = O_RDONLY; \
    global_cmd = COMMAND_INDEX__DUP__FD; \
}


#else /* USE_CMD_DUP_R */
#define STARTUP__COMMAND_INDEX__DUP_R
#endif /* USE_CMD_DUP_R */


// Execution runs the same across all these commands.
// should be ifdef USES_DUP
#if defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R)

#include <fcntl.h>

#define CASE__COMMAND_INDEX__DUP__FD \
case COMMAND_INDEX__DUP__FD: \
    LOG(":: dup fd read\n"); \
    global_arg2_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
    if (global_arg2_i < 0) { \
        global_err = 1; \
        global_cmd = COMMAND_INDEX__ERR; \
    } \
    global_cmd = COMMAND_INDEX__DUP__TGT; \
    break;

#define CASE__COMMAND_INDEX__DUP__TGT \
case COMMAND_INDEX__DUP__TGT: \
    LOG(":: dup "); \
    if (strequal("&2", global_arg)) { \
        LOG(" stderr\n"); \
        global_arg3_i = STDERR_FILENO; \
    } else if (strequal("&1", global_arg)) { \
        LOG(" stdout\n"); \
        global_arg3_i = STDOUT_FILENO; \
    } else if (strequal("&0", global_arg)) { \
        LOG(" stdin\n"); \
        global_arg3_i = STDIN_FILENO; \
    } else { \
        LOGLN(global_arg); \
        global_arg3_i = open( \
            global_arg, global_arg1_i, global_fmode \
        ); \
    } \
    if (global_arg3_i == -1) { \
        global_err = -1; \
    } else { \
        if (dup2(global_arg3_i, global_arg2_i) == -1) { \
            global_err = 1; \
        } \
    } \
    /* Do not allow another target; it doesn't make sense */ \
    global_cmd = COMMAND_INDEX__ERR; \
    break;

#else /* defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R) */

#define CASE__COMMAND_INDEX__DUP__FD
#define CASE__COMMAND_INDEX__DUP__TGT

#endif /* defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R) */

#endif /* _FS_SHELL__CMD_DUP_ */
