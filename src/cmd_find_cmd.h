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

#ifndef _FS_SHELL__CMD_FIND_CMD_
#define _FS_SHELL__CMD_FIND_CMD_

// A very special command.  Must always be included.

#include "uses.h"
#include "command_def.h"
#include "command_common.h"


extern const CommandFunc cmd_find_cmd_run;


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_FIND_CMD        COMMAND_INDEX__FIND_CMD
#define NAME__CMD_FIND_CMD        EMPTY_STRING
#define NAMEVAR__CMD_FIND_CMD     command_common_empty_name
#define SETUP__CMD_FIND_CMD       command_common_setup_identity
#define RUN__CMD_FIND_CMD         cmd_find_cmd_run
#define INIT__CMD_FIND_CMD


// Extrapolated defines
#define ENUM_LIST__CMD_FIND_CMD   ENUM__CMD_FIND_CMD,
#define NAME_TC__CMD_FIND_CMD     NAME__CMD_FIND_CMD,
#define NAME_VC__CMD_FIND_CMD     NAMEVAR__CMD_FIND_CMD,
#define NAME_VS__CMD_FIND_CMD     names[COMMAND_INDEX__FIND_CMD] = NAMEVAR__CMD_FIND_CMD;
#define SETUP_C__CMD_FIND_CMD     SETUP__CMD_FIND_CMD,
#define SETUP_S__CMD_FIND_CMD     setups[COMMAND_INDEX__FIND_CMD] = SETUP__CMD_FIND_CMD;
#define RUN_C__CMD_FIND_CMD       RUN__CMD_FIND_CMD,
#define RUN_S__CMD_FIND_CMD       runs[COMMAND_INDEX__FIND_CMD] = RUN__CMD_FIND_CMD,
#define CALL_INIT__CMD_FIND_CMD



extern const char *command_name_find_cmd;
extern const CommandFunc cmd_find_cmd_run;

#endif /* _FS_SHELL__CMD_FIND_CMD_ */
