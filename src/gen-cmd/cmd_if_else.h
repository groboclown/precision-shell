// GENERATED FROM cmd_if_else.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_IF_ELSE_
#define _FS_SHELL__CMD_IF_ELSE_

// Optional command if-else

#ifdef USE_CMD_IF_ELSE



#include <stdlib.h>
#include "args.h"
#include "output.h"
#include "globals.h"
#include "command_runner.h"







    // Runs the first argument.  If the error code

    // is zero, then it runs the second, otherwise it runs the third.


    // Run the step.

    // The state of the next command is determined by

    // global_arg2_i, and its follow up is global_arg3_i


    // Pass over this argument.

    // The state of the next command is determined by

    // global_arg2_i, and its follow up is global_arg3_i
/* from cmd_if_else.h.in:35 */
extern const char cmd_name_if_else[];
#define ENUM_LIST__IF_ELSE \
            /* from cmd_if_else.h.in:35 */ \
            COMMAND_INDEX__IF_ELSE,
#define VIRTUAL_ENUM_LIST__IF_ELSE \
            /* from cmd_if_else.h.in:59 */ \
            COMMAND_INDEX__IF_ELSE__RUN, \
            /* from cmd_if_else.h.in:78 */ \
            COMMAND_INDEX__IF_ELSE__SKIP,
#define GLOBAL_VARDEF__IF_ELSE \
            /* from cmd_if_else.h.in:35 */ \
            const char cmd_name_if_else[] = "if-else";
#define INITIALIZE__IF_ELSE \
            /* from cmd_if_else.h.in:35 */ \
            command_list_names[COMMAND_INDEX__IF_ELSE] = cmd_name_if_else;
#define STARTUP_CASE__IF_ELSE
#define RUN_CASE__IF_ELSE \
    case COMMAND_INDEX__IF_ELSE: \
        /* from cmd_if_else.h.in:35 */ \
            /* from cmd_if_else.h.in:40 */ \
        shared_split_input_argv[2] = global_arg; \
        shared_split_arg_state = args_setup_tokenizer( \
            3, (char **) shared_split_input_argv, \
            /* Any way to force this to not parse env?*/ \
            1); \
        /* Note: error code is not going to be in the global_err*/ \
        tmp_val = command_runner(shared_split_arg_state); \
        global_arg3_i = COMMAND_INDEX__ERR; \
        if (tmp_val == 0) { \
            global_cmd = COMMAND_INDEX__IF_ELSE__RUN; \
            global_arg2_i = COMMAND_INDEX__IF_ELSE__SKIP; \
        } else { \
            global_cmd = COMMAND_INDEX__IF_ELSE__SKIP; \
            global_arg2_i = COMMAND_INDEX__IF_ELSE__RUN; \
        } \
        break; \
    case COMMAND_INDEX__IF_ELSE__RUN: \
        /* from cmd_if_else.h.in:59 */ \
            /* from cmd_if_else.h.in:64 */ \
        LOG(":: running argument in a sub-command\n"); \
        shared_split_input_argv[2] = global_arg; \
        shared_split_arg_state = args_setup_tokenizer( \
            3, (char **) shared_split_input_argv, \
            /* Any way to force this to not parse env?*/ \
            1); \
        global_err = command_runner(shared_split_arg_state); \
        global_cmd = global_arg2_i; \
        global_arg2_i = global_arg3_i; \
        break; \
    case COMMAND_INDEX__IF_ELSE__SKIP: \
        /* from cmd_if_else.h.in:78 */ \
            /* from cmd_if_else.h.in:82 */ \
        global_cmd = global_arg2_i; \
        global_arg2_i = global_arg3_i; \
        LOG(":: skipping argument\n"); \
        break;
#define REQUIRES_ADDL_ARG__IF_ELSE \
            case COMMAND_INDEX__IF_ELSE:

#else /* USE_CMD_IF_ELSE */

#define ENUM_LIST__IF_ELSE
#define VIRTUAL_ENUM_LIST__IF_ELSE
#define GLOBAL_VARDEF__IF_ELSE
#define INITIALIZE__IF_ELSE
#define STARTUP_CASE__IF_ELSE
#define RUN_CASE__IF_ELSE
#define REQUIRES_ADDL_ARG__IF_ELSE
#endif /* USE_CMD_IF_ELSE */

#endif /* _FS_SHELL__CMD_IF_ELSE_ */
