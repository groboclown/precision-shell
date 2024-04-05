// GENERATED FROM cmd_is_eq.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_IS_EQUAL_
#define _FS_SHELL__CMD_IS_EQUAL_

// Optional command is-equal

#ifdef USE_CMD_IS_EQUAL



#include "output.h"
#include "globals.h"


/* from cmd_is_eq.h.in:30 */
extern const char cmd_name_is_equal[];
#define ENUM_LIST__IS_EQUAL \
            /* from cmd_is_eq.h.in:30 */ \
            COMMAND_INDEX__IS_EQUAL,
#define VIRTUAL_ENUM_LIST__IS_EQUAL
#define GLOBAL_VARDEF__IS_EQUAL \
            /* from cmd_is_eq.h.in:30 */ \
            const char cmd_name_is_equal[] = "is-eq";
#define INITIALIZE__IS_EQUAL \
            /* from cmd_is_eq.h.in:30 */ \
            command_list_names[COMMAND_INDEX__IS_EQUAL] = cmd_name_is_equal;
#define STARTUP_CASE__IS_EQUAL \
    case COMMAND_INDEX__IS_EQUAL: \
        /* from cmd_is_eq.h.in:30 */ \
            /* from cmd_is_eq.h.in:32 */ \
        LOG(":: check string equality\n"); \
        /* Capture the first string*/ \
        global_cmd = COMMAND_INDEX__SHARED_STR; \
        /* Then each additional argument must equal the first.*/ \
        global_arg3_i = COMMAND_INDEX__IS_EQUAL; \
        break;
#define RUN_CASE__IS_EQUAL \
    case COMMAND_INDEX__IS_EQUAL: \
        /* from cmd_is_eq.h.in:30 */ \
            /* from cmd_is_eq.h.in:42 */ \
        /* String equality between the current argument and the captured argument.*/ \
        /* strcmp returns 0 if equal.*/ \
        global_err += strcmp(global_arg, global_arg_cached); \
        break;
#define REQUIRES_ADDL_ARG__IS_EQUAL

#else /* USE_CMD_IS_EQUAL */

#define ENUM_LIST__IS_EQUAL
#define VIRTUAL_ENUM_LIST__IS_EQUAL
#define GLOBAL_VARDEF__IS_EQUAL
#define INITIALIZE__IS_EQUAL
#define STARTUP_CASE__IS_EQUAL
#define RUN_CASE__IS_EQUAL
#define REQUIRES_ADDL_ARG__IS_EQUAL
#endif /* USE_CMD_IS_EQUAL */

#endif /* _FS_SHELL__CMD_IS_EQUAL_ */
