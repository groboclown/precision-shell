// GENERATED FROM cmd_ls_ls.h.in.  DO NOT EDIT.

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


/* from cmd_ls_ls.h.in:28 */
extern const char cmd_name_ls[];
#define ENUM_LIST__LS \
            /* from cmd_ls_ls.h.in:28 */ \
            COMMAND_INDEX__LS,
#define VIRTUAL_ENUM_LIST__LS
#define GLOBAL_VARDEF__LS \
            /* from cmd_ls_ls.h.in:28 */ \
            const char cmd_name_ls[] = "ls";
#define INITIALIZE__LS \
            /* from cmd_ls_ls.h.in:28 */ \
            command_list_names[COMMAND_INDEX__LS] = cmd_name_ls;
#define STARTUP_CASE__LS
#define RUN_CASE__LS \
    case COMMAND_INDEX__LS: \
        /* from cmd_ls_ls.h.in:28 */ \
            /* from cmd_ls_ls.h.in:29 */ \
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



/* from cmd_ls_ls.h.in:58 */
extern const char cmd_name_ls_t[];
#define ENUM_LIST__LS_T \
            /* from cmd_ls_ls.h.in:58 */ \
            COMMAND_INDEX__LS_T,
#define VIRTUAL_ENUM_LIST__LS_T
#define GLOBAL_VARDEF__LS_T \
            /* from cmd_ls_ls.h.in:58 */ \
            const char cmd_name_ls_t[] = "ls-t";
#define INITIALIZE__LS_T \
            /* from cmd_ls_ls.h.in:58 */ \
            command_list_names[COMMAND_INDEX__LS_T] = cmd_name_ls_t;
#define STARTUP_CASE__LS_T
#define RUN_CASE__LS_T \
    case COMMAND_INDEX__LS_T: \
        /* from cmd_ls_ls.h.in:58 */ \
            /* from cmd_ls_ls.h.in:60 */ \
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


// Optional command LS_L

#ifdef USE_CMD_LS_L


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/sysmacros.h>



/* from cmd_ls_ls.h.in:136 */
extern const char cmd_name_ls_l[];
#define ENUM_LIST__LS_L \
            /* from cmd_ls_ls.h.in:136 */ \
            COMMAND_INDEX__LS_L,
#define VIRTUAL_ENUM_LIST__LS_L
#define GLOBAL_VARDEF__LS_L \
            /* from cmd_ls_ls.h.in:136 */ \
            const char cmd_name_ls_l[] = "ls-l";
#define INITIALIZE__LS_L \
            /* from cmd_ls_ls.h.in:131 */ \
        struct stat ls_l_stat_buf; \
        char *ls_l_filename; \
            /* from cmd_ls_ls.h.in:136 */ \
            command_list_names[COMMAND_INDEX__LS_L] = cmd_name_ls_l;
#define STARTUP_CASE__LS_L
#define RUN_CASE__LS_L \
    case COMMAND_INDEX__LS_L: \
        /* from cmd_ls_ls.h.in:136 */ \
            /* from cmd_ls_ls.h.in:138 */ \
            LOG(":: ls-l "); \
            LOGLN(global_arg); \
            global_arg2_i = strlen(global_arg); \
            if (global_arg2_i <= 0) { \
                global_err = 1; \
                break; \
            } \
            while (global_arg2_i > 1 && global_arg[global_arg2_i - 1] == '/') { \
                /* overwrite the trailing '/' later.*/ \
                --global_arg2_i; \
            } \
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
                global_arg3_i = strlen(ls_dirent->d_name); \
                ls_l_filename = malloc(sizeof(char) * (global_arg2_i + global_arg3_i + 2)); \
                if (ls_l_filename == NULL) { \
                    stderrP(helper_str__malloc_failed); \
                    global_err++; \
                    break; \
                } \
                strcpy(ls_l_filename, global_arg); \
                ls_l_filename[global_arg2_i] = '/'; \
                strcpy(&(ls_l_filename[global_arg2_i + 1]), ls_dirent->d_name); \
                global_arg1_i = stat(ls_l_filename, &ls_l_stat_buf); \
                if (global_arg1_i == 0) { \
                    switch (ls_l_stat_buf.st_mode & S_IFMT) { \
                        case S_IFSOCK: \
                            stdoutP("s"); \
                            break; \
                        case S_IFLNK: \
                            stdoutP("l"); \
                            break; \
                        case S_IFREG: \
                            stdoutP("-"); \
                            break; \
                        case S_IFBLK: \
                            stdoutP("b"); \
                            break; \
                        case S_IFDIR: \
                            stdoutP("d"); \
                            break; \
                        case S_IFCHR: \
                            stdoutP("c"); \
                            break; \
                        case S_IFIFO: \
                            stdoutP("p"); \
                            break; \
                        default: \
                            stdoutP("?"); \
                            break; \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_ISUID) != 0) { \
                        stdoutP("t"); \
                    } else if ((ls_l_stat_buf.st_mode & S_ISGID) != 0) { \
                        stdoutP("s"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IRUSR) != 0) { \
                        stdoutP("r"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IWUSR) != 0) { \
                        stdoutP("w"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IXUSR) != 0) { \
                        stdoutP("x"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IRGRP) != 0) { \
                        stdoutP("r"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IWGRP) != 0) { \
                        stdoutP("w"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IXGRP) != 0) { \
                        stdoutP("x"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IROTH) != 0) { \
                        stdoutP("r"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IWOTH) != 0) { \
                        stdoutP("w"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    if ((ls_l_stat_buf.st_mode & S_IXOTH) != 0) { \
                        stdoutP("x"); \
                    } else { \
                        stdoutP("-"); \
                    } \
                    stdoutP(" "); \
                    stdoutP(shared_itoa(ls_l_stat_buf.st_nlink, global_itoa)); \
                    stdoutP(" "); \
                    stdoutP(shared_itoa(ls_l_stat_buf.st_uid, global_itoa)); \
                    stdoutP(" "); \
                    stdoutP(shared_itoa(ls_l_stat_buf.st_gid, global_itoa)); \
                    stdoutP(" "); \
                    /* st_rdev is unusable for device numbers like mkdev.*/ \
                    /* split it up for normal values.*/ \
                    /* stdoutP(shared_itoa(ls_l_stat_buf.st_rdev, global_itoa));*/ \
                    stdoutP(shared_itoa(major(ls_l_stat_buf.st_rdev), global_itoa)); \
                    stdoutP(" "); \
                    stdoutP(shared_itoa(minor(ls_l_stat_buf.st_rdev), global_itoa)); \
                    stdoutP(" "); \
                    stdoutP(shared_itoa(ls_l_stat_buf.st_size, global_itoa)); \
                    stdoutP(" "); \
                    /* Note: no date / time reported.*/ \
                    stdoutPLn(ls_l_filename); \
                } else { \
                    /* printf(":: stat(%s) == %d\n", ls_l_filename, global_arg1_i);*/ \
                    global_err++; \
                } \
                free(ls_l_filename); \
            } \
        break;
#define REQUIRES_ADDL_ARG__LS_L

#else /* USE_CMD_LS_L */

#define ENUM_LIST__LS_L
#define VIRTUAL_ENUM_LIST__LS_L
#define GLOBAL_VARDEF__LS_L
#define INITIALIZE__LS_L
#define STARTUP_CASE__LS_L
#define RUN_CASE__LS_L
#define REQUIRES_ADDL_ARG__LS_L
#endif /* USE_CMD_LS_L */


#if defined(USE_CMD_LS) || defined(USE_CMD_LS_T) || defined(USE_CMD_LS_L)


#include <sys/types.h>
#include <dirent.h>
#include "output.h"
#include "globals.h"
#include "helpers.h"


#define ENUM_LIST__LS_LS_F \
            ENUM_LIST__LS \
            ENUM_LIST__LS_T \
            ENUM_LIST__LS_L
#define VIRTUAL_ENUM_LIST__LS_LS_F \
            VIRTUAL_ENUM_LIST__LS \
            VIRTUAL_ENUM_LIST__LS_T \
            VIRTUAL_ENUM_LIST__LS_L \
            /* from cmd_ls_ls.h.in:301 */ \
            COMMAND_INDEX__LS_ANY,
#define GLOBAL_VARDEF__LS_LS_F \
            GLOBAL_VARDEF__LS \
            GLOBAL_VARDEF__LS_T \
            GLOBAL_VARDEF__LS_L
#define INITIALIZE__LS_LS_F \
            INITIALIZE__LS \
            INITIALIZE__LS_T \
            INITIALIZE__LS_L \
            /* from cmd_ls_ls.h.in:302 */ \
            DIR *ls_dir_stream; \
            struct dirent *ls_dirent;
#define STARTUP_CASE__LS_LS_F \
            STARTUP_CASE__LS \
            STARTUP_CASE__LS_T \
            STARTUP_CASE__LS_L
#define RUN_CASE__LS_LS_F \
            RUN_CASE__LS \
            RUN_CASE__LS_T \
            RUN_CASE__LS_L
#define REQUIRES_ADDL_ARG__LS_LS_F \
            REQUIRES_ADDL_ARG__LS \
            REQUIRES_ADDL_ARG__LS_T \
            REQUIRES_ADDL_ARG__LS_L

#else /* defined(USE_CMD_LS) || defined(USE_CMD_LS_T) || defined(USE_CMD_LS_L) */

#define ENUM_LIST__LS_LS_F
#define VIRTUAL_ENUM_LIST__LS_LS_F
#define GLOBAL_VARDEF__LS_LS_F
#define INITIALIZE__LS_LS_F
#define STARTUP_CASE__LS_LS_F
#define RUN_CASE__LS_LS_F
#define REQUIRES_ADDL_ARG__LS_LS_F
#endif /* defined(USE_CMD_LS) || defined(USE_CMD_LS_T) || defined(USE_CMD_LS_L) */


#endif /* _FS_SHELL__CMD_LS_LS_F_ */
