// GENERATED FROM cmd_while.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_WHILE_
#define _FS_SHELL__CMD_WHILE_


#include <stdlib.h>
#include "args.h"
#include "output.h"
#include "globals.h"
#include "command_runner.h"


// Optional command while-no-error

#ifdef USE_CMD_WHILE_NO_ERROR



/* from cmd_while.h.in:35 */
extern const char cmd_name_while_no_error[];
#define ENUM_LIST__WHILE_NO_ERROR \
            /* from cmd_while.h.in:35 */ \
            COMMAND_INDEX__WHILE_NO_ERROR,
#define VIRTUAL_ENUM_LIST__WHILE_NO_ERROR
#define GLOBAL_VARDEF__WHILE_NO_ERROR \
            /* from cmd_while.h.in:35 */ \
            const char cmd_name_while_no_error[] = "while-no-error";
#define INITIALIZE__WHILE_NO_ERROR \
            /* from cmd_while.h.in:35 */ \
            command_list_names[COMMAND_INDEX__WHILE_NO_ERROR] = cmd_name_while_no_error;
#define STARTUP_CASE__WHILE_NO_ERROR \
    case COMMAND_INDEX__WHILE_NO_ERROR: \
        /* from cmd_while.h.in:35 */ \
            /* from cmd_while.h.in:37 */ \
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


/* from cmd_while.h.in:48 */
extern const char cmd_name_while_error[];
#define ENUM_LIST__WHILE_ERROR \
            /* from cmd_while.h.in:48 */ \
            COMMAND_INDEX__WHILE_ERROR,
#define VIRTUAL_ENUM_LIST__WHILE_ERROR
#define GLOBAL_VARDEF__WHILE_ERROR \
            /* from cmd_while.h.in:48 */ \
            const char cmd_name_while_error[] = "while-error";
#define INITIALIZE__WHILE_ERROR \
            /* from cmd_while.h.in:48 */ \
            command_list_names[COMMAND_INDEX__WHILE_ERROR] = cmd_name_while_error;
#define STARTUP_CASE__WHILE_ERROR \
    case COMMAND_INDEX__WHILE_ERROR: \
        /* from cmd_while.h.in:48 */ \
            /* from cmd_while.h.in:50 */ \
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



#if defined(USE_CMD_WHILE_NO_ERROR) || defined(USE_CMD_WHILE_ERROR)


        // Because we change the next command at the end,

        //      requiring another arg only applies for this

        //      one virtual step.

#define ENUM_LIST__WHILE \
            ENUM_LIST__WHILE_NO_ERROR \
            ENUM_LIST__WHILE_ERROR
#define VIRTUAL_ENUM_LIST__WHILE \
            VIRTUAL_ENUM_LIST__WHILE_NO_ERROR \
            VIRTUAL_ENUM_LIST__WHILE_ERROR \
            /* from cmd_while.h.in:62 */ \
            COMMAND_INDEX__WHILE_LOOP,
#define GLOBAL_VARDEF__WHILE \
            GLOBAL_VARDEF__WHILE_NO_ERROR \
            GLOBAL_VARDEF__WHILE_ERROR
#define INITIALIZE__WHILE \
            INITIALIZE__WHILE_NO_ERROR \
            INITIALIZE__WHILE_ERROR
#define STARTUP_CASE__WHILE \
            STARTUP_CASE__WHILE_NO_ERROR \
            STARTUP_CASE__WHILE_ERROR
#define RUN_CASE__WHILE \
            RUN_CASE__WHILE_NO_ERROR \
            RUN_CASE__WHILE_ERROR \
    case COMMAND_INDEX__WHILE_LOOP: \
        /* from cmd_while.h.in:62 */ \
            /* from cmd_while.h.in:67 */ \
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
#define REQUIRES_ADDL_ARG__WHILE \
            REQUIRES_ADDL_ARG__WHILE_NO_ERROR \
            REQUIRES_ADDL_ARG__WHILE_ERROR \
            case COMMAND_INDEX__WHILE_LOOP:

#else /* defined(USE_CMD_WHILE_NO_ERROR) || defined(USE_CMD_WHILE_ERROR) */

#define ENUM_LIST__WHILE
#define VIRTUAL_ENUM_LIST__WHILE
#define GLOBAL_VARDEF__WHILE
#define INITIALIZE__WHILE
#define STARTUP_CASE__WHILE
#define RUN_CASE__WHILE
#define REQUIRES_ADDL_ARG__WHILE
#endif /* defined(USE_CMD_WHILE_NO_ERROR) || defined(USE_CMD_WHILE_ERROR) */


#endif /* _FS_SHELL__CMD_WHILE_ */
