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

#ifdef USE_CMD_SIGNAL

#include <signal.h>

#include "output.h"
#include "globals.h"
#include "helpers.h"
#include "command_common.h"
#include "command_list.h"
#include "cmd_signal.h"


const char NAMEVAR__CMD_SIGNAL[] = NAME__CMD_SIGNAL;


void signal_empty_handler(int signal) {
    LOG(":: handled signal\n");
}


int cmd_signal_setup(int idx) {
    sigemptyset(&global_signal_set);
    return idx;
}


int cmd_signal_run() {
    int val;
    int err;

    // The "wait" string indicates the end of the signals.
    if (strequal("wait", global_arg)) {
        LOG(":: start signal wait\n");
        // Early POSIX Draft 6 has sigwait() return the signal number.
        // New standard has sigwait() return 0 for no error, or the error number.
        // Dietlibc uses the old return code.
        // There's a small chance that an error occurs and that error code
        //   matches the signal.
        val = 0;
        err = sigwait(&global_signal_set, &val);
#ifdef DEBUG
printf(":: sigwait() returned %i, signal %i\n", err, val);
#endif
        LOG(":: wait complete\n");

        // it doesn't make sense to keep parsing signals at this point.
        global_cmd = COMMAND_INDEX__ERR;

        if (err != 0 && err != val) {
            // Due to the initialization and the old way of working,
            //   this situation means that this is a real error situation.
            return 1;
        }
        // Looks like a success.
        return 0;
    }
    // "wait" hasn't been found yet, so each argument is a
    // signal number.
    val = helper_arg_to_uint(10, 0xffff);
    if (val < 0) {
        // Do not allow the signal to wait.  This can lead an invalid operations.
        global_cmd = COMMAND_INDEX__ERR;
        return 1;
    }
    LOG(":: signal ");
    LOGLN(global_arg);
    sigaddset(&global_signal_set, val);
    signal(val, &signal_empty_handler);
    return 0;
}



#else
// disable pedantic warning
typedef int iso_translation_unit__signal;
#endif /* USE_CMD_SIGNAL */
