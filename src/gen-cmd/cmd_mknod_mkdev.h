// GENERATED FROM cmd_mknod_mkdev.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_MKNOD_MKDEV_
#define _FS_SHELL__CMD_MKNOD_MKDEV_


// Optional command mknod

#ifdef USE_CMD_MKNOD



/* from cmd_mknod_mkdev.h.in:28 */
extern const char cmd_name_mknod_start[];
#define ENUM_LIST__MKNOD \
            /* from cmd_mknod_mkdev.h.in:28 */ \
            COMMAND_INDEX__MKNOD_START,
#define VIRTUAL_ENUM_LIST__MKNOD
#define GLOBAL_VARDEF__MKNOD \
            /* from cmd_mknod_mkdev.h.in:28 */ \
            const char cmd_name_mknod_start[] = "mknod";
#define INITIALIZE__MKNOD \
            /* from cmd_mknod_mkdev.h.in:28 */ \
            command_list_names[COMMAND_INDEX__MKNOD_START] = cmd_name_mknod_start;
#define STARTUP_CASE__MKNOD \
    case COMMAND_INDEX__MKNOD_START: \
        /* from cmd_mknod_mkdev.h.in:28 */ \
            /* from cmd_mknod_mkdev.h.in:29 */ \
            LOG(":: mknod setup\n"); \
            global_cmd = COMMAND_INDEX__MKNOD_DEV__TYPE; \
            global_arg1_i = 0; \
        break;
#define RUN_CASE__MKNOD
#define REQUIRES_ADDL_ARG__MKNOD

#else /* USE_CMD_MKNOD */

#define ENUM_LIST__MKNOD
#define VIRTUAL_ENUM_LIST__MKNOD
#define GLOBAL_VARDEF__MKNOD
#define INITIALIZE__MKNOD
#define STARTUP_CASE__MKNOD
#define RUN_CASE__MKNOD
#define REQUIRES_ADDL_ARG__MKNOD
#endif /* USE_CMD_MKNOD */


// Optional command mkdev

#ifdef USE_CMD_MKDEV


#include <sys/sysmacros.h>


/* from cmd_mknod_mkdev.h.in:43 */
extern const char cmd_name_mkdev_start[];
#define ENUM_LIST__MKDEV \
            /* from cmd_mknod_mkdev.h.in:43 */ \
            COMMAND_INDEX__MKDEV_START,
#define VIRTUAL_ENUM_LIST__MKDEV \
            /* from cmd_mknod_mkdev.h.in:53 */ \
            COMMAND_INDEX__MKDEV__DEVICE,
#define GLOBAL_VARDEF__MKDEV \
            /* from cmd_mknod_mkdev.h.in:43 */ \
            const char cmd_name_mkdev_start[] = "mkdev";
#define INITIALIZE__MKDEV \
            /* from cmd_mknod_mkdev.h.in:43 */ \
            command_list_names[COMMAND_INDEX__MKDEV_START] = cmd_name_mkdev_start;
#define STARTUP_CASE__MKDEV \
    case COMMAND_INDEX__MKDEV_START: \
        /* from cmd_mknod_mkdev.h.in:43 */ \
            /* from cmd_mknod_mkdev.h.in:44 */ \
            LOG(":: mkdev setup\n"); \
            /* Call the shared store to capture the next argument.*/ \
            /* Which is the major device number*/ \
            global_cmd = COMMAND_INDEX__SHARED_INT2; \
            global_arg3_i = COMMAND_INDEX__MKDEV__DEVICE; \
        break;
#define RUN_CASE__MKDEV \
    case COMMAND_INDEX__MKDEV__DEVICE: \
        /* from cmd_mknod_mkdev.h.in:53 */ \
            /* from cmd_mknod_mkdev.h.in:56 */ \
            /* Just read in the major version into arg2.*/ \
            /* This arg is the minor version.*/ \
            LOG(":: mkdev minor device "); \
            LOGLN(global_arg); \
            global_cmd = COMMAND_INDEX__MKNOD_DEV__TYPE; \
            global_arg3_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
            if (global_arg3_i == -1) { \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
            } else { \
                global_arg1_i = makedev(global_arg2_i, global_arg3_i); \
                if (global_arg1_i == -1) { \
                    global_err = 1; \
                    global_cmd = COMMAND_INDEX__ERR; \
                } \
            } \
        break;
#define REQUIRES_ADDL_ARG__MKDEV \
            case COMMAND_INDEX__MKDEV__DEVICE:

#else /* USE_CMD_MKDEV */

#define ENUM_LIST__MKDEV
#define VIRTUAL_ENUM_LIST__MKDEV
#define GLOBAL_VARDEF__MKDEV
#define INITIALIZE__MKDEV
#define STARTUP_CASE__MKDEV
#define RUN_CASE__MKDEV
#define REQUIRES_ADDL_ARG__MKDEV
#endif /* USE_CMD_MKDEV */


#if defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV)


#include <fcntl.h>
#include <sys/stat.h>




#define ENUM_LIST__MKNOD_MKDEV \
            ENUM_LIST__MKNOD \
            ENUM_LIST__MKDEV
#define VIRTUAL_ENUM_LIST__MKNOD_MKDEV \
            VIRTUAL_ENUM_LIST__MKNOD \
            VIRTUAL_ENUM_LIST__MKDEV \
            /* from cmd_mknod_mkdev.h.in:83 */ \
            COMMAND_INDEX__MKNOD_DEV__TYPE, \
            /* from cmd_mknod_mkdev.h.in:109 */ \
            COMMAND_INDEX__MKNOD_DEV__RUN,
#define GLOBAL_VARDEF__MKNOD_MKDEV \
            GLOBAL_VARDEF__MKNOD \
            GLOBAL_VARDEF__MKDEV
#define INITIALIZE__MKNOD_MKDEV \
            INITIALIZE__MKNOD \
            INITIALIZE__MKDEV
#define STARTUP_CASE__MKNOD_MKDEV \
            STARTUP_CASE__MKNOD \
            STARTUP_CASE__MKDEV
#define RUN_CASE__MKNOD_MKDEV \
            RUN_CASE__MKNOD \
            RUN_CASE__MKDEV \
    case COMMAND_INDEX__MKNOD_DEV__TYPE: \
        /* from cmd_mknod_mkdev.h.in:83 */ \
            /* from cmd_mknod_mkdev.h.in:86 */ \
            LOG(":: create node with device type "); \
            LOGLN(global_arg); \
            global_cmd = COMMAND_INDEX__MKNOD_DEV__RUN; \
            /* just look at the first character*/ \
            global_arg3_i = global_arg[0]; \
            global_arg2_i = global_fmode; \
            if (global_arg3_i == 'c' || global_arg3_i == 'u') { \
                    global_arg2_i |= S_IFCHR; \
            } else if (global_arg3_i == 'b') { \
                global_arg2_i |= S_IFBLK; \
            } else if (global_arg3_i == 's') { \
                global_arg2_i |= S_IFSOCK; \
            } else if (global_arg3_i == 'p') { \
                global_arg2_i |= S_IFIFO; \
            } else { \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
            } \
        break; \
    case COMMAND_INDEX__MKNOD_DEV__RUN: \
        /* from cmd_mknod_mkdev.h.in:109 */ \
            /* from cmd_mknod_mkdev.h.in:110 */ \
            LOG(":: mknod/dev "); \
            LOGLN(global_arg); \
            /* target file, mode, device*/ \
            global_err = mknod(global_arg, global_arg2_i, global_arg1_i); \
            /* Could include*/ \
            /* global_cmd = COMMAND_INDEX__ERR;*/ \
            /* to make this create only one file, but let it*/ \
            /* create as many as the user wants.  It actually makes the code smaller*/ \
            /* while giving more behavior.*/ \
        break;
#define REQUIRES_ADDL_ARG__MKNOD_MKDEV \
            REQUIRES_ADDL_ARG__MKNOD \
            REQUIRES_ADDL_ARG__MKDEV \
            case COMMAND_INDEX__MKNOD_DEV__TYPE:

#else /* defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV) */

#define ENUM_LIST__MKNOD_MKDEV
#define VIRTUAL_ENUM_LIST__MKNOD_MKDEV
#define GLOBAL_VARDEF__MKNOD_MKDEV
#define INITIALIZE__MKNOD_MKDEV
#define STARTUP_CASE__MKNOD_MKDEV
#define RUN_CASE__MKNOD_MKDEV
#define REQUIRES_ADDL_ARG__MKNOD_MKDEV
#endif /* defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV) */


#endif /* _FS_SHELL__CMD_MKNOD_MKDEV_ */
