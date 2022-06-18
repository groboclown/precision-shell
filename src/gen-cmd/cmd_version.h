// GENERATED FROM cmd_version.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_VERSION_
#define _FS_SHELL__CMD_VERSION_

// Required command version


#include "output.h"
#include "globals.h"
#include "helpers.h"


/* from cmd_version.h.in:30 */
extern const char cmd_name_version[];
#define ENUM_LIST__VERSION \
            /* from cmd_version.h.in:30 */ \
            COMMAND_INDEX__VERSION,
#define VIRTUAL_ENUM_LIST__VERSION
#define GLOBAL_VARDEF__VERSION \
            /* from cmd_version.h.in:30 */ \
            const char cmd_name_version[] = "version";
#define INITIALIZE__VERSION \
            /* from cmd_version.h.in:30 */ \
            command_list_names[COMMAND_INDEX__VERSION] = cmd_name_version;
#define STARTUP_CASE__VERSION \
    case COMMAND_INDEX__VERSION: \
        /* from cmd_version.h.in:30 */ \
            /* from cmd_version.h.in:31 */ \
            /* stdoutP(global_invoked_name);*/ \
            stdoutP("fs-shell"); \
            stdoutP(VERSION_STR); \
            for (tmp_val = 0; tmp_val < COMMAND_INDEX__LAST_NAMED_CMD; tmp_val++) { \
                /* Only output included commands that are not virtual.*/ \
                if (command_list_names[tmp_val][0] != 0) { \
                    stdoutP(" +"); \
                    stdoutP(command_list_names[tmp_val]); \
                } \
            } \
            stdoutP("\n"); \
            /* should be no arguments, so immediately switch to error mode.*/ \
            global_cmd = COMMAND_INDEX__ERR; \
        break;
#define RUN_CASE__VERSION
#define REQUIRES_ADDL_ARG__VERSION

#endif /* _FS_SHELL__CMD_VERSION_ */
