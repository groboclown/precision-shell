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
#include "output.h"
#include "args.h"
#include "globals.h"
#include "command_runner.h"
#include "command_list.h"
#include "helpers.h"


// command_runner runs the commands over the parsed arguments.
//   returns the error code.
int command_runner() {
    // Initialze the command execution.
    int is_cmd_active = 0;
    int err = 0;
    int err_count = 0;
    int idx;

    // global value initialization.
#ifdef USES_FMODE
    global_fmode = 0644;
#endif

    global_arg = args_advance_token();
    while (global_arg != NULL) {
        // This is a long if/else block until error checking.

        // ==================================================================
        // Check end-of-command first.
        //   This allows recognizing "; ;" as okay.

        if (strequal("&&", global_arg)) {
            if (err_count > 0) {
                // && with errors stops the shell.
                stderrP("FAIL &&\n");
                break;
            }
            LOG(":: &&\n");
            is_cmd_active = 0;
        } else

        if (strequal(";", global_arg)) {
            // ";" ignores any errors, resetting the error count.
            LOG(":: ;\n");
            err_count = 0;
            is_cmd_active = 0;
        } else

        if (is_cmd_active) {
            // ==================================================================
            // Run the currently active command.
            err = command_function[global_cmd]();
        } else {
            // ==================================================================
            // Find the invoked command.
            global_cmd_name = global_arg;

            // Assume that this will fail...
            global_cmd = COMMAND_INDEX__ERR;
            err = 1;

            for (idx = COMMAND_INDEX__NOOP; idx < COMMAND_INDEX__ERR; idx++) {
                if (strequal(global_arg, command_list_names[idx])) {
                    global_cmd = command_setup[idx](idx);
                    err = 0;
                    break;
                }
            }
        }

        // ==================================================================
        // Error checking.
        if (err != 0) {
            // Could use fprintf, but let's get smaller.
            stderrP("ERROR ");
            stderrP(global_cmd_name);
            stderrP(": ");
            stderrPLn(global_arg);
            err_count++;
        }

        // advance.
        global_arg = args_advance_token();
    }
    return err_count;
}
