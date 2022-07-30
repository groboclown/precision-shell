// GENERATED FROM cmd_cd.h.in.  DO NOT EDIT.

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


#ifndef _FS_SHELL__CMD_CD_
#define _FS_SHELL__CMD_CD_

// Optional command cd

#ifdef USE_CMD_CD


#include <unistd.h>
#include "globals.h"
#include "helpers.h"


/* from cmd_cd.h.in:31 */
extern const char cmd_name_cd[];
#define ENUM_LIST__CD \
            /* from cmd_cd.h.in:31 */ \
            COMMAND_INDEX__CD,
#define VIRTUAL_ENUM_LIST__CD
#define GLOBAL_VARDEF__CD \
            /* from cmd_cd.h.in:31 */ \
            const char cmd_name_cd[] = "cd";
#define INITIALIZE__CD \
            /* from cmd_cd.h.in:31 */ \
            command_list_names[COMMAND_INDEX__CD] = cmd_name_cd;
#define STARTUP_CASE__CD
#define RUN_CASE__CD \
    case COMMAND_INDEX__CD: \
        /* from cmd_cd.h.in:31 */ \
            /* from cmd_cd.h.in:32 */ \
        LOG(":: cd "); \
        LOGLN(global_arg); \
        global_err = chdir(global_arg); \
        break;
#define REQUIRES_ADDL_ARG__CD

#else /* USE_CMD_CD */

#define ENUM_LIST__CD
#define VIRTUAL_ENUM_LIST__CD
#define GLOBAL_VARDEF__CD
#define INITIALIZE__CD
#define STARTUP_CASE__CD
#define RUN_CASE__CD
#define REQUIRES_ADDL_ARG__CD
#endif /* USE_CMD_CD */

#endif /* _FS_SHELL__CMD_CD_ */
