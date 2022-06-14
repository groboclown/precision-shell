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

#ifndef _FS_SHELL__CMD_VERSION_

#define NAME__VERSION "version"

// Version is always included, so no ifdef/else around it.

#define STARTUP__COMMAND_INDEX__VERSION \
else if (strequal(global_arg, NAME__VERSION)) { \
    /* stdoutP(global_invoked_name); */ \
    stdoutP("fs-shell"); \
    stdoutP(VERSION_STR); \
    for (global_arg1_i = COMMAND_INDEX__NOOP; global_arg1_i < COMMAND_INDEX__ERR; global_arg1_i++) { \
        /* Only output included commands that are not virtual. */ \
        if (command_list_names[global_arg1_i][0] != 0) { \
            stdoutP(" +"); \
            stdoutP(command_list_names[global_arg1_i]); \
        } \
    } \
    stdoutP("\n"); \
    /* should be no arguments, so immediately switch to error mode. */ \
    global_cmd = COMMAND_INDEX__ERR; \
}

// No case execution
#define CASE__COMMAND_INDEX__VERSION

#endif /* _FS_SHELL__CMD_VERSION_ */
