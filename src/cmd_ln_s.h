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

#ifndef _FS_SHELL__CMD_LN_S_

#include "uses.h"

#ifdef USE_CMD_LN_S

#include "command_def.h"
#include "command_common.h"


extern const char command_name_ln_s[];
int cmd_ln_s_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_LN_S        COMMAND_INDEX__LN_S
#define NAME__CMD_LN_S        "ln-s"
#define NAMEVAR__CMD_LN_S     command_name_ln_s
#define SETUP__CMD_LN_S       command_common_setup_identity
#define RUN__CMD_LN_S         cmd_ln_s_run
#define INIT__CMD_LN_S


// Extrapolated defines
#define ENUM_LIST__CMD_LN_S   ENUM__CMD_LN_S, ENUM__CMD_LN_S__RUN,
#define NAME_TC__CMD_LN_S     NAME__CMD_LN_S, EMPTY_STRING,
#define NAME_VC__CMD_LN_S     NAMEVAR__CMD_LN_S, command_common_empty_name,
#define NAME_VS__CMD_LN_S     names[COMMAND_INDEX__LN_S] = NAMEVAR__CMD_LN_S; \
                              names[COMMAND_INDEX__LN_S__RUN] = command_common_empty_name;
#define SETUP_C__CMD_LN_S     &SETUP__CMD_LN_S, &command_common_setup_identity,
#define SETUP_S__CMD_LN_S     setups[COMMAND_INDEX__LN_S] = &SETUP__CMD_LN_S; \
                              setups[COMMAND_INDEX__LN_S__RUN] = &command_common_setup_identity;
#define RUN_C__CMD_LN_S       &command_common_run_store_arg, &RUN__CMD_LN_S,
#define RUN_S__CMD_LN_S       runs[COMMAND_INDEX__LN_S] = &command_common_run_store_arg; \
                              runs[COMMAND_INDEX__LN_S] = &RUN__CMD_LN_S;
#define CALL_INIT__CMD_LN_S




#else /* USE_CMD_LN_S */
#define ENUM_LIST__CMD_LN_S
#define NAME_TC__CMD_LN_S
#define NAME_VC__CMD_LN_S
#define NAME_VS__CMD_LN_S
#define SETUP_C__CMD_LN_S
#define SETUP_S__CMD_LN_S
#define RUN_C__CMD_LN_S
#define RUN_S__CMD_LN_S
#define CALL_INIT__CMD_LN_S
#endif /* USE_CMD_LN_S */
#endif /* _FS_SHELL__CMD_LN_S_ */
