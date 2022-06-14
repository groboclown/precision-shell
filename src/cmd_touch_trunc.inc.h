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

#ifndef _FS_SHELL__CMD_TOUCH_

// Execution is not defined

#define CASE__COMMAND_INDEX__TOUCH
#define CASE__COMMAND_INDEX__TRUNC

// This one has no setup
#define STARTUP__COMMAND_INDEX__TRUNC_TOUCH__RUN


#ifdef USE_CMD_TOUCH

#define STARTUP__COMMAND_INDEX__TOUCH \
case COMMAND_INDEX__TOUCH: \
    LOG(":: preparing touch\n"); \
    global_arg1_i = O_WRONLY | O_CREAT; \
    global_cmd = COMMAND_INDEX__TRUNC_TOUCH__RUN; \
    break;

#else /* USE_CMD_TOUCH */
#define STARTUP__COMMAND_INDEX__TOUCH
#endif /* USE_CMD_TOUCH */


#ifdef USE_CMD_TRUNC

#define STARTUP__COMMAND_INDEX__TRUNC \
case COMMAND_INDEX__TRUNC: \
    LOG(":: preparing trunc\n"); \
    global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC; \
    global_cmd = COMMAND_INDEX__TRUNC_TOUCH__RUN; \
    break;

#else /* USE_CMD_TRUNC */
#define STARTUP__COMMAND_INDEX__TRUNC
#endif /* USE_CMD_TRUNC */


#if defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC)
#include <fcntl.h>

// Only defining one of these startups, because it covers both.  Note order.
#define CASE__COMMAND_INDEX__TRUNC_TOUCH__RUN \
case COMMAND_INDEX__TRUNC_TOUCH__RUN: \
    LOG(":: touch/trunc "); \
    LOGLN(global_arg); \
    global_arg2_i = open( \
        global_arg, global_arg1_i, global_fmode \
    ); \
    if (global_arg2_i == -1) { \
        global_err = 1; \
    } else { \
        close(global_arg2_i); \
    } \
    break;

#else /* defined(USE_CMD_TOUCH) && defined(USE_CMD_TRUNC) */
#define CASE__COMMAND_INDEX__TRUNC_TOUCH__RUN
#endif /* defined(USE_CMD_TOUCH) && defined(USE_CMD_TRUNC) */

#endif /* _FS_SHELL__CMD_TOUCH_ */
