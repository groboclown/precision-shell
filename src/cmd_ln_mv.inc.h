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

#ifndef _FS_SHELL__CMD_LN_MV_

#define NAME__LN_S "ln-s"
#define NAME__LN_S__RUN ""
#define NAME__LN_H "ln-h"
#define NAME__LN_H__RUN ""
#define NAME__MV "mv"
#define NAME__MV__RUN ""

// No startup execution
#define STARTUP__COMMAND_INDEX__LN_S
#define STARTUP__COMMAND_INDEX__LN_S__RUN
#define STARTUP__COMMAND_INDEX__LN_H
#define STARTUP__COMMAND_INDEX__LN_H__RUN
#define STARTUP__COMMAND_INDEX__MV
#define STARTUP__COMMAND_INDEX__MV__RUN


#if defined(USE_CMD_LN_S) || defined(USE_CMD_LN_H) || defined(USE_CMD_MV)

// ln-s, ln-h, and mv have very similar logic.

#define CASE__COMMAND_INDEX__LN_MV_COMMON_BODY \
    LOG(":: ln/mv source "); \
    LOGLN(global_arg); \
    global_arg_cached = global_arg; \
    global_arg = args_advance_token(); \
    if (global_arg == NULL) { \
        /* No need to set cmd, because there will be no more arguments */ \
        global_err = 1; \
    } else

#define CASE__COMMAND_INDEX__LN_MV_COMMON_BODY_END \
    break;

#else /* defined(USE_CMD_LN_S) || defined(USE_CMD_LN_H) || defined(USE_CMD_MV) */
#define CASE__COMMAND_INDEX__LN_MV_COMMON_BODY
#define CASE__COMMAND_INDEX__LN_MV_COMMON_BODY_END
#endif /* defined(USE_CMD_LN_S) || defined(USE_CMD_LN_H) || defined(USE_CMD_MV) */


#ifdef USE_CMD_LN_S

#define CASE__COMMAND_INDEX__LN_S \
case COMMAND_INDEX__LN_S:

#define CASE__COMMAND_INDEX__LN_S_BODY \
    if (global_cmd == COMMAND_INDEX__LN_S) { \
        LOG(":: symlink "); \
        LOG(global_arg_cached); \
        LOG(" to "); \
        LOGLN(global_arg); \
        global_err = symlink(global_arg_cached, global_arg); \
    }

#else /* USE_CMD_LN_S */
#define CASE__COMMAND_INDEX__LN_S
#define CASE__COMMAND_INDEX__LN_S_BODY
#endif /* USE_CMD_LN_S */


#ifdef USE_CMD_LN_H

#define CASE__COMMAND_INDEX__LN_H \
case COMMAND_INDEX__LN_H:

#define CASE__COMMAND_INDEX__LN_H_BODY \
    if (global_cmd == COMMAND_INDEX__LN_H) { \
        LOG(":: hard link "); \
        LOG(global_arg_cached); \
        LOG(" to "); \
        LOGLN(global_arg); \
        global_err = link(global_arg_cached, global_arg); \
    }

#else /* USE_CMD_LN_H */
#define CASE__COMMAND_INDEX__LN_H
#define CASE__COMMAND_INDEX__LN_H_BODY
#endif /* USE_CMD_LN_H */


#ifdef USE_CMD_MV

#define CASE__COMMAND_INDEX__MV \
case COMMAND_INDEX__MV:

#define CASE__COMMAND_INDEX__MV_BODY \
    if (global_cmd == COMMAND_INDEX__MV) { \
        LOG(":: mv "); \
        LOG(global_arg_cached); \
        LOG(" to "); \
        LOGLN(global_arg); \
        global_err = rename(global_arg_cached, global_arg); \
    }

#else /* USE_CMD_MV */
#define CASE__COMMAND_INDEX__MV
#define CASE__COMMAND_INDEX__MV_BODY
#endif /* USE_CMD_MV */

#endif /* _FS_SHELL__CMD_LN_S_ */
