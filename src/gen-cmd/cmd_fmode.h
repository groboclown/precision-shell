// GENERATED FROM cmd_fmode.h.in.  DO NOT EDIT.

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

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EFMODEPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__CMD_FMODE_
#define _FS_SHELL__CMD_FMODE_

// Optional command fmode

#ifdef USES_FMODE



/* from cmd_fmode.h.in:26 */
extern const char cmd_name_fmode[];
#define ENUM_LIST__FMODE \
            /* from cmd_fmode.h.in:26 */ \
            COMMAND_INDEX__FMODE,
#define VIRTUAL_ENUM_LIST__FMODE
#define GLOBAL_VARDEF__FMODE \
            /* from cmd_fmode.h.in:26 */ \
            const char cmd_name_fmode[] = "fmode";
#define INITIALIZE__FMODE \
            /* from cmd_fmode.h.in:26 */ \
            command_list_names[COMMAND_INDEX__FMODE] = cmd_name_fmode; \
            /* from cmd_fmode.h.in:27 */ \
        /* Including the fmode command creates a shared variable,*/ \
        /* with a default value.*/ \
        int global_fmode = 0644;
#define STARTUP_CASE__FMODE
#define RUN_CASE__FMODE \
    case COMMAND_INDEX__FMODE: \
        /* from cmd_fmode.h.in:26 */ \
            /* from cmd_fmode.h.in:33 */ \
        LOG(":: fmode "); \
        LOGLN(global_arg); \
        /* File mode is only up to the first 3 nybbles.*/ \
        /* Due to error checking, this will not change fmode unless it's okay.*/ \
        tmp_val = helper_arg_to_uint(global_arg, 8, 0777); \
        if (tmp_val < 0) { \
            global_err = 1; \
            break; \
        } \
        global_fmode = tmp_val; \
        /* No other args will be processed.*/ \
        global_cmd = COMMAND_INDEX__ERR; \
        break;
#define REQUIRES_ADDL_ARG__FMODE

#else /* USES_FMODE */

#define ENUM_LIST__FMODE
#define VIRTUAL_ENUM_LIST__FMODE
#define GLOBAL_VARDEF__FMODE
#define INITIALIZE__FMODE
#define STARTUP_CASE__FMODE
#define RUN_CASE__FMODE
#define REQUIRES_ADDL_ARG__FMODE
#endif /* USES_FMODE */

#endif /* _FS_SHELL__CMD_FMODE_ */
