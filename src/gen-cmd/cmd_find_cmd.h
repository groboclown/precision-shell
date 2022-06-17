// GENERATED FROM cmd_find_cmd.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_FIND_CMD_
#define _FS_SHELL__CMD_FIND_CMD_

// Required command find_cmd



#include <stdlib.h>
#include "output.h"
#include "globals.h"
#include "helpers.h"



/* from cmd_find_cmd.h.in:52 */
extern const char cmd_name_find_cmd[];
#define ENUM_LIST__FIND_CMD \
            /* from cmd_find_cmd.h.in:52 */ \
            COMMAND_INDEX__FIND_CMD = 0,
#define VIRTUAL_ENUM_LIST__FIND_CMD
#define GLOBAL_VARDEF__FIND_CMD \
            /* from cmd_find_cmd.h.in:52 */ \
            const char cmd_name_find_cmd[] = "";
#define INITIALIZE__FIND_CMD \
            /* from cmd_find_cmd.h.in:30 */ \
    /* Global variables common to all commands.*/ \
    /* List of command names to check.*/ \
    const char *command_list_names[COMMAND_INDEX__LAST_NAMED_CMD]; \
    /* Common global values for cross-command storage.*/ \
    const char *global_arg_cached = NULL; \
    int global_arg1_i = 0; \
    int global_arg2_i = 0; \
    int global_arg3_i = 0; \
    /* This one does not span across commands; any step can use it as it needs.*/ \
    int tmp_val; \
    /* The current command name being run.*/ \
    const char *global_cmd_name = NULL; \
    /* Initialize globals.*/ \
    global_cmd = COMMAND_INDEX__FIND_CMD; \
            /* from cmd_find_cmd.h.in:52 */ \
            command_list_names[COMMAND_INDEX__FIND_CMD] = cmd_name_find_cmd;
#define STARTUP_CASE__FIND_CMD
#define RUN_CASE__FIND_CMD \
    case COMMAND_INDEX__FIND_CMD: \
        /* from cmd_find_cmd.h.in:52 */ \
            /* from cmd_find_cmd.h.in:53 */ \
        LOG(":: find_cmd start\n"); \
        global_cmd_name = global_arg; \
        /* Assume that this will fail... */ \
        global_cmd = COMMAND_INDEX__ERR; \
        global_err = 1; \
        /* Do not check if the command is "error", as that is not */ \
        /* a real callable command. */ \
        /* printf(":: scanning from %d to %d\n", COMMAND_INDEX__FIND_CMD, COMMAND_INDEX__LAST_NAMED_CMD);*/ \
        for (tmp_val = COMMAND_INDEX__FIND_CMD; tmp_val < COMMAND_INDEX__LAST_NAMED_CMD; tmp_val++) { \
            /* printf(" - checking %d - %s\n", tmp_val, command_list_names[tmp_val]);*/ \
            if (strequal(global_arg, command_list_names[tmp_val])) { \
                LOG(":: starting command processing for "); \
                LOGLN(global_arg); \
                global_err = 0; \
                global_cmd = tmp_val; \
                switch (tmp_val) { \
                    CMD_STARTUP_CASE \
                } \
                break; \
            } \
            /* printf(" -  no match on %d\n", tmp_val);*/ \
        } \
        /* printf(":: final global_cmd %d\n", global_cmd);*/ \
        break; \
        break;

#endif /* _FS_SHELL__CMD_FIND_CMD_ */
