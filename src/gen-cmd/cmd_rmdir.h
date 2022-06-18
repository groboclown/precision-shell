// GENERATED FROM cmd_rmdir.h.in.  DO NOT EDIT.

/*
MIT License

Copyright (c) 2022 groboclown

Permdirission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permdirit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permdirission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, ERMDIRPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__CMD_RMDIR_
#define _FS_SHELL__CMD_RMDIR_

// Optional command rmdir

#ifdef USE_CMD_RMDIR


#include <stdio.h>


/* from cmd_rmdir.h.in:28 */
extern const char cmd_name_rmdir[];
#define ENUM_LIST__RMDIR \
            /* from cmd_rmdir.h.in:28 */ \
            COMMAND_INDEX__RMDIR,
#define VIRTUAL_ENUM_LIST__RMDIR
#define GLOBAL_VARDEF__RMDIR \
            /* from cmd_rmdir.h.in:28 */ \
            const char cmd_name_rmdir[] = "rmdir";
#define INITIALIZE__RMDIR \
            /* from cmd_rmdir.h.in:28 */ \
            command_list_names[COMMAND_INDEX__RMDIR] = cmd_name_rmdir;
#define STARTUP_CASE__RMDIR
#define RUN_CASE__RMDIR \
    case COMMAND_INDEX__RMDIR: \
        /* from cmd_rmdir.h.in:28 */ \
            /* from cmd_rmdir.h.in:29 */ \
            LOG(":: rmdir "); \
            LOGLN(global_arg); \
            global_err = rmdir(global_arg); \
        break;
#define REQUIRES_ADDL_ARG__RMDIR

#else /* USE_CMD_RMDIR */

#define ENUM_LIST__RMDIR
#define VIRTUAL_ENUM_LIST__RMDIR
#define GLOBAL_VARDEF__RMDIR
#define INITIALIZE__RMDIR
#define STARTUP_CASE__RMDIR
#define RUN_CASE__RMDIR
#define REQUIRES_ADDL_ARG__RMDIR
#endif /* USE_CMD_RMDIR */

#endif /* _FS_SHELL__CMD_RMDIR_ */
