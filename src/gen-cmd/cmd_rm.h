// GENERATED FROM cmd_rm.h.in.  DO NOT EDIT.

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

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, ERMPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__CMD_RM_
#define _FS_SHELL__CMD_RM_

// Optional command rm

#ifdef USE_CMD_RM


#include <stdio.h>


/* from cmd_rm.h.in:28 */
extern const char cmd_name_rm[];
#define ENUM_LIST__RM \
            /* from cmd_rm.h.in:28 */ \
            COMMAND_INDEX__RM,
#define VIRTUAL_ENUM_LIST__RM
#define GLOBAL_VARDEF__RM \
            /* from cmd_rm.h.in:28 */ \
            const char cmd_name_rm[] = "rm";
#define INITIALIZE__RM \
            /* from cmd_rm.h.in:28 */ \
            command_list_names[COMMAND_INDEX__RM] = cmd_name_rm;
#define STARTUP_CASE__RM
#define RUN_CASE__RM \
    case COMMAND_INDEX__RM: \
        /* from cmd_rm.h.in:28 */ \
            /* from cmd_rm.h.in:29 */ \
            LOG(":: rm "); \
            LOGLN(global_arg); \
            global_err = unlink(global_arg); \
        break;
#define REQUIRES_ADDL_ARG__RM

#else /* USE_CMD_RM */

#define ENUM_LIST__RM
#define VIRTUAL_ENUM_LIST__RM
#define GLOBAL_VARDEF__RM
#define INITIALIZE__RM
#define STARTUP_CASE__RM
#define RUN_CASE__RM
#define REQUIRES_ADDL_ARG__RM
#endif /* USE_CMD_RM */

#endif /* _FS_SHELL__CMD_RM_ */
