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
#include "version.h"


// command_runner runs the commands over the parsed arguments.
//   returns the error code.
int command_runner() {
    // ======================================================================
    // Command runner internal variables
    int _err_count = 0;
    int _idx;

    LOCAL_GLOBALS

    // Initialize all the commands that need it.
    CMD_INITIALIZE

    // ======================================================================
    // Argument Parsing Loop
    while (1 == 1) {
        global_arg = args_advance_token();
        if (global_arg == NULL) {
            break;
        }
        // This is a long if/else block until error checking.

        // ==================================================================
        // Check end-of-command first.
        //   This allows recognizing "; ;" as okay.

        if (strequal("&&", global_arg)) {
            if (_err_count > 0) {
                // && with errors stops the shell.
                stderrP("FAIL &&\n");
                break;
            }
            LOG(":: &&\n");
            global_cmd = COMMAND_INDEX__FIND_CMD;
        } else

        if (strequal(";", global_arg)) {
            // ";" ignores any errors, resetting the error count.
            LOG(":: ;\n");
            _err_count = 0;
            global_cmd = COMMAND_INDEX__FIND_CMD;
        } else {
            // Default to no error.
            global_err = 0;

            switch (global_cmd) {
                // ==========================================================
                // Find the invoked command.
                case COMMAND_INDEX__FIND_CMD:
                    // This should be defined in its own cmd file,
                    // but, due to ordering of referenced values, it
                    // would otherwise cause bad loading as command_list
                    // depends on find_cmd and find_cmd depends on command_list.
                    global_cmd_name = global_arg;

                    // Assume that this will fail...
                    global_cmd = COMMAND_INDEX__ERR;
                    global_err = 1;

                    // Don't check if the command is "error", as that's not
                    // a real callable command.
                    for (_idx = COMMAND_INDEX__FIND_CMD; _idx < COMMAND_INDEX__ERR; _idx++) {
                        if (strequal(global_arg, command_list_names[_idx])) {
                            LOG(":: starting command processing for ");
                            LOGLN(global_arg);
                            global_err = 0;
                            global_cmd = _idx;
                            switch (_idx) {
                                CMD_STARTUP_CASE                                
                            }
                            break;
                        }
                    }
                    break;

                CMD_RUN_CASE
            }
        }

        // ==================================================================
        // Error checking.
        if (global_err != 0) {
            // Could use fprintf, but let's get smaller.
            stderrP("ERROR ");
            stderrP(global_cmd_name);
            stderrP(": ");
            stderrPLn(global_arg);
            _err_count++;
        }
    }
    return _err_count;
}
