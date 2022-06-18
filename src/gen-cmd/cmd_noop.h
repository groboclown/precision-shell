// GENERATED FROM cmd_noop.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_NOOP_
#define _FS_SHELL__CMD_NOOP_

// Optional command noop

#ifdef USE_CMD_NOOP



/* from cmd_noop.h.in:26 */
extern const char cmd_name_noop[];
#define ENUM_LIST__NOOP \
            /* from cmd_noop.h.in:26 */ \
            COMMAND_INDEX__NOOP,
#define VIRTUAL_ENUM_LIST__NOOP
#define GLOBAL_VARDEF__NOOP \
            /* from cmd_noop.h.in:26 */ \
            const char cmd_name_noop[] = "noop";
#define INITIALIZE__NOOP \
            /* from cmd_noop.h.in:26 */ \
            command_list_names[COMMAND_INDEX__NOOP] = cmd_name_noop;
#define STARTUP_CASE__NOOP
#define RUN_CASE__NOOP \
    case COMMAND_INDEX__NOOP: \
        /* from cmd_noop.h.in:26 */ \
            /* from cmd_noop.h.in:27 */ \
            LOG(":: ignore"); \
            LOGLN(global_arg); \
        break;
#define REQUIRES_ADDL_ARG__NOOP

#else /* USE_CMD_NOOP */

#define ENUM_LIST__NOOP
#define VIRTUAL_ENUM_LIST__NOOP
#define GLOBAL_VARDEF__NOOP
#define INITIALIZE__NOOP
#define STARTUP_CASE__NOOP
#define RUN_CASE__NOOP
#define REQUIRES_ADDL_ARG__NOOP
#endif /* USE_CMD_NOOP */

#endif /* _FS_SHELL__CMD_NOOP_ */
