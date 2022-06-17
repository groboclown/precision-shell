// GENERATED FROM cmd_mv.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_MV_
#define _FS_SHELL__CMD_MV_

// Optional command mv

#ifdef USE_CMD_MV



/* from cmd_mv.h.in:26 */
extern const char cmd_name_mv[];
#define ENUM_LIST__MV \
            /* from cmd_mv.h.in:26 */ \
            COMMAND_INDEX__MV,
#define VIRTUAL_ENUM_LIST__MV
#define GLOBAL_VARDEF__MV \
            /* from cmd_mv.h.in:26 */ \
            const char cmd_name_mv[] = "mv";
#define INITIALIZE__MV \
            /* from cmd_mv.h.in:26 */ \
            command_list_names[COMMAND_INDEX__MV] = cmd_name_mv;
#define STARTUP_CASE__MV \
    case COMMAND_INDEX__MV: \
        /* from cmd_mv.h.in:26 */ \
            /* from cmd_mv.h.in:27 */ \
        /* Call the shared store to capture the next argument.*/ \
        global_cmd = COMMAND_INDEX__SHARED_STR; \
        global_arg3_i = COMMAND_INDEX__MV; \
        break;
#define RUN_CASE__MV \
    case COMMAND_INDEX__MV: \
        /* from cmd_mv.h.in:26 */ \
            /* from cmd_mv.h.in:33 */ \
        /* Called after the first argument was captured, and now*/ \
        /* on the second argument.*/ \
        LOG(":: move "); \
        LOG(global_arg_cached); \
        LOG(" to "); \
        LOGLN(global_arg); \
        global_err = rename(global_arg_cached, global_arg); \
        /* No more arguments allowed.*/ \
        global_cmd = COMMAND_INDEX__ERR; \
        break;

#else /* USE_CMD_MV */

#define ENUM_LIST__MV
#define VIRTUAL_ENUM_LIST__MV
#define GLOBAL_VARDEF__MV
#define INITIALIZE__MV
#define STARTUP_CASE__MV
#define RUN_CASE__MV
#endif /* USE_CMD_MV */

#endif /* _FS_SHELL__CMD_MV_ */
