// GENERATED FROM cmd_ls.h.in.  DO NOT EDIT.

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


#ifndef _FS_SHELL__CMD_LS_LS_F_
#define _FS_SHELL__CMD_LS_LS_F_



// Optional command LS

#ifdef USE_CMD_LS


/* from cmd_ls.h.in:28 */
extern const char cmd_name_ls[];
#define ENUM_LIST__LS \
            /* from cmd_ls.h.in:28 */ \
            COMMAND_INDEX__LS,
#define VIRTUAL_ENUM_LIST__LS
#define GLOBAL_VARDEF__LS \
            /* from cmd_ls.h.in:28 */ \
            const char cmd_name_ls[] = "ls";
#define INITIALIZE__LS \
            /* from cmd_ls.h.in:28 */ \
            command_list_names[COMMAND_INDEX__LS] = cmd_name_ls;
#define STARTUP_CASE__LS
#define RUN_CASE__LS \
    case COMMAND_INDEX__LS: \
        /* from cmd_ls.h.in:28 */ \
            /* from cmd_ls.h.in:29 */ \
            LOG(":: ls "); \
            LOGLN(global_arg); \
            ls_dir_stream = opendir(global_arg); \
            if (ls_dir_stream == NULL) { \
                global_err = 1; \
                break; \
            } \
            while ((ls_dirent = readdir(ls_dir_stream)) != NULL) { \
                global_arg1_i = 0; \
                while (ls_dirent->d_name[global_arg1_i] == '.') { \
                    global_arg1_i++; \
                } \
                if (global_arg1_i <= 2 && ls_dirent->d_name[global_arg1_i] == 0) { \
                    /* Skip '.' and '..' directories*/ \
                    /* This also implicitly skips any record with a zero length.*/ \
                    continue; \
                } \
                stdoutPLn(ls_dirent->d_name); \
            } \
        break;
#define REQUIRES_ADDL_ARG__LS

#else /* USE_CMD_LS */

#define ENUM_LIST__LS
#define VIRTUAL_ENUM_LIST__LS
#define GLOBAL_VARDEF__LS
#define INITIALIZE__LS
#define STARTUP_CASE__LS
#define RUN_CASE__LS
#define REQUIRES_ADDL_ARG__LS
#endif /* USE_CMD_LS */


// Optional command LS_T

#ifdef USE_CMD_LS_T



/* from cmd_ls.h.in:58 */
extern const char cmd_name_ls_t[];
#define ENUM_LIST__LS_T \
            /* from cmd_ls.h.in:58 */ \
            COMMAND_INDEX__LS_T,
#define VIRTUAL_ENUM_LIST__LS_T
#define GLOBAL_VARDEF__LS_T \
            /* from cmd_ls.h.in:58 */ \
            const char cmd_name_ls_t[] = "ls-t";
#define INITIALIZE__LS_T \
            /* from cmd_ls.h.in:58 */ \
            command_list_names[COMMAND_INDEX__LS_T] = cmd_name_ls_t;
#define STARTUP_CASE__LS_T
#define RUN_CASE__LS_T \
    case COMMAND_INDEX__LS_T: \
        /* from cmd_ls.h.in:58 */ \
            /* from cmd_ls.h.in:60 */ \
            LOG(":: ls-f "); \
            LOGLN(global_arg); \
            ls_dir_stream = opendir(global_arg); \
            if (ls_dir_stream == NULL) { \
                global_err = 1; \
                break; \
            } \
            while ((ls_dirent = readdir(ls_dir_stream)) != NULL) { \
                global_arg1_i = 0; \
                while (ls_dirent->d_name[global_arg1_i] == '.') { \
                    global_arg1_i++; \
                } \
                if (global_arg1_i <= 2 && ls_dirent->d_name[global_arg1_i] == 0) { \
                    /* Skip '.' and '..' directories*/ \
                    /* This also implicitly skips any record with a zero length.*/ \
                    continue; \
                } \
                switch (ls_dirent->d_type) { \
                    case DT_BLK: \
                        /* block device*/ \
                        stdoutP("b "); \
                        break; \
                    case DT_CHR: \
                        /* character device*/ \
                        stdoutP("c "); \
                        break; \
                    case DT_DIR: \
                        /* directory*/ \
                        stdoutP("d "); \
                        break; \
                    case DT_FIFO: \
                        /* named pipe*/ \
                        stdoutP("p "); \
                        break; \
                    case DT_LNK: \
                        /* symbolic link*/ \
                        stdoutP("l "); \
                        break; \
                    case DT_REG: \
                        /* regular file*/ \
                        stdoutP("f "); \
                        break; \
                    case DT_SOCK: \
                        /* UNIX domain socket*/ \
                        stdoutP("s "); \
                        break; \
                    case DT_UNKNOWN: \
                    default: \
                        /* Some other file type*/ \
                        stdoutP("o "); \
                        break; \
                } \
                stdoutPLn(ls_dirent->d_name); \
            } \
        break;
#define REQUIRES_ADDL_ARG__LS_T

#else /* USE_CMD_LS_T */

#define ENUM_LIST__LS_T
#define VIRTUAL_ENUM_LIST__LS_T
#define GLOBAL_VARDEF__LS_T
#define INITIALIZE__LS_T
#define STARTUP_CASE__LS_T
#define RUN_CASE__LS_T
#define REQUIRES_ADDL_ARG__LS_T
#endif /* USE_CMD_LS_T */



#if defined(USE_CMD_LS) || defined(USE_CMD_LS_T)


#include <sys/types.h>
#include <dirent.h>
#include "output.h"
#include "globals.h"
#include "helpers.h"


#define ENUM_LIST__LS_LS_F \
            ENUM_LIST__LS \
            ENUM_LIST__LS_T
#define VIRTUAL_ENUM_LIST__LS_LS_F \
            VIRTUAL_ENUM_LIST__LS \
            VIRTUAL_ENUM_LIST__LS_T \
            /* from cmd_ls.h.in:132 */ \
            COMMAND_INDEX__LS_ANY,
#define GLOBAL_VARDEF__LS_LS_F \
            GLOBAL_VARDEF__LS \
            GLOBAL_VARDEF__LS_T
#define INITIALIZE__LS_LS_F \
            INITIALIZE__LS \
            INITIALIZE__LS_T \
            /* from cmd_ls.h.in:133 */ \
            DIR *ls_dir_stream; \
            struct dirent *ls_dirent;
#define STARTUP_CASE__LS_LS_F \
            STARTUP_CASE__LS \
            STARTUP_CASE__LS_T
#define RUN_CASE__LS_LS_F \
            RUN_CASE__LS \
            RUN_CASE__LS_T
#define REQUIRES_ADDL_ARG__LS_LS_F \
            REQUIRES_ADDL_ARG__LS \
            REQUIRES_ADDL_ARG__LS_T

#else /* defined(USE_CMD_LS) || defined(USE_CMD_LS_T) */

#define ENUM_LIST__LS_LS_F
#define VIRTUAL_ENUM_LIST__LS_LS_F
#define GLOBAL_VARDEF__LS_LS_F
#define INITIALIZE__LS_LS_F
#define STARTUP_CASE__LS_LS_F
#define RUN_CASE__LS_LS_F
#define REQUIRES_ADDL_ARG__LS_LS_F
#endif /* defined(USE_CMD_LS) || defined(USE_CMD_LS_T) */


#endif /* _FS_SHELL__CMD_LS_LS_F_ */
