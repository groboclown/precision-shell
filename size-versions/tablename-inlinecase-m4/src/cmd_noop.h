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

#ifndef _FS_SHELL__CMD_NOOP_
#define _FS_SHELL__CMD_NOOP_

#include "uses.h"

#ifdef USE_CMD_NOOP

#include "force-inline.h"
#include "output.h"

// Command COMMAND_INDEX__NOOP
#define NAME__CMD_NOOP              "noop"
#define NAMEVAR__CMD_NOOP           cmd_name_noop

// As a header, this defines the external existance.
//   The physical location is put into the global section below.
extern const char cmd_name_noop[];

// Operations.  Defined as functions, but marked as "inline" to
// force 


// Run - runs the command.
forceinline void run_cmd_noop() {
    LOG(":: xx ");
    LOGLN(global_arg);
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
#define ENUM_LIST__NOOP \
    COMMAND_INDEX__NOOP,


// Initialize the command name array with the name.
//   Note that this points to the constant variable, not a string.
#define SET_NAME__NOOP \
    names[COMMAND_INDEX__NOOP] = NAMEVAR__CMD_NOOP;


// Global/static variables that are available to all functions.
//   Each one of these must have an "extern" defined in this file.
#define GLOBAL_VARDEF__NOOP \
    const char NAMEVAR__CMD_NOOP[] = NAME__CMD_NOOP;


#define INITIALIZE__NOOP \
    initialize_cmd_noop();


// The case/call/break statement(s).
#define STARTUP_CASE__NOOP \
case COMMAND_INDEX__NOOP: \
    cmd = startup_cmd_noop(cmd); \
    break;


// The case/call/break statement(s).
#define RUN_CASE__NOOP \
case COMMAND_INDEX__NOOP: \
    err = run_cmd_noop(); \
    break;


#else /* USE_CMD_NOOP */
#define ENUM_LIST__NOOP
#define SET_NAME__NOOP
#define GLOBAL_VARDEF__NOOP
#define INITIALIZE__NOOP
#define STARTUP_CASE__NOOP
#define RUN_CASE__NOOP
#endif /* USE_CMD_NOOP */

#endif /* _FS_SHELL__CMD_NOOP_ */
