// GENERATED FROM cmd_touch_trunc.h.in.  DO NOT EDIT.

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

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, ETOUCHPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__CMD_TOUCH_TRUNC_
#define _FS_SHELL__CMD_TOUCH_TRUNC_


// Optional command touch

#ifdef USE_CMD_TOUCH

/* from cmd_touch_trunc.h.in:27 */
extern const char cmd_name_touch[];
#define ENUM_LIST__TOUCH \
            /* from cmd_touch_trunc.h.in:27 */ \
            COMMAND_INDEX__TOUCH,
#define VIRTUAL_ENUM_LIST__TOUCH
#define GLOBAL_VARDEF__TOUCH \
            /* from cmd_touch_trunc.h.in:27 */ \
            const char cmd_name_touch[] = "touch";
#define INITIALIZE__TOUCH \
            /* from cmd_touch_trunc.h.in:27 */ \
            command_list_names[COMMAND_INDEX__TOUCH] = cmd_name_touch;
#define STARTUP_CASE__TOUCH \
    case COMMAND_INDEX__TOUCH: \
        /* from cmd_touch_trunc.h.in:27 */ \
            /* from cmd_touch_trunc.h.in:28 */ \
            LOG(":: preparing touch\n"); \
            global_arg1_i = O_WRONLY | O_CREAT; \
            global_cmd = COMMAND_INDEX__TRUNC_TOUCH__RUN; \
        break;
#define RUN_CASE__TOUCH
#define REQUIRES_ADDL_ARG__TOUCH

#else /* USE_CMD_TOUCH */

#define ENUM_LIST__TOUCH
#define VIRTUAL_ENUM_LIST__TOUCH
#define GLOBAL_VARDEF__TOUCH
#define INITIALIZE__TOUCH
#define STARTUP_CASE__TOUCH
#define RUN_CASE__TOUCH
#define REQUIRES_ADDL_ARG__TOUCH
#endif /* USE_CMD_TOUCH */

// Optional command trunc

#ifdef USE_CMD_TRUNC

/* from cmd_touch_trunc.h.in:37 */
extern const char cmd_name_trunc[];
#define ENUM_LIST__TRUNC \
            /* from cmd_touch_trunc.h.in:37 */ \
            COMMAND_INDEX__TRUNC,
#define VIRTUAL_ENUM_LIST__TRUNC
#define GLOBAL_VARDEF__TRUNC \
            /* from cmd_touch_trunc.h.in:37 */ \
            const char cmd_name_trunc[] = "trunc";
#define INITIALIZE__TRUNC \
            /* from cmd_touch_trunc.h.in:37 */ \
            command_list_names[COMMAND_INDEX__TRUNC] = cmd_name_trunc;
#define STARTUP_CASE__TRUNC \
    case COMMAND_INDEX__TRUNC: \
        /* from cmd_touch_trunc.h.in:37 */ \
            /* from cmd_touch_trunc.h.in:38 */ \
            LOG(":: preparing trunc\n"); \
            global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC;; \
            global_cmd = COMMAND_INDEX__TRUNC_TOUCH__RUN; \
        break;
#define RUN_CASE__TRUNC
#define REQUIRES_ADDL_ARG__TRUNC

#else /* USE_CMD_TRUNC */

#define ENUM_LIST__TRUNC
#define VIRTUAL_ENUM_LIST__TRUNC
#define GLOBAL_VARDEF__TRUNC
#define INITIALIZE__TRUNC
#define STARTUP_CASE__TRUNC
#define RUN_CASE__TRUNC
#define REQUIRES_ADDL_ARG__TRUNC
#endif /* USE_CMD_TRUNC */

#if defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC)


#include <fcntl.h>



#define ENUM_LIST__TOUCH_TRUNC \
            ENUM_LIST__TOUCH \
            ENUM_LIST__TRUNC
#define VIRTUAL_ENUM_LIST__TOUCH_TRUNC \
            VIRTUAL_ENUM_LIST__TOUCH \
            VIRTUAL_ENUM_LIST__TRUNC \
            /* from cmd_touch_trunc.h.in:51 */ \
            COMMAND_INDEX__TRUNC_TOUCH__RUN,
#define GLOBAL_VARDEF__TOUCH_TRUNC \
            GLOBAL_VARDEF__TOUCH \
            GLOBAL_VARDEF__TRUNC
#define INITIALIZE__TOUCH_TRUNC \
            INITIALIZE__TOUCH \
            INITIALIZE__TRUNC
#define STARTUP_CASE__TOUCH_TRUNC \
            STARTUP_CASE__TOUCH \
            STARTUP_CASE__TRUNC
#define RUN_CASE__TOUCH_TRUNC \
            RUN_CASE__TOUCH \
            RUN_CASE__TRUNC \
    case COMMAND_INDEX__TRUNC_TOUCH__RUN: \
        /* from cmd_touch_trunc.h.in:51 */ \
            /* from cmd_touch_trunc.h.in:52 */ \
            LOG(":: touch/trunc "); \
            LOGLN(global_arg); \
            global_arg2_i = open( \
                global_arg, global_arg1_i, global_fmode \
            ); \
            if (global_arg2_i == -1) { \
                global_err = 1; \
            } else { \
                global_err = close(global_arg2_i); \
            } \
        break;
#define REQUIRES_ADDL_ARG__TOUCH_TRUNC \
            REQUIRES_ADDL_ARG__TOUCH \
            REQUIRES_ADDL_ARG__TRUNC

#else /* defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC) */

#define ENUM_LIST__TOUCH_TRUNC
#define VIRTUAL_ENUM_LIST__TOUCH_TRUNC
#define GLOBAL_VARDEF__TOUCH_TRUNC
#define INITIALIZE__TOUCH_TRUNC
#define STARTUP_CASE__TOUCH_TRUNC
#define RUN_CASE__TOUCH_TRUNC
#define REQUIRES_ADDL_ARG__TOUCH_TRUNC
#endif /* defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC) */


#endif /* _FS_SHELL__CMD_TOUCH_TRUNC_ */
