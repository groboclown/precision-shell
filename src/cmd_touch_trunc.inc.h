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

// touch and trunc can be used together to save space.

#if defined(USE_CMD_TOUCH) && defined(USE_CMD_TRUNC)

#define STARTUP__COMMAND_INDEX__TOUCH \
case COMMAND_INDEX__TOUCH: \
    global_arg1_i = O_WRONLY | O_CREAT;
    break;

#define STARTUP__COMMAND_INDEX__TRUNC \
case COMMAND_INDEX__TRUNC: \
    global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC;
    break;

#else

// Only defining one of these startups, because it covers both.  Note order.
#define STARTUP__COMMAND_INDEX__TOUCH
#define STARTUP__COMMAND_INDEX__TRUNC \
case COMMAND_INDEX__TOUCH: \
case COMMAND_INDEX__TRUNC: \
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

#ifdef USE_CMD_TOUCH

#define CASE__COMMAND_INDEX__TOUCH \
case COMMAND_INDEX__TOUCH: \
    LOG(":: touch "); \
    LOGLN(global_arg); \
    global_arg2_i = open( \
        global_arg, O_WRONLY | O_CREAT, global_fmode \
    ); \
    if (global_arg2_i == -1) { \
        global_err = 1; \
    } else { \
        close(global_arg2_i); \
    } \
    break;

#else /* USE_CMD_TOUCH */
#define CASE__COMMAND_INDEX__TOUCH
#endif /* USE_CMD_TOUCH */
#ifdef USE_CMD_TRUNC

#define CASE__COMMAND_INDEX__TRUNC \
case COMMAND_INDEX__TRUNC: \
    LOG(":: touch "); \
    LOGLN(global_arg); \
    global_arg2_i = open( \
        global_arg, O_WRONLY | O_CREAT | O_TRUNC, global_fmode \
    ); \
    if (global_arg2_i == -1) { \
        global_err = 1; \
    } else { \
        close(global_arg2_i); \
    } \
    break;

#else /* USE_CMD_TOUCH */
#define CASE__CMD_TRUNC
#endif /* USE_CMD_TRUNC */


#endif /* defined(USE_CMD_TOUCH) && defined(USE_CMD_TRUNC) */

#endif /* _FS_SHELL__CMD_TOUCH_ */
