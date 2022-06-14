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

// Find command - always included.

#include "output.h"
#include "globals.h"
#include "helpers.h"
#include "command_common.h"
#include "command_list.h"
#include "cmd_find_cmd.h"


// cmd_find_cmd_run__func match the current arg against the command list.
int cmd_find_cmd_run__func() {
    const char **command_list_names = get_command_list_names();
    const CommandSetup *command_setup = get_command_setup();

    // No matter what this finds, the command name is the current argument.
    global_cmd_name = global_arg;

    // Note that the loop includes find_cmd, to keep the finding going,
    // and does not include err.
    for (int idx = COMMAND_INDEX__FIND_CMD; idx < COMMAND_INDEX__ERR; idx++) {
        if (strequal(global_arg, command_list_names[idx])) {
            global_cmd = command_setup[idx](idx);
            return 0;
        }
    }
    global_cmd = COMMAND_INDEX__ERR;
    return 1;
}

extern const CommandFunc cmd_find_cmd_run = &cmd_find_cmd_run__func;
