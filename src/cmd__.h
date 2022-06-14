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

#ifndef _FS_SHELL__CMD_XX_

#include "uses.h"

#ifdef USE_CMD_XX

#include "command_def.h"
#include "command_common.h"


extern const char *command_name_xx;
int cmd_xx_init();
int cmd_xx_setup(int);
int cmd_xx_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_XX        COMMAND_INDEX__XX
#define NAME__CMD_XX        "xx"
#define NAMEVAR__CMD_XX     command_name_xx
#define SETUP__CMD_XX       cmd_xx_setup
#define RUN__CMD_XX         cmd_xx_run
#define INIT__CMD_XX        cmd_xx_init


// Extrapolated defines
#define ENUM_LIST__CMD_XX   ENUM__CMD_XX,
#define NAME_T__CMD_XX      NAME__CMD_XX
#define NAME_TC__CMD_XX     NAME__CMD_XX,
#define NAME_VC__CMD_XX     NAMEVAR__CMD_XX,
#define NAME_VS__CMD_XX     names[COMMAND_INDEX__XX] = NAMEVAR__CMD_XX;
#define SETUP_C__CMD_XX     &SETUP__CMD_XX,
#define SETUP_S__CMD_XX     setups[COMMAND_INDEX__XX] = &SETUP__CMD_XX;
#define RUN_C__CMD_XX       &RUN__CMD_XX,
#define RUN_S__CMD_XX       runs[COMMAND_INDEX__XX] = &RUN__CMD_XX;
#define CALL_INIT__CMD_XX   err += INIT__CMD_XX();




#else /* USE_CMD_XX */
#define ENUM_LIST__CMD_XX
#define NAME_TC__CMD_XX
#define NAME_VC__CMD_XX
#define NAME_VS__CMD_XX
#define SETUP_C__CMD_XX
#define SETUP_S__CMD_XX
#define RUN_C__CMD_XX
#define RUN_S__CMD_XX
#define CALL_INIT__CMD_XX
#endif /* USE_CMD_XX */
#endif /* _FS_SHELL__CMD_XX_ */
