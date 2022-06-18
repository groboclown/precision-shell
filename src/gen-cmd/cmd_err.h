// GENERATED FROM cmd_err.h.in.  DO NOT EDIT.

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
SOFTWARE. */

#ifndef _FS_SHELL__CMD_ERR_
#define _FS_SHELL__CMD_ERR_

// Required command err


#include "output.h"
#include "globals.h"
#include "helpers.h"



#define ENUM_LIST__ERR
#define VIRTUAL_ENUM_LIST__ERR \
            /* from cmd_err.h.in:28 */ \
            COMMAND_INDEX__ERR,
#define GLOBAL_VARDEF__ERR
#define INITIALIZE__ERR
#define STARTUP_CASE__ERR
#define RUN_CASE__ERR \
    case COMMAND_INDEX__ERR: \
        /* from cmd_err.h.in:28 */ \
            /* from cmd_err.h.in:29 */ \
        LOG(":: err arg "); \
        LOGLN(global_arg); \
        global_err = 1; \
        break;
#define REQUIRES_ADDL_ARG__ERR

#endif /* _FS_SHELL__CMD_ERR_ */
