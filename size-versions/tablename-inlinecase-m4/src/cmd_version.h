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
#define _FS_SHELL__CMD_VERSION_

#include "uses.h"

// Version is required.

#include "force-inline.h"
#include "output.h"
#include "version.h"
#include "globals.h"


// Command COMMAND_INDEX__VERSION
#define NAME__CMD_VERSION              "version"
#define NAMEVAR__CMD_VERSION           cmd_name_version

// As a header, this defines the external existance.
//   The physical location is put into the global section below.
extern const char cmd_name_version[];

// Operations.  Defined as functions, but marked as "inline" to
// force 

forceinline void startup_cmd_version() {
    int val; \
    // stdoutP(global_invoked_name);
    stdoutP("fs-shell");
    stdoutP(VERSION_STR);
    for (val = 0; val < cmd_err_index; val++) {
        // Only output included commands that are not virtual.
        if (command_list_names[val][0] != 0) {
            stdoutP(" +");
            stdoutP(command_list_names[val]);
        }
    }
    stdoutP("\n");
    // should be no arguments, so immediately switch to error mode.
    global_cmd = cmd_err_index;
}


// Derived values
//   These are the ones referenced by command_list.h
//   While there can be multiple of the above values, they are
//   assembled into defines referenced elsewhere only in the following
//   values.
//   The operation in these defines can be modified at will.
//   They will be inserted in-line where needed.


// List of enum names, inserted into the enum list; must be comma terminated.
//   If this file defines multiple enum values, this needs to be
//   a comma separated list of each name.
#define ENUM_LIST__VERSION \
    COMMAND_INDEX__VERSION,


// Initialize the command name array with the name.
//   Note that this points to the constant variable, not a string.
#define SET_NAME__VERSION \
    names[COMMAND_INDEX__VERSION] = NAMEVAR__CMD_VERSION;


// Global/static variables that are available to all functions.
//   Each one of these must have an "extern" defined in this file.
#define GLOBAL_VARDEF__VERSION \
    const char NAMEVAR__CMD_VERSION[] = NAME__CMD_VERSION;


#define INITIALIZE__VERSION


// The case/call/break statement(s).
#define STARTUP_CASE__VERSION \
case COMMAND_INDEX__VERSION: \
    startup_cmd_version(); \
    break;


// The case/call/break statement(s).
#define RUN_CASE__VERSION

#endif /* _FS_SHELL__CMD_VERSION_ */
