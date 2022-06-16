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

#ifndef _FS_SHELL__CMD_ERR_
#define _FS_SHELL__CMD_ERR_

#include "uses.h"

// err is always present.

#include "force-inline.h"
#include "globals.h"
#include "helpers.h"


// Command COMMAND_INDEX__ERR
#define NAME__CMD_ERR              EMPTY_STRING
#define NAMEVAR__CMD_ERR           empty_string


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
#define ENUM_LIST__ERR \
    COMMAND_INDEX__ERR,


// Initialize the command name array with the name.
//   Note that this points to the constant variable, not a string.
#define SET_NAME__ERR \
    names[COMMAND_INDEX__ERR] = NAMEVAR__CMD_ERR;


// Global/static variables that are available to all functions.
//   Each one of these must have an "extern" defined in this file.
#define GLOBAL_VARDEF__ERR


#define INITIALIZE__ERR
#define STARTUP_CASE__ERR


// The case/call/break statement(s).
#define RUN_CASE__ERR \
case COMMAND_INDEX__ERR: \
    global_err = 1; \
    break;

#endif /* _FS_SHELL__CMD_ERR_ */
