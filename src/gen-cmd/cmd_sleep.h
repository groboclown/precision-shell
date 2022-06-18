// GENERATED FROM cmd_sleep.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_SLEEP_
#define _FS_SHELL__CMD_SLEEP_

// Optional command sleep

#ifdef USE_CMD_SLEEP



/* from cmd_sleep.h.in:26 */
extern const char cmd_name_sleep[];
#define ENUM_LIST__SLEEP \
            /* from cmd_sleep.h.in:26 */ \
            COMMAND_INDEX__SLEEP,
#define VIRTUAL_ENUM_LIST__SLEEP
#define GLOBAL_VARDEF__SLEEP \
            /* from cmd_sleep.h.in:26 */ \
            const char cmd_name_sleep[] = "sleep";
#define INITIALIZE__SLEEP \
            /* from cmd_sleep.h.in:26 */ \
            command_list_names[COMMAND_INDEX__SLEEP] = cmd_name_sleep;
#define STARTUP_CASE__SLEEP
#define RUN_CASE__SLEEP \
    case COMMAND_INDEX__SLEEP: \
        /* from cmd_sleep.h.in:26 */ \
            /* from cmd_sleep.h.in:28 */ \
            LOG(":: sleep "); \
            LOGLN(global_arg); \
            global_arg1_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
            if (global_arg1_i < 0) { \
                global_err = 1; \
            } else if (global_arg1_i > 0) { \
                sleep(global_arg1_i); \
            } \
        break;
#define REQUIRES_ADDL_ARG__SLEEP

#else /* USE_CMD_SLEEP */

#define ENUM_LIST__SLEEP
#define VIRTUAL_ENUM_LIST__SLEEP
#define GLOBAL_VARDEF__SLEEP
#define INITIALIZE__SLEEP
#define STARTUP_CASE__SLEEP
#define RUN_CASE__SLEEP
#define REQUIRES_ADDL_ARG__SLEEP
#endif /* USE_CMD_SLEEP */

#endif /* _FS_SHELL__CMD_SLEEP_ */
