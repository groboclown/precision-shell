// GENERATED FROM cmd_not.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_NOT_
#define _FS_SHELL__CMD_NOT_

// Optional command not

#ifdef USE_CMD_NOT



#include <stdlib.h>
#include "args.h"
#include "output.h"
#include "globals.h"
#include "command_runner.h"


/* from cmd_not.h.in:33 */
extern const char cmd_name_not[];
#define ENUM_LIST__NOT \
            /* from cmd_not.h.in:33 */ \
            COMMAND_INDEX__NOT,
#define VIRTUAL_ENUM_LIST__NOT
#define GLOBAL_VARDEF__NOT \
            /* from cmd_not.h.in:33 */ \
            const char cmd_name_not[] = "not";
#define INITIALIZE__NOT \
            /* from cmd_not.h.in:33 */ \
            command_list_names[COMMAND_INDEX__NOT] = cmd_name_not;
#define STARTUP_CASE__NOT
#define RUN_CASE__NOT \
    case COMMAND_INDEX__NOT: \
        /* from cmd_not.h.in:33 */ \
            /* from cmd_not.h.in:35 */ \
        /* Split the arguments.*/ \
        shared_split_input_argv[2] = global_arg; \
        shared_split_arg_state = args_setup_tokenizer( \
            3, (char **) shared_split_input_argv, \
            /* Any way to force this to not parse env?*/ \
            1); \
        global_err = 0; \
        if (command_runner(shared_split_arg_state) == 0) { \
            global_err = 1; \
        } \
        global_err += args_close_tokenizer(shared_split_arg_state); \
        break;
#define REQUIRES_ADDL_ARG__NOT

#else /* USE_CMD_NOT */

#define ENUM_LIST__NOT
#define VIRTUAL_ENUM_LIST__NOT
#define GLOBAL_VARDEF__NOT
#define INITIALIZE__NOT
#define STARTUP_CASE__NOT
#define RUN_CASE__NOT
#define REQUIRES_ADDL_ARG__NOT
#endif /* USE_CMD_NOT */

#endif /* _FS_SHELL__CMD_NOT_ */
