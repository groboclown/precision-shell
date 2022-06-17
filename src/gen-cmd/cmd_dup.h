// GENERATED FROM cmd_dup.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_DUP_
#define _FS_SHELL__CMD_DUP_


// Optional command dup-a

#ifdef USE_CMD_DUP_A



/* from cmd_dup.h.in:26 */
extern const char cmd_name_dup_a__setup[];
#define ENUM_LIST__DUP_A \
            /* from cmd_dup.h.in:26 */ \
            COMMAND_INDEX__DUP_A__SETUP,
#define VIRTUAL_ENUM_LIST__DUP_A
#define GLOBAL_VARDEF__DUP_A \
            /* from cmd_dup.h.in:26 */ \
            const char cmd_name_dup_a__setup[] = "dup-a";
#define INITIALIZE__DUP_A \
            /* from cmd_dup.h.in:26 */ \
            command_list_names[COMMAND_INDEX__DUP_A__SETUP] = cmd_name_dup_a__setup;
#define STARTUP_CASE__DUP_A \
    case COMMAND_INDEX__DUP_A__SETUP: \
        /* from cmd_dup.h.in:26 */ \
            /* from cmd_dup.h.in:27 */ \
            LOG(":: preparing dup-a\n"); \
            global_arg1_i = O_WRONLY | O_CREAT | O_APPEND; \
            global_cmd = COMMAND_INDEX__SHARED_INT2; \
            /* shared int will call this index when it's done.*/ \
            global_arg3_i = COMMAND_INDEX__DUP__TGT; \
        break;
#define RUN_CASE__DUP_A

#else /* USE_CMD_DUP_A */

#define ENUM_LIST__DUP_A
#define VIRTUAL_ENUM_LIST__DUP_A
#define GLOBAL_VARDEF__DUP_A
#define INITIALIZE__DUP_A
#define STARTUP_CASE__DUP_A
#define RUN_CASE__DUP_A
#endif /* USE_CMD_DUP_A */


// Optional command dup-w

#ifdef USE_CMD_DUP_W



/* from cmd_dup.h.in:41 */
extern const char cmd_name_dup_w__setup[];
#define ENUM_LIST__DUP_W \
            /* from cmd_dup.h.in:41 */ \
            COMMAND_INDEX__DUP_W__SETUP,
#define VIRTUAL_ENUM_LIST__DUP_W
#define GLOBAL_VARDEF__DUP_W \
            /* from cmd_dup.h.in:41 */ \
            const char cmd_name_dup_w__setup[] = "dup-w";
#define INITIALIZE__DUP_W \
            /* from cmd_dup.h.in:41 */ \
            command_list_names[COMMAND_INDEX__DUP_W__SETUP] = cmd_name_dup_w__setup;
#define STARTUP_CASE__DUP_W \
    case COMMAND_INDEX__DUP_W__SETUP: \
        /* from cmd_dup.h.in:41 */ \
            /* from cmd_dup.h.in:42 */ \
            LOG(":: preparing dup-w\n"); \
            global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC; \
            global_cmd = COMMAND_INDEX__SHARED_INT2; \
            /* shared int will call this index when it's done.*/ \
            global_arg3_i = COMMAND_INDEX__DUP__TGT; \
        break;
#define RUN_CASE__DUP_W

#else /* USE_CMD_DUP_W */

#define ENUM_LIST__DUP_W
#define VIRTUAL_ENUM_LIST__DUP_W
#define GLOBAL_VARDEF__DUP_W
#define INITIALIZE__DUP_W
#define STARTUP_CASE__DUP_W
#define RUN_CASE__DUP_W
#endif /* USE_CMD_DUP_W */


// Optional command dup-r

#ifdef USE_CMD_DUP_R



/* from cmd_dup.h.in:56 */
extern const char cmd_name_dup_r__setup[];
#define ENUM_LIST__DUP_R \
            /* from cmd_dup.h.in:56 */ \
            COMMAND_INDEX__DUP_R__SETUP,
#define VIRTUAL_ENUM_LIST__DUP_R
#define GLOBAL_VARDEF__DUP_R \
            /* from cmd_dup.h.in:56 */ \
            const char cmd_name_dup_r__setup[] = "dup-r";
#define INITIALIZE__DUP_R \
            /* from cmd_dup.h.in:56 */ \
            command_list_names[COMMAND_INDEX__DUP_R__SETUP] = cmd_name_dup_r__setup;
#define STARTUP_CASE__DUP_R \
    case COMMAND_INDEX__DUP_R__SETUP: \
        /* from cmd_dup.h.in:56 */ \
            /* from cmd_dup.h.in:57 */ \
            LOG(":: preparing dup-w\n"); \
            global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC; \
            global_cmd = COMMAND_INDEX__SHARED_INT2; \
            /* shared int will call this index when it's done.*/ \
            global_arg3_i = COMMAND_INDEX__DUP__TGT; \
        break;
#define RUN_CASE__DUP_R

#else /* USE_CMD_DUP_R */

#define ENUM_LIST__DUP_R
#define VIRTUAL_ENUM_LIST__DUP_R
#define GLOBAL_VARDEF__DUP_R
#define INITIALIZE__DUP_R
#define STARTUP_CASE__DUP_R
#define RUN_CASE__DUP_R
#endif /* USE_CMD_DUP_R */


#if defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R)


#include <sys/stat.h>
#include <fcntl.h>

#include "output.h"
#include "globals.h"
#include "helpers.h"

    // Use the shared "COMMAND_INDEX__SHARED_INT"
    // to store the fd in global_arg2_i
    // In order to use this:
    //   - global_cmd = COMMAND_INDEX__SHARED_INT;
    //   - global_arg3_i = COMMAND_INDEX__DUP__TGT;



#define ENUM_LIST__DUP \
            ENUM_LIST__DUP_A \
            ENUM_LIST__DUP_W \
            ENUM_LIST__DUP_R
#define VIRTUAL_ENUM_LIST__DUP \
            VIRTUAL_ENUM_LIST__DUP_A \
            VIRTUAL_ENUM_LIST__DUP_W \
            VIRTUAL_ENUM_LIST__DUP_R \
            /* from cmd_dup.h.in:84 */ \
            COMMAND_INDEX__DUP__TGT,
#define GLOBAL_VARDEF__DUP \
            GLOBAL_VARDEF__DUP_A \
            GLOBAL_VARDEF__DUP_W \
            GLOBAL_VARDEF__DUP_R
#define INITIALIZE__DUP \
            INITIALIZE__DUP_A \
            INITIALIZE__DUP_W \
            INITIALIZE__DUP_R
#define STARTUP_CASE__DUP \
            STARTUP_CASE__DUP_A \
            STARTUP_CASE__DUP_W \
            STARTUP_CASE__DUP_R
#define RUN_CASE__DUP \
            RUN_CASE__DUP_A \
            RUN_CASE__DUP_W \
            RUN_CASE__DUP_R \
    case COMMAND_INDEX__DUP__TGT: \
        /* from cmd_dup.h.in:84 */ \
            /* from cmd_dup.h.in:85 */ \
            LOG(":: dup "); \
            if (strequal("&2", global_arg)) { \
                LOG(" stderr\n"); \
                global_arg3_i = STDERR_FILENO; \
            } else if (strequal("&1", global_arg)) { \
                LOG(" stdout\n"); \
                global_arg3_i = STDOUT_FILENO; \
            } else if (strequal("&0", global_arg)) { \
                LOG(" stdin\n"); \
                global_arg3_i = STDIN_FILENO; \
            } else { \
                LOGLN(global_arg); \
                global_arg3_i = open( \
                    global_arg, global_arg1_i, global_fmode \
                ); \
            } \
            if (global_arg3_i == -1) { \
                global_err = -1; \
            } else { \
                if (dup2(global_arg3_i, global_arg2_i) == -1) { \
                    global_err = 1; \
                } \
            } \
        break;

#else /* defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R) */

#define ENUM_LIST__DUP
#define VIRTUAL_ENUM_LIST__DUP
#define GLOBAL_VARDEF__DUP
#define INITIALIZE__DUP
#define STARTUP_CASE__DUP
#define RUN_CASE__DUP
#endif /* defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R) */

#endif /* _FS_SHELL__CMD_DUP_ */
