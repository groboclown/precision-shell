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

// version is universal and does not have a ifdef

#include "version.h"
#include "output.h"
#include "globals.h"
#include "command_list.h"


int cmd_version_setup(int idx) {
    stdoutP(global_invoked_name);
    stdoutP(VERSION_STR);
    for (idx = COMMAND_INDEX__NOOP; idx < COMMAND_INDEX__ERR; idx++) {
        // Only output included commands that are not virtual.
        if (command_list_names[idx][0] != 0) {
            stdoutP(" +");
            stdoutP(command_list_names[idx]);
        }
    }
    stdoutP("\n");
    // should be no arguments, so immediately switch to error mode.
    return COMMAND_INDEX__ERR;
}
