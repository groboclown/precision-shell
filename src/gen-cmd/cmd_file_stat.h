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


/* from cmd_file_stat.h.in:31 */
extern const char cmd_name_file_stat[];
#define ENUM_LIST__FILE_STAT \
            /* from cmd_file_stat.h.in:31 */ \
            COMMAND_INDEX__FILE_STAT,
#define VIRTUAL_ENUM_LIST__FILE_STAT
#define GLOBAL_VARDEF__FILE_STAT \
            /* from cmd_file_stat.h.in:31 */ \
            const char cmd_name_file_stat[] = "file-stat";
#define INITIALIZE__FILE_STAT \
            /* from cmd_file_stat.h.in:31 */ \
            command_list_names[COMMAND_INDEX__FILE_STAT] = cmd_name_file_stat; \
            /* from cmd_file_stat.h.in:32 */ \
        struct stat file_stat_buf;
#define STARTUP_CASE__FILE_STAT
#define RUN_CASE__FILE_STAT \
    case COMMAND_INDEX__FILE_STAT: \
        /* from cmd_file_stat.h.in:31 */ \
            /* from cmd_file_stat.h.in:37 */ \
        LOG(":: file-stat "); \
        LOGLN(global_arg); \
        global_err = stat(global_arg, &file_stat_buf); \
        if (global_err == 0) { \
            stdoutP("File type: "); \
            switch (file_stat_buf.st_mode & S_IFMT) { \
                case S_IFSOCK: \
                    stdoutP("socket\n"); \
                    break; \
                case S_IFLNK: \
                    stdoutP("symbolic link\n"); \
                    break; \
                case S_IFREG: \
                    stdoutP("file\n"); \
                    break; \
                case S_IFBLK: \
                    stdoutP("block\n"); \
                    break; \
                case S_IFDIR: \
                    stdoutP("directory\n"); \
                    break; \
                case S_IFCHR: \
                    stdoutP("character\n"); \
                    break; \
                case S_IFIFO: \
                    stdoutP("fifo\n"); \
                    break; \
            } \
            stdoutP("Mode: "); \
            global_itoa[0] = '-'; \
            if ((file_stat_buf.st_mode & S_ISUID) != 0) { \
                global_itoa[0] = 'U'; \
            } \
            global_itoa[1] = '-'; \
            if ((file_stat_buf.st_mode & S_ISGID) != 0) { \
                global_itoa[1] = 'G'; \
            } \
            global_itoa[2] = '-'; \
            if ((file_stat_buf.st_mode & S_ISVTX) != 0) { \
                global_itoa[2] = 's'; \
            } \
            global_itoa[3] = '-'; \
            if ((file_stat_buf.st_mode & S_IRUSR) != 0) { \
                global_itoa[3] = 'r'; \
            } \
            global_itoa[4] = '-'; \
            if ((file_stat_buf.st_mode & S_IWUSR) != 0) { \
                global_itoa[4] = 'w'; \
            } \
            global_itoa[5] = '-'; \
            if ((file_stat_buf.st_mode & S_IXUSR) != 0) { \
                global_itoa[5] = 'x'; \
            } \
            global_itoa[6] = '-'; \
            if ((file_stat_buf.st_mode & S_IRGRP) != 0) { \
                global_itoa[6] = 'r'; \
            } \
            global_itoa[7] = '-'; \
            if ((file_stat_buf.st_mode & S_IWGRP) != 0) { \
                global_itoa[7] = 'w'; \
            } \
            global_itoa[8] = '-'; \
            if ((file_stat_buf.st_mode & S_IXGRP) != 0) { \
                global_itoa[8] = 'x'; \
            } \
            global_itoa[9] = '-'; \
            if ((file_stat_buf.st_mode & S_IROTH) != 0) { \
                global_itoa[9] = 'r'; \
            } \
            global_itoa[10] = '-'; \
            if ((file_stat_buf.st_mode & S_IWOTH) != 0) { \
                global_itoa[10] = 'w'; \
            } \
            global_itoa[11] = '-'; \
            if ((file_stat_buf.st_mode & S_IXOTH) != 0) { \
                global_itoa[11] = 'x'; \
            } \
            global_itoa[12] = '-'; \
            if ((file_stat_buf.st_mode & S_IXOTH) != 0) { \
                global_itoa[12] = 'x'; \
            } \
            global_itoa[13] = '\n'; \
            global_itoa[14] = '\0'; \
            stdoutP(global_itoa); \
            stdoutP("UID: "); \
            stdoutPLn(shared_itoa(file_stat_buf.st_uid, global_itoa)); \
            stdoutP("GID: "); \
            stdoutPLn(shared_itoa(file_stat_buf.st_gid, global_itoa)); \
            stdoutP("DevID: "); \
            stdoutPLn(shared_itoa(file_stat_buf.st_rdev, global_itoa)); \
            stdoutP("Hard link count: "); \
            stdoutPLn(shared_itoa(file_stat_buf.st_nlink, global_itoa)); \
            stdoutP("Bytes: "); \
            stdoutPLn(shared_itoa(file_stat_buf.st_size, global_itoa)); \
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