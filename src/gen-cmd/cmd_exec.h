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



// Strange form of the command.  All other arguments must be pulled in and run as-is.
// This takes over the command parsing loop

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
    LOG(exec_argv[0]); \
    LOG("] with arguments ["); \
    for (global_arg3_i = 0; exec_argv[global_arg3_i] != NULL; global_arg3_i++) { \
        if (global_arg3_i > 0) { \
            LOG("] ["); \
        } \
        LOG(exec_argv[global_arg3_i]); \
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
            command_list_names[COMMAND_INDEX__EXEC] = cmd_name_exec; \
            /* from cmd_exec.h.in:68 */ \
        /* This command needs some of its own variables.*/ \
        char **exec_argv; \
        char *exec_arg3;
#define STARTUP_CASE__EXEC \
    case COMMAND_INDEX__EXEC: \
        /* from cmd_exec.h.in:67 */ \
            /* from cmd_exec.h.in:74 */ \
        /* add a trailing + 1 for the final 0, if necessary*/ \
        exec_argv = malloc((sizeof(const char *) * MAX_EXEC_ARGS) + 1); \
        if (exec_argv == NULL) { \
            stderrP("ERROR malloc failed\n"); \
            return 1; \
        } \
        /* add a trailing + 1 for the final 0, if necessary*/ \
        exec_arg3 = malloc((sizeof(const char) * MAX_EXEC_ARG_LEN) + 1); \
        if (exec_arg3 == NULL) { \
            stderrP("ERROR malloc failed\n"); \
            return 1; \
        } \
        global_arg1_i = 0; \
        global_arg2_i = 0; \
        global_arg_state = args_advance_token(); \
        LOG(":: generating arguments\n"); \
        while (global_arg_state->state < ARG_STATE_END) { \
            if (global_arg1_i >= MAX_EXEC_ARGS) { \
                stderrP("ERROR exec too many arguments\n"); \
                /* Use "return" because the command is not expected to*/ \
                /* keep the tool running.*/ \
                return 1; \
            } \
            if (global_arg2_i >= MAX_EXEC_ARG_LEN) { \
                stderrP("ERROR exec argument total length exceeded\n"); \
                /* Use "return" because the command is not expected to*/ \
                /* keep the tool running.*/ \
                return 1; \
            } \
            global_arg = global_arg_state->arg; \
            /* set current argv index to point to start of argument, and advance argv index.*/ \
            exec_argv[global_arg1_i++] = &(exec_arg3[global_arg2_i]); \
            /* copy the current argument into the long array*/ \
            global_arg3_i = 0; \
            while (global_arg[global_arg3_i] != 0 && global_arg2_i < MAX_EXEC_ARG_LEN) { \
                exec_arg3[global_arg2_i++] = global_arg[global_arg3_i++]; \
            } \
            /* terminate the copied argument.*/ \
            exec_arg3[global_arg2_i++] = 0; \
            LOG(":: arg: ["); \
            LOG(exec_argv[global_arg1_i-1]); \
            LOG("]\n"); \
            global_arg_state = args_advance_token(); \
        } \
        if (global_arg_state->state == ARG_STATE_ERR) { \
            /* Argument parsing already reported the error.*/ \
            /* Use "return" because the command is not expected to*/ \
            /* keep the tool running.*/ \
            return 1; \
        } \
        /* set the final argument to NULL to terminate the list of pointers.*/ \
        exec_argv[global_arg1_i++] = NULL; \
        if (global_arg1_i <= 1) { \
            /* No command to run*/ \
            stderrP("ERROR no command\n"); \
            /* Use "return" because the command is not expected to*/ \
            /* keep the tool running.*/ \
            return 1; \
        } \
        EXEC_DEBUG_REPORT \
        /* This launches a new executable and terminates this one immediately.*/ \
        execvp(exec_argv[0], (char * const*) exec_argv); \
        /* If the code is still running at this point, then there was an error.*/ \
        stderrP("ERROR exec failed to launch command "); \
        stderrPLn(exec_argv[0]); \
        /* Use "return" because the command is not expected to*/ \
        /* keep the tool running.*/ \
        return 1; \
        break;
#define RUN_CASE__EXEC
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
