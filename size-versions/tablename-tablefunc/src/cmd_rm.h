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

#ifndef _FS_SHELL__CMD_RM_

#include "uses.h"

#ifdef USE_CMD_RM

#include "command_def.h"
#include "command_common.h"


extern const char command_name_rm[];
int cmd_rm_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_RM        COMMAND_INDEX__RM
#define NAME__CMD_RM        "rm"
#define NAMEVAR__CMD_RM     command_name_rm
#define SETUP__CMD_RM       command_common_setup_identity
#define RUN__CMD_RM         cmd_rm_run
#define INIT__CMD_RM


// Extrapolated defines
#define ENUM_LIST__CMD_RM   ENUM__CMD_RM,
#define NAME_TC__CMD_RM     NAME__CMD_RM,
#define NAME_VC__CMD_RM     NAMEVAR__CMD_RM,
#define NAME_VS__CMD_RM     names[COMMAND_INDEX__RM] = NAMEVAR__CMD_RM;
#define SETUP_C__CMD_RM     &SETUP__CMD_RM,
#define SETUP_S__CMD_RM     setups[COMMAND_INDEX__RM] = &SETUP__CMD_RM;
#define RUN_C__CMD_RM       &RUN__CMD_RM,
#define RUN_S__CMD_RM       runs[COMMAND_INDEX__RM] = &RUN__CMD_RM;
#define CALL_INIT__CMD_RM




#else /* USE_CMD_RM */
#define ENUM_LIST__CMD_RM
#define NAME_TC__CMD_RM
#define NAME_VC__CMD_RM
#define NAME_VS__CMD_RM
#define SETUP_C__CMD_RM
#define SETUP_S__CMD_RM
#define RUN_C__CMD_RM
#define RUN_S__CMD_RM
#define CALL_INIT__CMD_RM
#endif /* USE_CMD_RM */
#endif /* _FS_SHELL__CMD_RM_ */
