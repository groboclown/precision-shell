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

#include "globals.h"
#include "output.h"
#include "command_common.h"

const char *command_common_empty_name = EMPTY_STRING;

// command_common_setup_identity a common setup that uses the invoked command index.
int command_common_setup_identity__func(int idx) {
    LOG(":: setup ");
    LOGLN(global_arg);
    return idx;
}

const CommandSetup command_common_setup_identity = &command_common_setup_identity__func;


// command_common_run_toint10 Useful command magic.
//   - Shuffles global_arg1_i to global_arg2_i
//   - parses global_arg as a base 10 integer into global_arg1_i, with error checking.
//   - increments global_cmd
int command_common_run_toint10__func() {
    LOG(":: ");
    LOG(global_cmd_name);
    LOG(" arg to base10 int ");
    LOGLN(global_arg);
    int val = helper_arg_to_uint(10, 0xffff);
    if (val < 0) {
        return 1;
    }
    global_arg2_i = global_arg1_i;
    global_arg1_i = val;
    global_cmd++;
    return 0;
}

const CommandFunc command_common_run_toint10 = &command_common_run_toint10__func;


// command_common_run_store_arg Use command magic.
//   - sets global_arg_cached to the current argument
//   - increments global_cmd
int command_common_run_store_arg__func() {
    LOG(":: ");
    LOG(global_cmd_name);
    LOG(" storing argument ");
    LOGLN(global_arg);
    global_arg_cached = global_arg;
    global_cmd++;
    return 0;
}

const CommandFunc command_common_run_store_arg = &command_common_run_store_arg__func;


// command_common_run_ok a command runner that just returns no-error.
int command_common_run_ok__func() {
    LOG(":: ");
    LOG(global_cmd_name);
    LOG(" skipping ");
    LOGLN(global_arg);
    return 0;
}

const CommandFunc command_common_run_ok = &command_common_run_ok__func;
