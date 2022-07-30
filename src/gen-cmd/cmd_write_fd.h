// GENERATED FROM cmd_write_fd.h.in.  DO NOT EDIT.

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


#ifndef _FS_SHELL__CMD_WRITE_FD_
#define _FS_SHELL__CMD_WRITE_FD_

// Optional command write-fd

#ifdef USE_CMD_WRITE_FD


#include "output.h"
#include "globals.h"
#include "helpers.h"



/* from cmd_write_fd.h.in:31 */
extern const char cmd_name_write_fd[];
#define ENUM_LIST__WRITE_FD \
            /* from cmd_write_fd.h.in:31 */ \
            COMMAND_INDEX__WRITE_FD,
#define VIRTUAL_ENUM_LIST__WRITE_FD \
            /* from cmd_write_fd.h.in:44 */ \
            COMMAND_INDEX__WRITE_FD__WRITE,
#define GLOBAL_VARDEF__WRITE_FD \
            /* from cmd_write_fd.h.in:31 */ \
            const char cmd_name_write_fd[] = "write-fd";
#define INITIALIZE__WRITE_FD \
            /* from cmd_write_fd.h.in:31 */ \
            command_list_names[COMMAND_INDEX__WRITE_FD] = cmd_name_write_fd;
#define STARTUP_CASE__WRITE_FD \
    case COMMAND_INDEX__WRITE_FD: \
        /* from cmd_write_fd.h.in:31 */ \
            /* from cmd_write_fd.h.in:34 */ \
        /* The first argument is the FD to write to.*/ \
        /*   which will be put into global_arg2_i*/ \
        global_cmd = COMMAND_INDEX__SHARED_INT2; \
        /* The command after that is to write a string.*/ \
        global_arg3_i = COMMAND_INDEX__WRITE_FD__WRITE; \
        break;
#define RUN_CASE__WRITE_FD \
    case COMMAND_INDEX__WRITE_FD__WRITE: \
        /* from cmd_write_fd.h.in:44 */ \
            /* from cmd_write_fd.h.in:45 */ \
        LOG(":: print\n"); \
        global_arg1_i = strlen(global_arg); \
        if (write(global_arg2_i, global_arg, global_arg1_i) != global_arg1_i) { \
            global_err = 1; \
        } \
        break;
#define REQUIRES_ADDL_ARG__WRITE_FD \
            case COMMAND_INDEX__WRITE_FD:

#else /* USE_CMD_WRITE_FD */

#define ENUM_LIST__WRITE_FD
#define VIRTUAL_ENUM_LIST__WRITE_FD
#define GLOBAL_VARDEF__WRITE_FD
#define INITIALIZE__WRITE_FD
#define STARTUP_CASE__WRITE_FD
#define RUN_CASE__WRITE_FD
#define REQUIRES_ADDL_ARG__WRITE_FD
#endif /* USE_CMD_WRITE_FD */

#endif /* _FS_SHELL__CMD_WRITE_FD_ */
