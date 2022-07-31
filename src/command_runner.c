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
#include "force-inline.h"

CMD_GLOBAL_VARDEF


// Temporarily disable until desired.
#ifdef REQUIRE_FULL_CMD
/*forceinline*/ int _on_cmd_end(const char *name, int cmd_idx) {
    switch (cmd_idx) {
        // The last index, which shouldn't ever be used,
        //   is included here to make the case not fail in
        //   the event that the compilation has no items.
        CMD_REQUIRES_ADDL_ARG
        case COMMAND_INDEX__LAST_VIRTUAL_CMD:
            stderrP("ERROR ");
            stderrP(name);
            stderrP(": requires another argument\n");
            return 1;
    }
    return 0;
}
#endif

// command_runner runs the commands over the parsed arguments.
//   returns the error code.
int command_runner(struct ArgState *arg_state) {
    // ======================================================================
    // Command runner internal variables
    int _err_count = 0;

    // Current command index to run.
    enum CommandIndex global_cmd;

    // The current argument being parsed.
    const Argument *global_arg_state;
    const char *global_arg;

    // The current command name being run.
    //   Because a command can have more arguments than the
    //   preserve count, this should be a copy of the input argument.
    char global_cmd_name[PARSED_ARG_SIZE] = "";

    // Marker that there's an error in the current command.
    int global_err = 0;

    // Initialize all the commands that need it.
    CMD_INITIALIZE

    // ======================================================================
    // Argument Parsing Loop
    while (1 == 1) {
        global_arg_state = args_advance_token(arg_state);
        if (global_arg_state->state == ARG_STATE_ERR) {
            _err_count++;
        }
        if (global_arg_state->state >= ARG_STATE_END) {
#ifdef REQUIRE_FULL_CMD
            _err_count += _on_cmd_end(global_cmd_name, global_cmd);
#endif
            break;
        }
        global_arg = global_arg_state->arg;

        // Default to no error for this argument.
        global_err = 0;

        // ==================================================================
        // Check end-of-command first.
        //   This allows recognizing "; ;" as okay.

        if (strequal("&&", global_arg)) {
#ifdef REQUIRE_FULL_CMD
            _err_count += _on_cmd_end(global_cmd_name, global_cmd);
#endif
            if (_err_count > 0) {
                // && with errors stops the shell.
                stderrP("FAIL &&\n");
                break;
            }
            LOG(":: &&\n");
            global_cmd = COMMAND_INDEX__FIND_CMD;
        } else

        if (strequal(";", global_arg)) {
#ifdef REQUIRE_FULL_CMD
            _err_count += _on_cmd_end(global_cmd_name, global_cmd);
#endif
            // ";" ignores any errors, resetting the error count.
            LOG(":: ;\n");
            _err_count = 0;
            global_cmd = COMMAND_INDEX__FIND_CMD;
        } else {
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
