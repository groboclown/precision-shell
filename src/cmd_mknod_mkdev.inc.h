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

// mknod and mkdev work with shared data to make them compiled together more efficient.

// mknod itself doesn't have any execution for mknod; instead,
// it sets up the parameters to run directly into COMMAND_INDEX__MKNOD_DEV__TYPE
#define CASE__COMMAND_INDEX__MKNOD

// the startup for the virtual commands can;t ever run.
#define STARTUP__COMMAND_INDEX__MKDEV__MINOR
#define STARTUP__COMMAND_INDEX__MKNOD_DEV__TYPE
#define STARTUP__COMMAND_INDEX__MKNOD_DEV__RUN

// this one has a follow-up command that means we don't need the startup
#define STARTUP__COMMAND_INDEX__MKDEV



#ifdef USE_CMD_MKNOD

#define STARTUP__COMMAND_INDEX__MKNOD \
case COMMAND_INDEX__MKNOD: \
    LOG(":: mknod setup\n"); \
    global_cmd = COMMAND_INDEX__MKNOD_DEV__TYPE; \
    global_arg1_i = 0; \
    break;

#else /* USE_CMD_MKNOD */
#define STARTUP__COMMAND_INDEX__MKNOD
#define CASE__COMMAND_INDEX__MKNOD
#endif /* USE_CMD_MKNOD */


#ifdef USE_CMD_MKDEV
#include <sys/sysmacros.h>

// Store up the read-in major/minor in arg2 and arg3, then
// store the device number in shared arg1 for the next __type call.
#define CASE__COMMAND_INDEX__MKDEV \
case COMMAND_INDEX__MKDEV: \
    LOG(":: mkdev major device "); \
    LOGLN(global_arg); \
    global_cmd = COMMAND_INDEX__MKDEV__MINOR; \
    global_arg2_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
    if (global_arg2_i == -1) { \
        global_err = 1; \
        global_cmd = COMMAND_INDEX__ERR; \
    } \
    break;

#define CASE__COMMAND_INDEX__MKDEV__MINOR \
case COMMAND_INDEX__MKDEV__MINOR: \
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


#else /* USE_CMD_MKDEV */
#define CASE__COMMAND_INDEX__MKDEV
#define CASE__COMMAND_INDEX__MKDEV__MINOR
#endif /* USE_CMD_MKDEV */


// Shared logic
#ifdef USES_MKNOD

#include <sys/stat.h>

// They use global_arg1_i to store the device number
// Then global_arg2_i will store the node mode
#define CASE__COMMAND_INDEX__MKNOD_DEV__TYPE \
case COMMAND_INDEX__MKNOD_DEV__TYPE: \
    LOG(":: create node with device type "); \
    LOGLN(global_arg); \
    global_cmd = COMMAND_INDEX__MKNOD_DEV__RUN; \
    \
    /* just look at the first character */ \
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
    break;

#define CASE__COMMAND_INDEX__MKNOD_DEV__RUN \
case COMMAND_INDEX__MKNOD_DEV__RUN: \
    LOG(":: mknod/dev "); \
    LOGLN(global_arg); \
    /* target file, mode, device */ \
    global_err = mknod(global_arg, global_arg2_i, global_arg1_i); \
    break;

#else

#define CASE__COMMAND_INDEX__MKNOD_DEV__TYPE
#define CASE__COMMAND_INDEX__MKNOD_DEV__RUN

#endif /* USES_MKNOD */


#endif /* _FS_SHELL__CMD_MKNOD_MKDEV_ */
