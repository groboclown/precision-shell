// GENERATED FROM cmd_su_exec.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_SU_EXEC_
#define _FS_SHELL__CMD_SU_EXEC_

// Optional command su-exec

#ifdef USE_CMD_SU_EXEC



// Exec takes a single argument and parses it into arguments using the
// input_loader.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "args.h"
#include "output.h"
#include "globals.h"


#ifdef DEBUG
#define SU_EXEC_DEBUG_REPORT \
    LOG(":: Running command ["); \
    LOG(shared_split_argv[0]); \
    LOG("] with arguments ["); \
    for (global_arg2_i = 0; shared_split_argv[global_arg2_i] != NULL; global_arg2_i++) { \
        if (global_arg2_i > 0) { \
            LOG("] ["); \
        } \
        LOG(shared_split_argv[global_arg2_i]); \
    } \
    LOG("]\n");
#else
#define SU_EXEC_DEBUG_REPORT
#endif




/* from cmd_su_exec.h.in:55 */
extern const char cmd_name_su_exec[];
#define ENUM_LIST__SU_EXEC \
            /* from cmd_su_exec.h.in:55 */ \
            COMMAND_INDEX__SU_EXEC,
#define VIRTUAL_ENUM_LIST__SU_EXEC \
            /* from cmd_su_exec.h.in:79 */ \
            COMMAND_INDEX__SU_EXEC__RUN,
#define GLOBAL_VARDEF__SU_EXEC \
            /* from cmd_su_exec.h.in:55 */ \
            const char cmd_name_su_exec[] = "su-exec";
#define INITIALIZE__SU_EXEC \
            /* from cmd_su_exec.h.in:55 */ \
            command_list_names[COMMAND_INDEX__SU_EXEC] = cmd_name_su_exec;
#define STARTUP_CASE__SU_EXEC \
    case COMMAND_INDEX__SU_EXEC: \
        /* from cmd_su_exec.h.in:55 */ \
            /* from cmd_su_exec.h.in:58 */ \
        /* arg2: uid*/ \
        global_cmd = COMMAND_INDEX__SHARED_INT2; \
        global_arg3_i = COMMAND_INDEX__SU_EXEC; \
        break;
#define RUN_CASE__SU_EXEC \
    case COMMAND_INDEX__SU_EXEC: \
        /* from cmd_su_exec.h.in:55 */ \
            /* from cmd_su_exec.h.in:64 */ \
        /* arg1: gid*/ \
        LOG(":: storing gid "); \
        LOGLN(global_arg); \
        tmp_val = helper_arg_to_uint(global_arg, 10, 0xffff); \
        if (tmp_val < 0) { \
            global_err = 1; \
            global_cmd = COMMAND_INDEX__ERR; \
            break; \
        } \
        global_arg1_i = tmp_val; \
        global_cmd = COMMAND_INDEX__SU_EXEC__RUN; \
        break; \
    case COMMAND_INDEX__SU_EXEC__RUN: \
        /* from cmd_su_exec.h.in:79 */ \
            /* from cmd_su_exec.h.in:81 */ \
        /* Split the arguments.*/ \
        SHARED_SPLIT__PARSE_ARG \
        SU_EXEC_DEBUG_REPORT \
        /* Delay switching UID and GID until the last possible moment.*/ \
        /* Note that gid is done first.*/ \
        global_arg1_i = setgid(global_arg1_i); \
        if (global_arg1_i >= 0) { \
            global_arg1_i = setuid(global_arg2_i); \
            if (global_arg1_i >= 0) { \
                /* This launches a new executable and terminates this one immediately.*/ \
                execvp(shared_split_argv[0], (char * const*) shared_split_argv); \
            } \
        } \
        /* Unlike the normal exec, this must exit*/ \
        /* immediately due to the setuid/setgid.  We don't*/ \
        /* want unexpected commands running with unexpected permissions.*/ \
        stderrP("Failed to su-exec "); \
        stderrPLn(shared_split_argv[0]); \
        _exit(1); \
        break;
#define REQUIRES_ADDL_ARG__SU_EXEC \
            case COMMAND_INDEX__SU_EXEC:

#else /* USE_CMD_SU_EXEC */

#define ENUM_LIST__SU_EXEC
#define VIRTUAL_ENUM_LIST__SU_EXEC
#define GLOBAL_VARDEF__SU_EXEC
#define INITIALIZE__SU_EXEC
#define STARTUP_CASE__SU_EXEC
#define RUN_CASE__SU_EXEC
#define REQUIRES_ADDL_ARG__SU_EXEC
#endif /* USE_CMD_SU_EXEC */

#endif /* _FS_SHELL__CMD_SU_EXEC_ */
