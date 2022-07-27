// GENERATED FROM cmd_shared_virtual.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_SHARED_VIRTUAL_
#define _FS_SHELL__CMD_SHARED_VIRTUAL_


// Many commands reuse the same basic argument parsing functionality.
// This captures that.
//
// These assign a global variable the parsed argument, then redirect
// the next arg processing to what is set in global_arg3_i
//
// COMMAND_INDEX__SHARED_STR - store arg into global_arg
// COMMAND_INDEX__SHARED_INT - store arg into global_arg2_i

// Optional command shared_str

#ifdef USES_SHARED_STR




#define ENUM_LIST__SHARED_STR
#define VIRTUAL_ENUM_LIST__SHARED_STR \
            /* from cmd_shared_virtual.h.in:36 */ \
            COMMAND_INDEX__SHARED_STR,
#define GLOBAL_VARDEF__SHARED_STR
#define INITIALIZE__SHARED_STR
#define STARTUP_CASE__SHARED_STR
#define RUN_CASE__SHARED_STR \
    case COMMAND_INDEX__SHARED_STR: \
        /* from cmd_shared_virtual.h.in:36 */ \
            /* from cmd_shared_virtual.h.in:38 */ \
            LOG(":: storing "); \
            LOGLN(global_arg); \
            global_arg_cached = global_arg; \
            global_cmd = global_arg3_i; \
        break;
#define REQUIRES_ADDL_ARG__SHARED_STR \
            case COMMAND_INDEX__SHARED_STR:

#else /* USES_SHARED_STR */

#define ENUM_LIST__SHARED_STR
#define VIRTUAL_ENUM_LIST__SHARED_STR
#define GLOBAL_VARDEF__SHARED_STR
#define INITIALIZE__SHARED_STR
#define STARTUP_CASE__SHARED_STR
#define RUN_CASE__SHARED_STR
#define REQUIRES_ADDL_ARG__SHARED_STR
#endif /* USES_SHARED_STR */


// Optional command shared_int

#ifdef USES_SHARED_INT




#define ENUM_LIST__SHARED_INT
#define VIRTUAL_ENUM_LIST__SHARED_INT \
            /* from cmd_shared_virtual.h.in:51 */ \
            COMMAND_INDEX__SHARED_INT2,
#define GLOBAL_VARDEF__SHARED_INT
#define INITIALIZE__SHARED_INT
#define STARTUP_CASE__SHARED_INT
#define RUN_CASE__SHARED_INT \
    case COMMAND_INDEX__SHARED_INT2: \
        /* from cmd_shared_virtual.h.in:51 */ \
            /* from cmd_shared_virtual.h.in:53 */ \
            LOG(":: storing int "); \
            LOGLN(global_arg); \
            tmp_val = helper_arg_to_uint(global_arg, 10, 0xffff); \
            if (tmp_val < 0) { \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
                break; \
            } \
            global_arg2_i = tmp_val; \
            global_cmd = global_arg3_i; \
        break;
#define REQUIRES_ADDL_ARG__SHARED_INT \
            case COMMAND_INDEX__SHARED_INT2:

#else /* USES_SHARED_INT */

#define ENUM_LIST__SHARED_INT
#define VIRTUAL_ENUM_LIST__SHARED_INT
#define GLOBAL_VARDEF__SHARED_INT
#define INITIALIZE__SHARED_INT
#define STARTUP_CASE__SHARED_INT
#define RUN_CASE__SHARED_INT
#define REQUIRES_ADDL_ARG__SHARED_INT
#endif /* USES_SHARED_INT */


#if defined(USES_SHARED_STR) || defined(USES_SHARED_INT)

// Nothing extra here.

#define ENUM_LIST__SHARED_VIRTUAL \
            ENUM_LIST__SHARED_STR \
            ENUM_LIST__SHARED_INT
#define VIRTUAL_ENUM_LIST__SHARED_VIRTUAL \
            VIRTUAL_ENUM_LIST__SHARED_STR \
            VIRTUAL_ENUM_LIST__SHARED_INT
#define GLOBAL_VARDEF__SHARED_VIRTUAL \
            GLOBAL_VARDEF__SHARED_STR \
            GLOBAL_VARDEF__SHARED_INT
#define INITIALIZE__SHARED_VIRTUAL \
            INITIALIZE__SHARED_STR \
            INITIALIZE__SHARED_INT
#define STARTUP_CASE__SHARED_VIRTUAL \
            STARTUP_CASE__SHARED_STR \
            STARTUP_CASE__SHARED_INT
#define RUN_CASE__SHARED_VIRTUAL \
            RUN_CASE__SHARED_STR \
            RUN_CASE__SHARED_INT
#define REQUIRES_ADDL_ARG__SHARED_VIRTUAL \
            REQUIRES_ADDL_ARG__SHARED_STR \
            REQUIRES_ADDL_ARG__SHARED_INT

#else /* defined(USES_SHARED_STR) || defined(USES_SHARED_INT) */

#define ENUM_LIST__SHARED_VIRTUAL
#define VIRTUAL_ENUM_LIST__SHARED_VIRTUAL
#define GLOBAL_VARDEF__SHARED_VIRTUAL
#define INITIALIZE__SHARED_VIRTUAL
#define STARTUP_CASE__SHARED_VIRTUAL
#define RUN_CASE__SHARED_VIRTUAL
#define REQUIRES_ADDL_ARG__SHARED_VIRTUAL
#endif /* defined(USES_SHARED_STR) || defined(USES_SHARED_INT) */


#endif /* _FS_SHELL__CMD_SHARED_VIRTUAL_ */
