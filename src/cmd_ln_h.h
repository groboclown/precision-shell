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

#ifndef _FS_SHELL__CMD_LN_H_

#include "uses.h"

#ifdef USE_CMD_LN_H

#include "command_def.h"
#include "command_common.h"


extern const char command_name_ln_h[];
int cmd_ln_h_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_LN_H        COMMAND_INDEX__LN_H
#define NAME__CMD_LN_H        "ln-h"
#define NAMEVAR__CMD_LN_H     command_name_ln_h
#define SETUP__CMD_LN_H       command_common_setup_identity
#define RUN__CMD_LN_H         cmd_ln_h_run
#define INIT__CMD_LN_H


// Extrapolated defines
#define ENUM_LIST__CMD_LN_H   ENUM__CMD_LN_H, COMMAND_INDEX__LN_H__RUN,
#define NAME_TC__CMD_LN_H     NAME__CMD_LN_H, EMPTY_STRING,
#define NAME_VC__CMD_LN_H     NAMEVAR__CMD_LN_H, command_common_empty_name,
#define NAME_VS__CMD_LN_H     names[COMMAND_INDEX__LN_H] = NAMEVAR__CMD_LN_H; \
                              names[COMMAND_INDEX__LN_H__RUN] = command_common_empty_name;
#define SETUP_C__CMD_LN_H     &SETUP__CMD_LN_H, &command_common_setup_identity,
#define SETUP_S__CMD_LN_H     setups[COMMAND_INDEX__LN_H] = &SETUP__CMD_LN_H; \
                              setups[COMMAND_INDEX__LN_H__RUN] = &command_common_setup_identity;
#define RUN_C__CMD_LN_H       &command_common_run_store_arg, &RUN__CMD_LN_H,
#define RUN_S__CMD_LN_H       runs[COMMAND_INDEX__LN_H] = &command_common_run_store_arg; \
                              runs[COMMAND_INDEX__LN_H] = &RUN__CMD_LN_H;
#define CALL_INIT__CMD_LN_H




#else /* USE_CMD_LN_H */
#define ENUM_LIST__CMD_LN_H
#define NAME_TC__CMD_LN_H
#define NAME_VC__CMD_LN_H
#define NAME_VS__CMD_LN_H
#define SETUP_C__CMD_LN_H
#define SETUP_S__CMD_LN_H
#define RUN_C__CMD_LN_H
#define RUN_S__CMD_LN_H
#define CALL_INIT__CMD_LN_H
#endif /* USE_CMD_LN_H */
#endif /* _FS_SHELL__CMD_LN_H_ */
