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

#ifndef _FS_SHELL__CMD_MKDIR_

#include "uses.h"

#ifdef USE_CMD_MKDIR

#include "command_def.h"
#include "command_common.h"


extern const char command_name_mkdir[];
int cmd_mkdir_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_MKDIR        COMMAND_INDEX__MKDIR
#define NAME__CMD_MKDIR        "mkdir"
#define NAMEVAR__CMD_MKDIR     command_name_mkdir
#define SETUP__CMD_MKDIR       command_common_setup_identity
#define RUN__CMD_MKDIR         cmd_mkdir_run
#define INIT__CMD_MKDIR


// Extrapolated defines
#define ENUM_LIST__CMD_MKDIR   ENUM__CMD_MKDIR,
#define NAME_TC__CMD_MKDIR     NAME__CMD_MKDIR,
#define NAME_VC__CMD_MKDIR     NAMEVAR__CMD_MKDIR,
#define NAME_VS__CMD_MKDIR     names[COMMAND_INDEX__MKDIR] = NAMEVAR__CMD_MKDIR;
#define SETUP_C__CMD_MKDIR     &SETUP__CMD_MKDIR,
#define SETUP_S__CMD_MKDIR     setups[COMMAND_INDEX__MKDIR] = &SETUP__CMD_MKDIR;
#define RUN_C__CMD_MKDIR       &RUN__CMD_MKDIR,
#define RUN_S__CMD_MKDIR       runs[COMMAND_INDEX__MKDIR] = &RUN__CMD_MKDIR;
#define CALL_INIT__CMD_MKDIR




#else /* USE_CMD_MKDIR */
#define ENUM_LIST__CMD_MKDIR
#define NAME_TC__CMD_MKDIR
#define NAME_VC__CMD_MKDIR
#define NAME_VS__CMD_MKDIR
#define SETUP_C__CMD_MKDIR
#define SETUP_S__CMD_MKDIR
#define RUN_C__CMD_MKDIR
#define RUN_S__CMD_MKDIR
#define CALL_INIT__CMD_MKDIR
#endif /* USE_CMD_MKDIR */
#endif /* _FS_SHELL__CMD_MKDIR_ */
