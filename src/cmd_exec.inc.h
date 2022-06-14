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

#ifndef _FS_SHELL__CMD_EXEC_

// No case execution
#define CASE__COMMAND_INDEX__EXEC


#ifdef USE_CMD_EXEC

#include <stdlib.h>

// Number of arguments for exec
#define MAX_EXEC_ARGS 1000
// Number of characters, including trailing \0 for all the arguments concatenated together.
#define MAX_EXEC_ARG_LEN (MAX_EXEC_ARGS * 100)


// Strange form of the command.  All other arguments must be pulled in and run as-is.
// This ignores all command parsing after this.

// Slurp up all remaining arguments up to command max.
// global_arg: current argument
// exec_argv: pointers to arguments.
// exec_arg3: container for all arguments copied in.  exec_argv points into this.
// global_arg1_i: position in exec_argv
// global_arg2_i: character position in argv[global_arg1_i]
// global_arg3_i: loop counter

#ifdef DEBUG
#define EXEC_DEBUG_REPORT \
    LOG(":: Running command ["); \
    LOG(exec_argv[0]); \
    LOG("] with arguments ["); \
    for (global_arg3_i = 0; exec_argv[global_arg3_i] != NULL; global_arg3_i++) { \
        if (global_arg3_i > 0) { \
            LOG("] ["); \
        } \
        LOG(exec_argv[global_arg3_i]); \
    } \
    LOG("]\n");
#else
#define EXEC_DEBUG_REPORT
#endif

#define STARTUP__COMMAND_INDEX__EXEC \
case COMMAND_INDEX__EXEC: \
    /* add a trailing + 1 for the final 0, if necessary */ \
    exec_argv = malloc((sizeof(const char *) * MAX_EXEC_ARGS) + 1); \
    if (exec_argv == NULL) { \
        stderrP("ERROR malloc failed\n"); \
        return 1; \
    } \
    /* add a trailing + 1 for the final 0, if necessary */ \
    exec_arg3 = malloc((sizeof(const char) * MAX_EXEC_ARG_LEN) + 1); \
    if (exec_arg3 == NULL) { \
        stderrP("ERROR malloc failed\n"); \
        return 1; \
    } \
    global_arg1_i = 0; \
    global_arg2_i = 0; \
    global_arg = args_advance_token(); \
    LOG(":: generating arguments\n"); \
    while (global_arg != NULL) { \
        if (global_arg1_i >= MAX_EXEC_ARGS) { \
            stderrP("ERROR exec too many arguments\n"); \
            return 1; \
        } \
        if (global_arg2_i >= MAX_EXEC_ARG_LEN) { \
            stderrP("ERROR exec argument total length exceeded\n"); \
            return 1; \
        } \
        /* set current argv index to point to start of argument, and advance argv index. */ \
        exec_argv[global_arg1_i++] = &(exec_arg3[global_arg2_i]); \
        /* copy the current argument into the long array */ \
        global_arg3_i = 0; \
        while (global_arg[global_arg3_i] != 0 && global_arg2_i < MAX_EXEC_ARG_LEN) { \
            exec_arg3[global_arg2_i++] = global_arg[global_arg3_i++]; \
        } \
        /* terminate the copied argument. */ \
        exec_arg3[global_arg2_i++] = 0; \
        LOG(":: arg: ["); \
        LOG(exec_argv[global_arg1_i-1]); \
        LOG("]\n"); \
        global_arg = args_advance_token(); \
    } \
    /* set the final argument to NULL to terminate the list of pointers. */ \
    exec_argv[global_arg1_i++] = NULL; \
    if (global_arg1_i <= 1) { \
        /* No command to run */ \
        stderrP("ERROR no command\n"); \
        return 1; \
    } \
    EXEC_DEBUG_REPORT \
    /* This launches a new executable and terminates this one immediately. */ \
    execvp(exec_argv[0], (char * const*) exec_argv);

    // trailing break isn't necessary here.

#else /* USE_CMD_EXEC */

#define STARTUP__COMMAND_INDEX__EXEC


#endif /* USE_CMD_EXEC */
#endif /* _FS_SHELL__CMD_EXEC_ */
