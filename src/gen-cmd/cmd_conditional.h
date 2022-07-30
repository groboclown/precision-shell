// GENERATED FROM cmd_conditional.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_CONDITIONAL_
#define _FS_SHELL__CMD_CONDITIONAL_

// Optional command conditional

#ifdef USE_CMD_CONDITIONAL



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
/* from cmd_conditional.h.in:34 */
extern const char cmd_name_conditional[];
#define ENUM_LIST__CONDITIONAL \
            /* from cmd_conditional.h.in:34 */ \
            COMMAND_INDEX__CONDITIONAL,
#define VIRTUAL_ENUM_LIST__CONDITIONAL \
            /* from cmd_conditional.h.in:58 */ \
            COMMAND_INDEX__CONDITIONAL__RUN, \
            /* from cmd_conditional.h.in:77 */ \
            COMMAND_INDEX__CONDITIONAL__SKIP,
#define GLOBAL_VARDEF__CONDITIONAL \
            /* from cmd_conditional.h.in:34 */ \
            const char cmd_name_conditional[] = "?:";
#define INITIALIZE__CONDITIONAL \
            /* from cmd_conditional.h.in:34 */ \
            command_list_names[COMMAND_INDEX__CONDITIONAL] = cmd_name_conditional;
#define STARTUP_CASE__CONDITIONAL
#define RUN_CASE__CONDITIONAL \
    case COMMAND_INDEX__CONDITIONAL: \
        /* from cmd_conditional.h.in:34 */ \
            /* from cmd_conditional.h.in:38 */ \
        shared_split_input_argv[2] = global_arg; \
        shared_split_arg_state = args_setup_tokenizer( \
            3, (char **) shared_split_input_argv, \
            /* Any way to force this to not parse env?*/ \
            1); \
        /* Note: error code is not going to be in the global_err*/ \
        tmp_val = command_runner(shared_split_arg_state); \
        if (tmp_val == 0) { \
            global_cmd = COMMAND_INDEX__CONDITIONAL__RUN; \
            global_arg2_i = COMMAND_INDEX__CONDITIONAL__SKIP; \
            global_arg3_i = COMMAND_INDEX__ERR; \
        } else { \
            global_cmd = COMMAND_INDEX__CONDITIONAL__SKIP; \
            global_arg2_i = COMMAND_INDEX__CONDITIONAL__RUN; \
            global_arg3_i = COMMAND_INDEX__ERR; \
        } \
        break; \
    case COMMAND_INDEX__CONDITIONAL__RUN: \
        /* from cmd_conditional.h.in:58 */ \
            /* from cmd_conditional.h.in:63 */ \
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
    case COMMAND_INDEX__CONDITIONAL__SKIP: \
        /* from cmd_conditional.h.in:77 */ \
            /* from cmd_conditional.h.in:81 */ \
        global_cmd = global_arg2_i; \
        global_arg2_i = global_arg3_i; \
        LOG(":: skipping argument\n"); \
        break;
#define REQUIRES_ADDL_ARG__CONDITIONAL

#else /* USE_CMD_CONDITIONAL */

#define ENUM_LIST__CONDITIONAL
#define VIRTUAL_ENUM_LIST__CONDITIONAL
#define GLOBAL_VARDEF__CONDITIONAL
#define INITIALIZE__CONDITIONAL
#define STARTUP_CASE__CONDITIONAL
#define RUN_CASE__CONDITIONAL
#define REQUIRES_ADDL_ARG__CONDITIONAL
#endif /* USE_CMD_CONDITIONAL */

#endif /* _FS_SHELL__CMD_CONDITIONAL_ */
