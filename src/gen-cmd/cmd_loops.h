// GENERATED FROM cmd_loops.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_LOOPS_
#define _FS_SHELL__CMD_LOOPS_


#include <stdlib.h>
#include "args.h"
#include "output.h"
#include "globals.h"
#include "command_runner.h"


// Optional command while-no-error

#ifdef USE_CMD_WHILE_NO_ERROR



/* from cmd_loops.h.in:35 */
extern const char cmd_name_while_no_error[];
#define ENUM_LIST__WHILE_NO_ERROR \
            /* from cmd_loops.h.in:35 */ \
            COMMAND_INDEX__WHILE_NO_ERROR,
#define VIRTUAL_ENUM_LIST__WHILE_NO_ERROR
#define GLOBAL_VARDEF__WHILE_NO_ERROR \
            /* from cmd_loops.h.in:35 */ \
            const char cmd_name_while_no_error[] = "while-no-error";
#define INITIALIZE__WHILE_NO_ERROR \
            /* from cmd_loops.h.in:35 */ \
            command_list_names[COMMAND_INDEX__WHILE_NO_ERROR] = cmd_name_while_no_error;
#define STARTUP_CASE__WHILE_NO_ERROR \
    case COMMAND_INDEX__WHILE_NO_ERROR: \
        /* from cmd_loops.h.in:35 */ \
            /* from cmd_loops.h.in:37 */ \
            global_arg1_i = 0; \
            global_cmd = COMMAND_INDEX__SHARED_STR; \
            global_arg3_i = COMMAND_INDEX__WHILE_LOOP; \
        break;
#define RUN_CASE__WHILE_NO_ERROR
#define REQUIRES_ADDL_ARG__WHILE_NO_ERROR \
            case COMMAND_INDEX__WHILE_NO_ERROR:

#else /* USE_CMD_WHILE_NO_ERROR */

#define ENUM_LIST__WHILE_NO_ERROR
#define VIRTUAL_ENUM_LIST__WHILE_NO_ERROR
#define GLOBAL_VARDEF__WHILE_NO_ERROR
#define INITIALIZE__WHILE_NO_ERROR
#define STARTUP_CASE__WHILE_NO_ERROR
#define RUN_CASE__WHILE_NO_ERROR
#define REQUIRES_ADDL_ARG__WHILE_NO_ERROR
#endif /* USE_CMD_WHILE_NO_ERROR */


// Optional command while-error

#ifdef USE_CMD_WHILE_ERROR


/* from cmd_loops.h.in:48 */
extern const char cmd_name_while_error[];
#define ENUM_LIST__WHILE_ERROR \
            /* from cmd_loops.h.in:48 */ \
            COMMAND_INDEX__WHILE_ERROR,
#define VIRTUAL_ENUM_LIST__WHILE_ERROR
#define GLOBAL_VARDEF__WHILE_ERROR \
            /* from cmd_loops.h.in:48 */ \
            const char cmd_name_while_error[] = "while-error";
#define INITIALIZE__WHILE_ERROR \
            /* from cmd_loops.h.in:48 */ \
            command_list_names[COMMAND_INDEX__WHILE_ERROR] = cmd_name_while_error;
#define STARTUP_CASE__WHILE_ERROR \
    case COMMAND_INDEX__WHILE_ERROR: \
        /* from cmd_loops.h.in:48 */ \
            /* from cmd_loops.h.in:50 */ \
            global_arg1_i = 1; \
            global_cmd = COMMAND_INDEX__SHARED_STR; \
            global_arg3_i = COMMAND_INDEX__WHILE_LOOP; \
        break;
#define RUN_CASE__WHILE_ERROR
#define REQUIRES_ADDL_ARG__WHILE_ERROR \
            case COMMAND_INDEX__WHILE_ERROR:

#else /* USE_CMD_WHILE_ERROR */

#define ENUM_LIST__WHILE_ERROR
#define VIRTUAL_ENUM_LIST__WHILE_ERROR
#define GLOBAL_VARDEF__WHILE_ERROR
#define INITIALIZE__WHILE_ERROR
#define STARTUP_CASE__WHILE_ERROR
#define RUN_CASE__WHILE_ERROR
#define REQUIRES_ADDL_ARG__WHILE_ERROR
#endif /* USE_CMD_WHILE_ERROR */


// Optional command for-each

#ifdef USE_CMD_FOR_EACH






        // Because we change the next command at the end,

        //      requiring another arg only applies for this

        //      one virtual step.
/* from cmd_loops.h.in:68 */
extern const char cmd_name_for_each[];
#define ENUM_LIST__FOR_EACH \
            /* from cmd_loops.h.in:68 */ \
            COMMAND_INDEX__FOR_EACH,
#define VIRTUAL_ENUM_LIST__FOR_EACH \
            /* from cmd_loops.h.in:79 */ \
            COMMAND_INDEX__FOR_EACH_IN,
#define GLOBAL_VARDEF__FOR_EACH \
            /* from cmd_loops.h.in:68 */ \
            const char cmd_name_for_each[] = "for-each";
#define INITIALIZE__FOR_EACH \
            /* from cmd_loops.h.in:62 */ \
        const char *for_each_var_name = NULL; \
        struct ArgState *for_each_split_arg_state; \
        const Argument *for_each_split_arg; \
            /* from cmd_loops.h.in:68 */ \
            command_list_names[COMMAND_INDEX__FOR_EACH] = cmd_name_for_each;
#define STARTUP_CASE__FOR_EACH
#define RUN_CASE__FOR_EACH \
    case COMMAND_INDEX__FOR_EACH: \
        /* from cmd_loops.h.in:68 */ \
            /* from cmd_loops.h.in:70 */ \
            LOG(":: storing each loop item in "); \
            LOGLN(global_arg); \
            for_each_var_name = global_arg; \
            global_cmd = COMMAND_INDEX__SHARED_STR; \
            global_arg3_i = COMMAND_INDEX__FOR_EACH_IN; \
        break; \
    case COMMAND_INDEX__FOR_EACH_IN: \
        /* from cmd_loops.h.in:79 */ \
            /* from cmd_loops.h.in:84 */ \
            global_cmd = COMMAND_INDEX__ERR; \
            /* Env name to store the list value in is for_each_var_name*/ \
            /* List of values is in global_arg_cached*/ \
            /* Argument to loop over is in global_arg*/ \
            shared_split_input_argv[2] = global_arg_cached; \
            for_each_split_arg_state = args_setup_tokenizer(3, (char **) shared_split_input_argv, 0); \
            if (for_each_split_arg_state == NULL) { \
                global_err = 1; \
                break; \
            } \
            while (global_err == 0) { \
                for_each_split_arg = args_advance_token(for_each_split_arg_state); \
                if (for_each_split_arg->state == ARG_STATE_END) { \
                    LOG(":: end of loop values\n"); \
                    break; \
                } \
                if (for_each_split_arg->state == ARG_STATE_ERR) { \
                    /* error alrady reported*/ \
                    global_err = 1; \
                    break; \
                } \
                LOG(":: Storing ;"); \
                LOG(for_each_split_arg->arg); \
                LOG("' in "); \
                LOGLN(for_each_var_name); \
                LOG("'\n"); \
                global_err = setenv(for_each_var_name, for_each_split_arg->arg, 1); \
                if (global_err == 0) { \
                    LOG(":: Running loop commands.\n"); \
                    shared_split_input_argv[2] = global_arg; \
                    shared_split_arg_state = args_setup_tokenizer( \
                        3, (char **) shared_split_input_argv, \
                        /* This tells the tokeniser to use whatever the compile default*/ \
                        /* for env variables.*/ \
                        1); \
                    global_err = \
                        command_runner(shared_split_arg_state) + args_close_tokenizer(shared_split_arg_state); \
                } \
            } \
            global_err += args_close_tokenizer(for_each_split_arg_state); \
        break;
#define REQUIRES_ADDL_ARG__FOR_EACH \
            case COMMAND_INDEX__FOR_EACH: \
            case COMMAND_INDEX__FOR_EACH_IN:

#else /* USE_CMD_FOR_EACH */

#define ENUM_LIST__FOR_EACH
#define VIRTUAL_ENUM_LIST__FOR_EACH
#define GLOBAL_VARDEF__FOR_EACH
#define INITIALIZE__FOR_EACH
#define STARTUP_CASE__FOR_EACH
#define RUN_CASE__FOR_EACH
#define REQUIRES_ADDL_ARG__FOR_EACH
#endif /* USE_CMD_FOR_EACH */



#if defined(USE_CMD_WHILE_NO_ERROR) || defined(USE_CMD_WHILE_ERROR) || defined(USE_CMD_FOR_EACH)


        // Because we change the next command at the end,

        //      requiring another arg only applies for this

        //      one virtual step.

#define ENUM_LIST__LOOPS \
            ENUM_LIST__WHILE_NO_ERROR \
            ENUM_LIST__WHILE_ERROR \
            ENUM_LIST__FOR_EACH
#define VIRTUAL_ENUM_LIST__LOOPS \
            VIRTUAL_ENUM_LIST__WHILE_NO_ERROR \
            VIRTUAL_ENUM_LIST__WHILE_ERROR \
            VIRTUAL_ENUM_LIST__FOR_EACH \
            /* from cmd_loops.h.in:136 */ \
            COMMAND_INDEX__WHILE_LOOP,
#define GLOBAL_VARDEF__LOOPS \
            GLOBAL_VARDEF__WHILE_NO_ERROR \
            GLOBAL_VARDEF__WHILE_ERROR \
            GLOBAL_VARDEF__FOR_EACH
#define INITIALIZE__LOOPS \
            INITIALIZE__WHILE_NO_ERROR \
            INITIALIZE__WHILE_ERROR \
            INITIALIZE__FOR_EACH
#define STARTUP_CASE__LOOPS \
            STARTUP_CASE__WHILE_NO_ERROR \
            STARTUP_CASE__WHILE_ERROR \
            STARTUP_CASE__FOR_EACH
#define RUN_CASE__LOOPS \
            RUN_CASE__WHILE_NO_ERROR \
            RUN_CASE__WHILE_ERROR \
            RUN_CASE__FOR_EACH \
    case COMMAND_INDEX__WHILE_LOOP: \
        /* from cmd_loops.h.in:136 */ \
            /* from cmd_loops.h.in:141 */ \
            global_cmd = COMMAND_INDEX__ERR; \
            /* global_arg1_i == 0 for no-error*/ \
            /* Argument to test is global_arg_cached*/ \
            /* Argument to loop over is in global_arg*/ \
            tmp_val = global_arg1_i; \
            while (global_err == 0) { \
                LOG(":: Testing "); \
                LOGLN(global_arg_cached); \
                shared_split_input_argv[2] = global_arg_cached; \
                shared_split_arg_state = args_setup_tokenizer( \
                    3, (char **) shared_split_input_argv, \
                    /* This tells the tokeniser to use whatever the compile default*/ \
                    /* for env variables.*/ \
                    1); \
                tmp_val = \
                    command_runner(shared_split_arg_state) + args_close_tokenizer(shared_split_arg_state); \
                if ( \
                    (global_arg1_i == 0 && tmp_val != 0) \
                    || (global_arg1_i != 0 && tmp_val == 0) \
                ) { \
                    LOG(":: Test passed criteria.  Stopping loop.\n"); \
                    break; \
                } \
                LOG(":: Running body\n"); \
                shared_split_input_argv[2] = global_arg; \
                shared_split_arg_state = args_setup_tokenizer( \
                    3, (char **) shared_split_input_argv, \
                    /* This tells the tokeniser to use whatever the compile default*/ \
                    /* for env variables.*/ \
                    1); \
                global_err = \
                    command_runner(shared_split_arg_state) + args_close_tokenizer(shared_split_arg_state); \
            } \
        break;
#define REQUIRES_ADDL_ARG__LOOPS \
            REQUIRES_ADDL_ARG__WHILE_NO_ERROR \
            REQUIRES_ADDL_ARG__WHILE_ERROR \
            REQUIRES_ADDL_ARG__FOR_EACH \
            case COMMAND_INDEX__WHILE_LOOP:

#else /* defined(USE_CMD_WHILE_NO_ERROR) || defined(USE_CMD_WHILE_ERROR) || defined(USE_CMD_FOR_EACH) */

#define ENUM_LIST__LOOPS
#define VIRTUAL_ENUM_LIST__LOOPS
#define GLOBAL_VARDEF__LOOPS
#define INITIALIZE__LOOPS
#define STARTUP_CASE__LOOPS
#define RUN_CASE__LOOPS
#define REQUIRES_ADDL_ARG__LOOPS
#endif /* defined(USE_CMD_WHILE_NO_ERROR) || defined(USE_CMD_WHILE_ERROR) || defined(USE_CMD_FOR_EACH) */


#endif /* _FS_SHELL__CMD_LOOPS_ */
