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

#if defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV)

#include <sys/stat.h>
#include <sys/sysmacros.h>

#include "output.h"
#include "globals.h"
#include "helpers.h"
#include "command_common.h"
#include "command_list.h"
#include "cmd_mknod_mkdev.h"


int cmd_mknod_dev_type() {
    // Before this is called, global_arg1_i is set to the device number.
    // This sets global_arg2_i == device type.
    int val;

    LOG(":: create node with device type ");
    LOGLN(global_arg);
    global_cmd = COMMAND_INDEX__MKNOD_DEV__RUN;

    // just look at the first character
    val = global_arg[0];
    global_arg2_i = global_fmode;
    if (val == 'c' || val == 'u') {
        global_arg2_i |= S_IFCHR;
    } else if (val == 'b') {
        global_arg2_i |= S_IFBLK;
    } else if (val == 's') {
        global_arg2_i |= S_IFSOCK;
    } else if (val == 'p') {
        global_arg2_i |= S_IFIFO;
    } else {
        global_cmd = COMMAND_INDEX__ERR;
        return 1;
    }
    return 0;
}


int cmd_mknod_dev_run() {
    // Before this is called:
    //  - global_arg1_i is set to the device number.
    //  - global_arg2_i == device type.
    LOG(":: mknod/dev ");
    LOGLN(global_arg);
    // target file, mode, device
    return mknod(global_arg, global_arg2_i, global_arg1_i);
}



#ifdef USE_CMD_MKNOD

const char command_name_mknod[] = NAME__CMD_MKNOD;

int cmd_mknod_setup(int idx) {
    LOG(":: mknod setup\n");
    global_arg1_i = 0;
    return idx;
}

#endif /* USE_CMD_MKNOD */



#ifdef USE_CMD_MKDEV

const char command_name_mkdev[] = NAME__CMD_MKDEV;

int cmd_mkdev_run() {
    // The previous command parsed the major device number into global_arg1_i
    // This reads the minor device number, creates a device, and sets global_arg1_i to the device number.
    // This then calls into loading the device type then creates the device file.
    int val;

    LOG(":: mkdev minor device ");
    LOGLN(global_arg);
    global_cmd = COMMAND_INDEX__MKDEV_DEV_TYPE;
    val = helper_arg_to_uint(10, 0xffff);
    if (val == -1) {
        global_cmd = COMMAND_INDEX__ERR;
        return 1;
    } else { \
        global_arg1_i = makedev(global_arg1_i, val);
        if (global_arg1_i == -1) {
            global_cmd = COMMAND_INDEX__ERR;
            return 1;
        }
    }
    return 0;
}


#endif /* USE_CMD_MKDEV */

#else
// disable pedantic warning
typedef int iso_translation_unit__mknod_mkdev;
#endif /* defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV) */
