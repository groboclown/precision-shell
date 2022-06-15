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

#include "uses.h"

#ifdef USE_CMD_EXEC

#include <stdlib.h>

#include "args.h"
#include "output.h"
#include "globals.h"
#include "helpers.h"
#include "command_common.h"
#include "command_list.h"
#include "cmd_exec.h"

// Number of arguments for exec
#define MAX_EXEC_ARGS 1000
// Number of characters, including trailing \0 for all the arguments concatenated together.
#define MAX_EXEC_ARG_LEN (MAX_EXEC_ARGS * 100)


const char NAMEVAR__CMD_EXEC[] = NAME__CMD_EXEC;


int cmd_exec_setup(int src_cmd_idx) {

    // Strange form of the command.  All other arguments must be pulled in and run as-is.
    // This ignores all command parsing after this.

    // Slurp up all remaining arguments up to command max.
    // inp_arg: current argument
    // exec_argv: pointers to arguments.
    // flat_args: container for all arguments copied in.  exec_argv points into this.
    // argv_index: position in exec_argv
    // arg_pos: character position in argv[argv_index]
    // loop_idx: loop counter

    const char **exec_argv = NULL;
    char *flat_args = NULL;
    int argv_index;
    int arg_pos;
    int loop_idx;
    const char *inp_arg;

    // add a trailing + 1 for the final 0, if necessary
    exec_argv = malloc((sizeof(const char *) * MAX_EXEC_ARGS) + 1);
    if (exec_argv == NULL) {
        stderrP("ERROR malloc failed\n");
        return -1;
    }
    // add a trailing + 1 for the final 0, if necessary
    flat_args = malloc((sizeof(const char) * MAX_EXEC_ARG_LEN) + 1);
    if (flat_args == NULL) {
        free(exec_argv);
        stderrP("ERROR malloc failed\n");
        return -1;
    }
    argv_index = 0;
    arg_pos = 0;
    inp_arg = args_advance_token();
    LOG(":: generating arguments\n");
    while (inp_arg != NULL) {
        if (argv_index >= MAX_EXEC_ARGS) {
            stderrP("ERROR exec too many arguments\n");
            return -1;
        }
        if (arg_pos >= MAX_EXEC_ARG_LEN) {
            stderrP("ERROR exec argument total length exceeded\n");
            return -1;
        }
        // set current argv index to point to start of argument, and advance argv index.
        exec_argv[argv_index++] = &(flat_args[arg_pos]);
        // copy the current argument into the long array
        loop_idx = 0;
        while (inp_arg[loop_idx] != 0 && arg_pos < MAX_EXEC_ARG_LEN) {
            flat_args[arg_pos++] = inp_arg[loop_idx++];
        }
        // terminate the copied argument.
        flat_args[arg_pos++] = 0;
        LOG(":: arg: [");
        LOG(exec_argv[argv_index-1]);
        LOG("]\n");
        inp_arg = args_advance_token();
    }
    // set the final argument to NULL to terminate the list of pointers.
    exec_argv[argv_index++] = NULL;
    if (argv_index <= 1) {
        // No command to run
        stderrP("ERROR no command\n");
        return -1;
    }


    #ifdef DEBUG
    LOG(":: Running command [");
    LOG(exec_argv[0]);
    LOG("] with arguments [");
    for (global_arg3_i = 0; exec_argv[global_arg3_i] != NULL; global_arg3_i++) {
        if (global_arg3_i > 0) {
            LOG("] [");
        }
        LOG(exec_argv[global_arg3_i]);
    }
    LOG("]\n");
    #endif /* DEBUG */


    // This launches a new executable and terminates this one immediately.
    execvp(exec_argv[0], (char * const*) exec_argv);

    // C requires we have a return statement, even though this code is unreachable.
    return COMMAND_INDEX__ERR;
}



#else
// disable pedantic warning
typedef int iso_translation_unit__exec;
#endif /* USE_CMD_EXEC */
