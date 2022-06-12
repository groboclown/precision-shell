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

#ifdef USE_CMD_CHMOD

#include <sys/stat.h>
#include "output.h"
#include "globals.h"
#include "command_list.h"
#include "helpers.h"


int cmd_chmod_mod_arg() {
    // chmod has a special usage that allows for the 4 nybbles to be used.

    LOG(":: Parsing arg as base 8 in val1: ");
    LOGLN(global_arg);

    // file mode, which is octal in range [0, 07777].
    global_arg1_i = helper_arg_to_uint(8, 07777);
    if (global_arg1_i < 0) {
        LOG("::  - Bad base 8 number, or out of range\n");
        global_cmd = COMMAND_INDEX__ERR;
        return 1;
    }

    // Next argument is command + 1
    global_cmd++;

    return 0;
}


int cmd_chmod_run() {
    LOG(":: chmod ");
    LOGLN(global_arg);
    return chmod(global_arg, global_arg1_i);
}


#else /* USE_CMD_CHMOD */
// disable pedantic warning
typedef int iso_translation_unit_CHMOD;
#endif
