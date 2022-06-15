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

#if defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC)

#include <fcntl.h>

#include "output.h"
#include "globals.h"
#include "helpers.h"
#include "command_common.h"
#include "cmd_touch_trunc.h"


int cmd_touch_trunc_run() {
    int val;
    LOG(":: touch/trunc ");
    LOGLN(global_arg);
    val = open(
        global_arg, global_arg1_i, global_fmode
    );
    if (val == -1) {
        return 1;
    } else {
        close(val);
    }
    return 0;
}


#ifdef USE_CMD_TOUCH

const char command_name_touch[] = NAME__CMD_TOUCH;
int cmd_touch_setup(int idx) {
    LOG(":: preparing touch\n");
    global_arg1_i = O_WRONLY | O_CREAT;
    return idx;
}

#endif /* USE_CMD_TOUCH */

#ifdef USE_CMD_TRUNC

const char command_name_trunc[] = NAME__CMD_TRUNC;
int cmd_trunc_setup(int idx) {
    LOG(":: preparing trunc\n");
    global_arg1_i = O_WRONLY | O_CREAT | O_TRUNC;
    return idx;
}

#endif /* USE_CMD_TRUNC */

#else
// disable pedantic warning
typedef int iso_translation_unit__touch_trunc;
#endif /* defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC) */
