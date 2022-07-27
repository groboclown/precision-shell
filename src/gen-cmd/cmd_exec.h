// GENERATED FROM cmd_exec.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_EXEC_
#define _FS_SHELL__CMD_EXEC_

// Optional command exec

#ifdef USE_CMD_EXEC



// Exec takes a single argument and parses it into arguments using the
// input_loader.

// Slurp up all remaining arguments up to command max.
// global_arg: current argument
// exec_argv: pointers to arguments.
// exec_arg3: container for all arguments copied in.  exec_argv points into this.
// global_arg1_i: position in exec_argv
// global_arg2_i: character position in argv[global_arg1_i]
// global_arg3_i: loop counter

#include <stdlib.h>
#include <stdio.h>
#include "args.h"
#include "output.h"
#include "globals.h"

// Number of arguments for exec
#define MAX_EXEC_ARGS 1000
// Number of characters, including trailing \0 for all the arguments concatenated together.
#define MAX_EXEC_ARG_LEN (MAX_EXEC_ARGS * 100)


#ifdef DEBUG
#define EXEC_DEBUG_REPORT \
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
#define EXEC_DEBUG_REPORT
#endif



/* from cmd_exec.h.in:67 */
extern const char cmd_name_exec[];
#define ENUM_LIST__EXEC \
            /* from cmd_exec.h.in:67 */ \
            COMMAND_INDEX__EXEC,
#define VIRTUAL_ENUM_LIST__EXEC
#define GLOBAL_VARDEF__EXEC \
            /* from cmd_exec.h.in:67 */ \
            const char cmd_name_exec[] = "exec";
#define INITIALIZE__EXEC \
            /* from cmd_exec.h.in:67 */ \
            command_list_names[COMMAND_INDEX__EXEC] = cmd_name_exec;
#define STARTUP_CASE__EXEC
#define RUN_CASE__EXEC \
    case COMMAND_INDEX__EXEC: \
        /* from cmd_exec.h.in:67 */ \
            /* from cmd_exec.h.in:69 */ \
        /* Split the arguments.*/ \
        SHARED_SPLIT__PARSE_ARG \
        if (global_arg3_i <= 1) { \
            /* No command to run*/ \
            stderrP("ERROR no command\n"); \
            /* Use "return" because the command is not expected to*/ \
            /* keep the tool running.*/ \
            return 1; \
        } \
        EXEC_DEBUG_REPORT \
        /* This launches a new executable and terminates this one immediately.*/ \
        execvp(shared_split_argv[0], (char * const*) shared_split_argv); \
        /* If the code is still running at this point, then there was an error.*/ \
        stderrP("ERROR exec failed to launch command "); \
        stderrPLn(shared_split_argv[0]); \
        /* Use "return" because the command is not expected to*/ \
        /* keep the tool running.*/ \
        return 1; \
        break;
#define REQUIRES_ADDL_ARG__EXEC

#else /* USE_CMD_EXEC */

#define ENUM_LIST__EXEC
#define VIRTUAL_ENUM_LIST__EXEC
#define GLOBAL_VARDEF__EXEC
#define INITIALIZE__EXEC
#define STARTUP_CASE__EXEC
#define RUN_CASE__EXEC
#define REQUIRES_ADDL_ARG__EXEC
#endif /* USE_CMD_EXEC */

#endif /* _FS_SHELL__CMD_EXEC_ */
