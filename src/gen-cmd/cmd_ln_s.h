// GENERATED FROM cmd_ln_s.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_LN_S_
#define _FS_SHELL__CMD_LN_S_

// Optional command ln-s

#ifdef USE_CMD_LN_S



/* from cmd_ln_s.h.in:26 */
extern const char cmd_name_ln_s[];
#define ENUM_LIST__LN_S \
            /* from cmd_ln_s.h.in:26 */ \
            COMMAND_INDEX__LN_S,
#define VIRTUAL_ENUM_LIST__LN_S
#define GLOBAL_VARDEF__LN_S \
            /* from cmd_ln_s.h.in:26 */ \
            const char cmd_name_ln_s[] = "ln-s";
#define INITIALIZE__LN_S \
            /* from cmd_ln_s.h.in:26 */ \
            command_list_names[COMMAND_INDEX__LN_S] = cmd_name_ln_s;
#define STARTUP_CASE__LN_S \
    case COMMAND_INDEX__LN_S: \
        /* from cmd_ln_s.h.in:26 */ \
            /* from cmd_ln_s.h.in:27 */ \
        /* Call the shared store to capture the next argument.*/ \
        global_cmd = COMMAND_INDEX__SHARED_STR; \
        global_arg3_i = COMMAND_INDEX__LN_S; \
        break;
#define RUN_CASE__LN_S \
    case COMMAND_INDEX__LN_S: \
        /* from cmd_ln_s.h.in:26 */ \
            /* from cmd_ln_s.h.in:33 */ \
        /* Called after the first argument was captured, and now*/ \
        /* on the second argument.*/ \
        LOG(":: symlink "); \
        LOG(global_arg_cached); \
        LOG(" to "); \
        LOGLN(global_arg); \
        global_err = symlink(global_arg_cached, global_arg); \
        /* No more arguments allowed.*/ \
        global_cmd = COMMAND_INDEX__ERR; \
        break;

#else /* USE_CMD_LN_S */

#define ENUM_LIST__LN_S
#define VIRTUAL_ENUM_LIST__LN_S
#define GLOBAL_VARDEF__LN_S
#define INITIALIZE__LN_S
#define STARTUP_CASE__LN_S
#define RUN_CASE__LN_S
#endif /* USE_CMD_LN_S */

#endif /* _FS_SHELL__CMD_LN_S_ */
