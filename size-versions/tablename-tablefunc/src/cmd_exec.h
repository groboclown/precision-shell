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

#ifndef _FS_SHELL__CMD_EXEC_

#include "uses.h"

#ifdef USE_CMD_EXEC

#include "command_def.h"
#include "command_common.h"


extern const char command_name_exec[];
int cmd_exec_setup(int);


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_EXEC        COMMAND_INDEX__EXEC
#define NAME__CMD_EXEC        "exec"
#define NAMEVAR__CMD_EXEC     command_name_exec
#define SETUP__CMD_EXEC       cmd_exec_setup
#define RUN__CMD_EXEC         command_common_run_ok
#define INIT__CMD_EXEC


// Extrapolated defines
#define ENUM_LIST__CMD_EXEC   ENUM__CMD_EXEC,
#define NAME_TC__CMD_EXEC     NAME__CMD_EXEC,
#define NAME_VC__CMD_EXEC     NAMEVAR__CMD_EXEC,
#define NAME_VS__CMD_EXEC     names[COMMAND_INDEX__EXEC] = NAMEVAR__CMD_EXEC;
#define SETUP_C__CMD_EXEC     &SETUP__CMD_EXEC,
#define SETUP_S__CMD_EXEC     setups[COMMAND_INDEX__EXEC] = &SETUP__CMD_EXEC;
#define RUN_C__CMD_EXEC       &RUN__CMD_EXEC,
#define RUN_S__CMD_EXEC       runs[COMMAND_INDEX__EXEC] = &RUN__CMD_EXEC;
#define CALL_INIT__CMD_EXEC




#else /* USE_CMD_EXEC */
#define ENUM_LIST__CMD_EXEC
#define NAME_TC__CMD_EXEC
#define NAME_VC__CMD_EXEC
#define NAME_VS__CMD_EXEC
#define SETUP_C__CMD_EXEC
#define SETUP_S__CMD_EXEC
#define RUN_C__CMD_EXEC
#define RUN_S__CMD_EXEC
#define CALL_INIT__CMD_EXEC
#endif /* USE_CMD_EXEC */
#endif /* _FS_SHELL__CMD_EXEC_ */
