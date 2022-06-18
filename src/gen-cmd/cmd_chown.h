// GENERATED FROM cmd_chown.h.in.  DO NOT EDIT.

/* MIT License

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
SOFTWARE. */

#ifndef _FS_SHELL__CMD_CHOWN_
#define _FS_SHELL__CMD_CHOWN_

// Optional command chown

#ifdef USE_CMD_CHOWN


#include <sys/stat.h>
#include <fcntl.h>

#include "output.h"
#include "globals.h"
#include "helpers.h"


/* from cmd_chown.h.in:31 */
extern const char cmd_name_chown_owner[];
#define ENUM_LIST__CHOWN \
            /* from cmd_chown.h.in:31 */ \
            COMMAND_INDEX__CHOWN_OWNER,
#define VIRTUAL_ENUM_LIST__CHOWN \
            /* from cmd_chown.h.in:50 */ \
            COMMAND_INDEX__CHOWN_RUN,
#define GLOBAL_VARDEF__CHOWN \
            /* from cmd_chown.h.in:31 */ \
            const char cmd_name_chown_owner[] = "chown";
#define INITIALIZE__CHOWN \
            /* from cmd_chown.h.in:31 */ \
            command_list_names[COMMAND_INDEX__CHOWN_OWNER] = cmd_name_chown_owner;
#define STARTUP_CASE__CHOWN
#define RUN_CASE__CHOWN \
    case COMMAND_INDEX__CHOWN_OWNER: \
        /* from cmd_chown.h.in:31 */ \
            /* from cmd_chown.h.in:34 */ \
        LOG(":: chown owner id "); \
        LOGLN(global_arg); \
        /* parse this argument into arg1.*/ \
        /* the next command will run shared_int2, which puts*/ \
        /* that argument parsed into arg2, then runs the next*/ \
        /* step in arg3, which is the run command.*/ \
        global_cmd = COMMAND_INDEX__SHARED_INT2; \
        global_arg3_i = COMMAND_INDEX__CHOWN_RUN; \
        global_arg1_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
        if (global_arg1_i < 0) { \
            global_err = 1; \
            global_cmd = COMMAND_INDEX__ERR; \
        } \
        break; \
    case COMMAND_INDEX__CHOWN_RUN: \
        /* from cmd_chown.h.in:50 */ \
            /* from cmd_chown.h.in:51 */ \
        LOG(":: chown on "); \
        LOGLN(global_arg); \
        global_err = chown(global_arg, global_arg1_i, global_arg2_i); \
        break;
#define REQUIRES_ADDL_ARG__CHOWN \
            case COMMAND_INDEX__CHOWN_OWNER:

#else /* USE_CMD_CHOWN */

#define ENUM_LIST__CHOWN
#define VIRTUAL_ENUM_LIST__CHOWN
#define GLOBAL_VARDEF__CHOWN
#define INITIALIZE__CHOWN
#define STARTUP_CASE__CHOWN
#define RUN_CASE__CHOWN
#define REQUIRES_ADDL_ARG__CHOWN
#endif /* USE_CMD_CHOWN */

#endif /* _FS_SHELL__CMD_CHOWN_ */
