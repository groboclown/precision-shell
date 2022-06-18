// GENERATED FROM cmd_ln_h.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_LN_H_
#define _FS_SHELL__CMD_LN_H_

// Optional command ln-h

#ifdef USE_CMD_LN_H



/* from cmd_ln_h.h.in:26 */
extern const char cmd_name_ln_h[];
#define ENUM_LIST__LN_H \
            /* from cmd_ln_h.h.in:26 */ \
            COMMAND_INDEX__LN_H,
#define VIRTUAL_ENUM_LIST__LN_H
#define GLOBAL_VARDEF__LN_H \
            /* from cmd_ln_h.h.in:26 */ \
            const char cmd_name_ln_h[] = "ln-h";
#define INITIALIZE__LN_H \
            /* from cmd_ln_h.h.in:26 */ \
            command_list_names[COMMAND_INDEX__LN_H] = cmd_name_ln_h;
#define STARTUP_CASE__LN_H \
    case COMMAND_INDEX__LN_H: \
        /* from cmd_ln_h.h.in:26 */ \
            /* from cmd_ln_h.h.in:27 */ \
        /* Call the shared store to capture the next argument.*/ \
        global_cmd = COMMAND_INDEX__SHARED_STR; \
        global_arg3_i = COMMAND_INDEX__LN_H; \
        break;
#define RUN_CASE__LN_H \
    case COMMAND_INDEX__LN_H: \
        /* from cmd_ln_h.h.in:26 */ \
            /* from cmd_ln_h.h.in:33 */ \
        /* Called after the first argument was captured, and now*/ \
        /* on the second argument.*/ \
        LOG(":: hard link "); \
        LOG(global_arg_cached); \
        LOG(" to "); \
        LOGLN(global_arg); \
        global_err = link(global_arg_cached, global_arg); \
        /* Chould stop here, but this allows a long list of linking of the*/ \
        /*   same source to other places.*/ \
        break;
#define REQUIRES_ADDL_ARG__LN_H

#else /* USE_CMD_LN_H */

#define ENUM_LIST__LN_H
#define VIRTUAL_ENUM_LIST__LN_H
#define GLOBAL_VARDEF__LN_H
#define INITIALIZE__LN_H
#define STARTUP_CASE__LN_H
#define RUN_CASE__LN_H
#define REQUIRES_ADDL_ARG__LN_H
#endif /* USE_CMD_LN_H */

#endif /* _FS_SHELL__CMD_LN_H_ */
