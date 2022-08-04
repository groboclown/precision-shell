// GENERATED FROM cmd_file_stat.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_FILE_STAT_
#define _FS_SHELL__CMD_FILE_STAT_

// Optional command file-stat

#ifdef USE_CMD_FILE_STAT



#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>


/* from cmd_file_stat.h.in:32 */
extern const char cmd_name_file_stat[];
#define ENUM_LIST__FILE_STAT \
            /* from cmd_file_stat.h.in:32 */ \
            COMMAND_INDEX__FILE_STAT,
#define VIRTUAL_ENUM_LIST__FILE_STAT
#define GLOBAL_VARDEF__FILE_STAT \
            /* from cmd_file_stat.h.in:32 */ \
            const char cmd_name_file_stat[] = "file-stat";
#define INITIALIZE__FILE_STAT \
            /* from cmd_file_stat.h.in:32 */ \
            command_list_names[COMMAND_INDEX__FILE_STAT] = cmd_name_file_stat; \
            /* from cmd_file_stat.h.in:33 */ \
        struct stat file_stat_buf;
#define STARTUP_CASE__FILE_STAT
#define RUN_CASE__FILE_STAT \
    case COMMAND_INDEX__FILE_STAT: \
        /* from cmd_file_stat.h.in:32 */ \
            /* from cmd_file_stat.h.in:38 */ \
        /* "ls -l" like format.*/ \
        LOG(":: file-stat "); \
        LOGLN(global_arg); \
        global_err = stat(global_arg, &file_stat_buf); \
        if (global_err == 0) { \
            switch (file_stat_buf.st_mode & S_IFMT) { \
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
            if ((file_stat_buf.st_mode & S_ISUID) != 0) { \
                stdoutP("t"); \
            } else if ((file_stat_buf.st_mode & S_ISGID) != 0) { \
                stdoutP("s"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IRUSR) != 0) { \
                stdoutP("r"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IWUSR) != 0) { \
                stdoutP("w"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IXUSR) != 0) { \
                stdoutP("x"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IRGRP) != 0) { \
                stdoutP("r"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IWGRP) != 0) { \
                stdoutP("w"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IXGRP) != 0) { \
                stdoutP("x"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IROTH) != 0) { \
                stdoutP("r"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IWOTH) != 0) { \
                stdoutP("w"); \
            } else { \
                stdoutP("-"); \
            } \
            if ((file_stat_buf.st_mode & S_IXOTH) != 0) { \
                stdoutP("x"); \
            } else { \
                stdoutP("-"); \
            } \
            stdoutP(" "); \
            stdoutP(shared_itoa(file_stat_buf.st_nlink, global_itoa)); \
            stdoutP(" "); \
            stdoutP(shared_itoa(file_stat_buf.st_uid, global_itoa)); \
            stdoutP(" "); \
            stdoutP(shared_itoa(file_stat_buf.st_gid, global_itoa)); \
            stdoutP(" "); \
            /* st_rdev is unusable for device numbers like mkdev.*/ \
            /* split it up for normal values.*/ \
            /* stdoutP(shared_itoa(file_stat_buf.st_rdev, global_itoa));*/ \
            stdoutP(shared_itoa(major(file_stat_buf.st_rdev), global_itoa)); \
            stdoutP(" "); \
            stdoutP(shared_itoa(minor(file_stat_buf.st_rdev), global_itoa)); \
            stdoutP(" "); \
            stdoutP(shared_itoa(file_stat_buf.st_size, global_itoa)); \
            stdoutP(" "); \
            stdoutPLn(global_arg); \
        } \
        break;
#define REQUIRES_ADDL_ARG__FILE_STAT

#else /* USE_CMD_FILE_STAT */

#define ENUM_LIST__FILE_STAT
#define VIRTUAL_ENUM_LIST__FILE_STAT
#define GLOBAL_VARDEF__FILE_STAT
#define INITIALIZE__FILE_STAT
#define STARTUP_CASE__FILE_STAT
#define RUN_CASE__FILE_STAT
#define REQUIRES_ADDL_ARG__FILE_STAT
#endif /* USE_CMD_FILE_STAT */

#endif /* _FS_SHELL__CMD_FILE_STAT_ */
