// GENERATED FROM cmd_shared_sub_args.h.in.  DO NOT EDIT.

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
SOFTWARE.
*/

#ifndef _FS_SHELL__CMD_SHARED_SUB_ARGS_
#define _FS_SHELL__CMD_SHARED_SUB_ARGS_

// Optional command shared-sub-args

#ifdef USES_SHARED_SPLIT_ARG


// Some commands need to perform splitting of a single argument into
// their own arguments.  This sets up shared variables for that.

#include <stdlib.h>
#include <string.h>
#include "args.h"


// Number of arguments for parsing
#ifndef MAX_SUB_ARG_COUNT
#define MAX_SUB_ARG_COUNT 100
#endif

// Total length can't exceed the source argument length: PARSED_ARG_SIZE + 1

// This sets up the parsed argument into shared_split_argv
// The number of arguments is in global_arg3_i;
#define SHARED_SPLIT__PARSE_ARG \
    shared_split_input_argv[2] = global_arg; \
    shared_split_arg_state = args_setup_tokenizer(3, (char **) shared_split_input_argv, 0); \
    if (shared_split_arg_state == NULL) { \
        global_err = 1; \
        break; \
    } \
    global_arg3_i = 0; \
    tmp_val = 0; \
    while (1 == 1) { \
        shared_split_arg = args_advance_token(shared_split_arg_state); \
        if (shared_split_arg->state == ARG_STATE_END) { \
            break; \
        } else if (shared_split_arg->state == ARG_STATE_ERR) {\
            global_err = 1; \
            break; \
        } else {\
            LOG(":: Parsed sub-arg '"); \
            LOG(shared_split_arg->arg); \
            LOG("'\n"); \
            shared_split_argv[global_arg3_i++] = &(shared_split_argv_buffer[tmp_val]); \
            strcpy(&(shared_split_argv_buffer[tmp_val]), shared_split_arg->arg); \
            tmp_val += strlen(shared_split_arg->arg) + 1; \
        } \
    } \
    shared_split_argv[global_arg3_i] = NULL; \
    global_err += args_close_tokenizer(shared_split_arg_state);




#define ENUM_LIST__SHARED_SUB_ARGS
#define VIRTUAL_ENUM_LIST__SHARED_SUB_ARGS
#define GLOBAL_VARDEF__SHARED_SUB_ARGS
#define INITIALIZE__SHARED_SUB_ARGS \
            /* from cmd_shared_sub_args.h.in:71 */ \
        struct ArgState *shared_split_arg_state; \
        const char *shared_split_input_argv[] = { NULL, "-c", NULL }; \
        const Argument *shared_split_arg; \
        /* These are allocated on the stack, which probably isn't right.*/ \
        char *shared_split_argv[MAX_SUB_ARG_COUNT + 1]; \
        char shared_split_argv_buffer[PARSED_ARG_SIZE + 1];
#define STARTUP_CASE__SHARED_SUB_ARGS
#define RUN_CASE__SHARED_SUB_ARGS
#define REQUIRES_ADDL_ARG__SHARED_SUB_ARGS

#else /* USES_SHARED_SPLIT_ARG */

#define ENUM_LIST__SHARED_SUB_ARGS
#define VIRTUAL_ENUM_LIST__SHARED_SUB_ARGS
#define GLOBAL_VARDEF__SHARED_SUB_ARGS
#define INITIALIZE__SHARED_SUB_ARGS
#define STARTUP_CASE__SHARED_SUB_ARGS
#define RUN_CASE__SHARED_SUB_ARGS
#define REQUIRES_ADDL_ARG__SHARED_SUB_ARGS
#endif /* USES_SHARED_SPLIT_ARG */

#endif /* _FS_SHELL__CMD_SHARED_SUB_ARGS_ */
