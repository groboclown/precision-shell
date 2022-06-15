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

#ifdef USE_CMD_FMODE

#include "output.h"
#include "globals.h"
#include "helpers.h"
#include "command_common.h"
#include "cmd_fmode.h"


const char NAMEVAR__CMD_FMODE[] = NAME__CMD_FMODE;


int cmd_fmode_run() {
    int val;

    // Nothing must run after this.
    global_cmd = COMMAND_INDEX__ERR;

    LOG(":: fmode ");
    LOGLN(global_arg);
    // File mode is only up to the first 3 nybbles.
    // Due to error checking, this will not change fmode unless it's okay.
    val = helper_arg_to_uint(global_arg, 8, 0777);
    if (val < 0) {
        return 1;
    }
    global_fmode = val;

    return 0;
}



#else
// disable pedantic warning
typedef int iso_translation_unit__fmode;
#endif /* USE_CMD_FMODE */
