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

#ifndef _FS_SHELL__CMD_MV_

#include "uses.h"

#ifdef USE_CMD_MV

#include "command_def.h"
#include "command_common.h"


extern const char command_name_mv[];
int cmd_mv_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_MV        COMMAND_INDEX__MV
#define NAME__CMD_MV        "mv"
#define NAMEVAR__CMD_MV     command_name_mv
#define SETUP__CMD_MV       command_common_setup_identity
#define RUN__CMD_MV         cmd_mv_run
#define INIT__CMD_MV


// Extrapolated defines
#define ENUM_LIST__CMD_MV   ENUM__CMD_MV, COMMAND_INDEX__MV__RUN,
#define NAME_TC__CMD_MV     NAME__CMD_MV, EMPTY_STRING,
#define NAME_VC__CMD_MV     NAMEVAR__CMD_MV, command_common_empty_name,
#define NAME_VS__CMD_MV     names[COMMAND_INDEX__MV] = NAMEVAR__CMD_MV; \
                              names[COMMAND_INDEX__MV__RUN] = command_common_empty_name;
#define SETUP_C__CMD_MV     &SETUP__CMD_MV, &command_common_setup_identity,
#define SETUP_S__CMD_MV     setups[COMMAND_INDEX__MV] = &SETUP__CMD_MV; \
                              setups[COMMAND_INDEX__MV__RUN] = &command_common_setup_identity;
#define RUN_C__CMD_MV       &command_common_run_store_arg, &RUN__CMD_MV,
#define RUN_S__CMD_MV       runs[COMMAND_INDEX__MV] = &command_common_run_store_arg; \
                              runs[COMMAND_INDEX__MV__RUN] = &RUN__CMD_MV;
#define CALL_INIT__CMD_MV




#else /* USE_CMD_MV */
#define ENUM_LIST__CMD_MV
#define NAME_TC__CMD_MV
#define NAME_VC__CMD_MV
#define NAME_VS__CMD_MV
#define SETUP_C__CMD_MV
#define SETUP_S__CMD_MV
#define RUN_C__CMD_MV
#define RUN_S__CMD_MV
#define CALL_INIT__CMD_MV
#endif /* USE_CMD_MV */
#endif /* _FS_SHELL__CMD_MV_ */
