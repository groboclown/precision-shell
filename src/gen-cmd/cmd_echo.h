// GENERATED FROM cmd_echo.h.in.  DO NOT EDIT.

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


#ifndef _FS_SHELL__CMD_ECHO_
#define _FS_SHELL__CMD_ECHO_

// Optional command echo

#ifdef USE_CMD_ECHO


#include "output.h"
#include "globals.h"
#include "helpers.h"


/* from cmd_echo.h.in:31 */
extern const char cmd_name_echo[];
#define ENUM_LIST__ECHO \
            /* from cmd_echo.h.in:31 */ \
            COMMAND_INDEX__ECHO,
#define VIRTUAL_ENUM_LIST__ECHO
#define GLOBAL_VARDEF__ECHO \
            /* from cmd_echo.h.in:31 */ \
            const char cmd_name_echo[] = "echo";
#define INITIALIZE__ECHO \
            /* from cmd_echo.h.in:31 */ \
            command_list_names[COMMAND_INDEX__ECHO] = cmd_name_echo;
#define STARTUP_CASE__ECHO
#define RUN_CASE__ECHO \
    case COMMAND_INDEX__ECHO: \
        /* from cmd_echo.h.in:31 */ \
            /* from cmd_echo.h.in:32 */ \
        LOG(":: echo\n"); \
        stdoutPLn(global_arg); \
        break;

#else /* USE_CMD_ECHO */

#define ENUM_LIST__ECHO
#define VIRTUAL_ENUM_LIST__ECHO
#define GLOBAL_VARDEF__ECHO
#define INITIALIZE__ECHO
#define STARTUP_CASE__ECHO
#define RUN_CASE__ECHO
#endif /* USE_CMD_ECHO */

#endif /* _FS_SHELL__CMD_ECHO_ */
