// GENERATED FROM cmd_spawn.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_SPAWN_
#define _FS_SHELL__CMD_SPAWN_

// Optional command spawn

#ifdef USE_CMD_SPAWN



// Spawn works just like exec, except that it always continues to the next command.

#include <stdlib.h>
#include <unistd.h>
#include "args.h"
#include "output.h"
#include "globals.h"


#ifdef DEBUG
#define EXEC_DEBUG_REPORT \
    LOG(":: Running command ["); \
    LOG(shared_split_argv[0]); \
    LOG("] with arguments ["); \
    for (global_arg2_i = 0; shared_split_argv[global_arg2_i] != NULL; global_arg2_i++) { \
        if (global_arg2_i > 0) { \
            LOG("] ["); \
        } \
        LOG(shared_split_argv[global_arg2_i]); \
    } \
    LOG("]\n");
#else
#define EXEC_DEBUG_REPORT
#endif






    // If this doesn't have another argument, bad things will happen.

/* from cmd_spawn.h.in:53 */
extern const char cmd_name_spawn[];
#define ENUM_LIST__SPAWN \
            /* from cmd_spawn.h.in:53 */ \
            COMMAND_INDEX__SPAWN,
#define VIRTUAL_ENUM_LIST__SPAWN \
            /* from cmd_spawn.h.in:78 */ \
            COMMAND_INDEX__SPAWN__CMD, \
            /* from cmd_spawn.h.in:101 */ \
            COMMAND_INDEX__SPAWN__PID,
#define GLOBAL_VARDEF__SPAWN \
            /* from cmd_spawn.h.in:53 */ \
            const char cmd_name_spawn[] = "spawn";
#define INITIALIZE__SPAWN \
            /* from cmd_spawn.h.in:53 */ \
            command_list_names[COMMAND_INDEX__SPAWN] = cmd_name_spawn; \
            /* from cmd_spawn.h.in:58 */ \
        char global_itoa[12]; \
        char *global_itoa_ptr; /* TODO find a different value to use.  global_arg_cached is const char *.*/ \
        pid_t global_pid;
#define STARTUP_CASE__SPAWN \
    case COMMAND_INDEX__SPAWN: \
        /* from cmd_spawn.h.in:53 */ \
            /* from cmd_spawn.h.in:64 */ \
        /* Fork the process.  This will have the potential to*/ \
        /* cause very weird behavior if no argument is given.*/ \
        global_pid = fork(); \
        if (global_pid == -1) { \
            LOG(":: failed to fork process\n"); \
            global_err = 1; \
            global_cmd = COMMAND_INDEX__ERR; \
        } else { \
            global_cmd = COMMAND_INDEX__SPAWN__CMD; \
        } \
        break;
#define RUN_CASE__SPAWN \
    case COMMAND_INDEX__SPAWN__CMD: \
        /* from cmd_spawn.h.in:78 */ \
            /* from cmd_spawn.h.in:79 */ \
        if (global_pid == 0) { \
            /* Split the arguments.*/ \
            SHARED_SPLIT__PARSE_ARG \
            EXEC_DEBUG_REPORT \
            /* This launches a new executable and terminates this one immediately.*/ \
            execvp(shared_split_argv[0], (char * const*) shared_split_argv); \
            /* If this code is reached, then exec failed to run.*/ \
            /* Unlike exec, fail immediately and don't keep going.*/ \
            return 1; \
        } \
        if (global_pid == -1) { \
            /* Fork failed, don't keep going.*/ \
            global_err = 1; \
        } \
        /* Else just slurp up the argument.*/ \
        /* Then, if the env is next, post the pid to that.*/ \
        global_cmd = COMMAND_INDEX__SPAWN__PID; \
        break; \
    case COMMAND_INDEX__SPAWN__PID: \
        /* from cmd_spawn.h.in:101 */ \
            /* from cmd_spawn.h.in:102 */ \
        /* Put the PID into the environment variable global_arg.*/ \
        /* This argument can only be run from the parent due to the logic above.*/ \
        /* itoa(global_pid, global_itoa, 10);*/ \
        global_itoa_ptr = global_itoa + (3 * sizeof(int)); \
        *(--global_itoa_ptr) = '\0'; \
        tmp_val = global_pid; \
        do { \
            *(--global_itoa_ptr) = '0' + (tmp_val % 10); \
            tmp_val /= 10; \
        } while (tmp_val); \
        LOG(":: Set env "); \
        LOG(global_arg); \
        LOG(" to PID "); \
        LOGLN(global_itoa_ptr); \
        setenv(global_arg, global_itoa_ptr, 1); \
        global_cmd = COMMAND_INDEX__ERR; \
        break;
#define REQUIRES_ADDL_ARG__SPAWN \
            case COMMAND_INDEX__SPAWN:

#else /* USE_CMD_SPAWN */

#define ENUM_LIST__SPAWN
#define VIRTUAL_ENUM_LIST__SPAWN
#define GLOBAL_VARDEF__SPAWN
#define INITIALIZE__SPAWN
#define STARTUP_CASE__SPAWN
#define RUN_CASE__SPAWN
#define REQUIRES_ADDL_ARG__SPAWN
#endif /* USE_CMD_SPAWN */

#endif /* _FS_SHELL__CMD_SPAWN_ */
