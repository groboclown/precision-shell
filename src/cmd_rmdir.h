/*
MIT License

Copyright (c) 2022 groboclown

Permdirission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permdirit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permdirission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__CMD_RMDIR_

#include "uses.h"

#ifdef USE_CMD_RMDIR

#include "command_def.h"
#include "command_common.h"


extern const char command_name_rmdir[];
int cmd_rmdir_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_RMDIR        COMMAND_INDEX__RMDIR
#define NAME__CMD_RMDIR        "rmdir"
#define NAMEVAR__CMD_RMDIR     command_name_rmdir
#define SETUP__CMD_RMDIR       command_common_setup_identity
#define RUN__CMD_RMDIR         cmd_rmdir_run
#define INIT__CMD_RMDIR


// Extrapolated defines
#define ENUM_LIST__CMD_RMDIR   ENUM__CMD_RMDIR,
#define NAME_TC__CMD_RMDIR     NAME__CMD_RMDIR,
#define NAME_VC__CMD_RMDIR     NAMEVAR__CMD_RMDIR,
#define NAME_VS__CMD_RMDIR     names[COMMAND_INDEX__RMDIR] = NAMEVAR__CMD_RMDIR;
#define SETUP_C__CMD_RMDIR     &SETUP__CMD_RMDIR,
#define SETUP_S__CMD_RMDIR     setups[COMMAND_INDEX__RMDIR] = &SETUP__CMD_RMDIR;
#define RUN_C__CMD_RMDIR       &RUN__CMD_RMDIR,
#define RUN_S__CMD_RMDIR       runs[COMMAND_INDEX__RMDIR] = &RUN__CMD_RMDIR;
#define CALL_INIT__CMD_RMDIR




#else /* USE_CMD_RMDIR */
#define ENUM_LIST__CMD_RMDIR
#define NAME_TC__CMD_RMDIR
#define NAME_VC__CMD_RMDIR
#define NAME_VS__CMD_RMDIR
#define SETUP_C__CMD_RMDIR
#define SETUP_S__CMD_RMDIR
#define RUN_C__CMD_RMDIR
#define RUN_S__CMD_RMDIR
#define CALL_INIT__CMD_RMDIR
#endif /* USE_CMD_RMDIR */
#endif /* _FS_SHELL__CMD_RMDIR_ */
