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
#include "gen-cmd/command_list.h"
#include "helpers.h"
#include "version.h"

CMD_GLOBAL_VARDEF

// command_runner runs the commands over the parsed arguments.
//   returns the error code.
int command_runner() {
    // ======================================================================
    // Command runner internal variables
    int _err_count = 0;

    // Current command index to run.
    int global_cmd;
    // The current argument being parsed.
    const char *global_arg;
    // Marker that there's an error in the current command.
    int global_err = 0;

    // Initialize all the commands that need it.
    CMD_INITIALIZE

    // ======================================================================
    // Argument Parsing Loop
    while (1 == 1) {
        global_arg = args_advance_token();
        if (global_arg == NULL) {
            // TODO could add another case here for the current command.
            // This would allow us to check for commands that require
            // an extra argument, and, if not present, geenrate an error.

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

            LOG(":: processing ");
            LOGLN(global_arg);

            switch (global_cmd) {
                // ==========================================================
                // Find the invoked command.

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
