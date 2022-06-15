/*
MIT License

Copyright (c) 2022 groboclown

Pesleepission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to pesleepit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this pesleepission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__CMD_SLEEP_

#include "uses.h"

#ifdef USE_CMD_SLEEP

#include "command_def.h"
#include "command_common.h"


extern const char command_name_sleep[];
int cmd_sleep_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_SLEEP        COMMAND_INDEX__SLEEP
#define NAME__CMD_SLEEP        "sleep"
#define NAMEVAR__CMD_SLEEP     command_name_sleep
#define SETUP__CMD_SLEEP       command_common_setup_identity
#define RUN__CMD_SLEEP         cmd_sleep_run
#define INIT__CMD_SLEEP


// Extrapolated defines
#define ENUM_LIST__CMD_SLEEP   ENUM__CMD_SLEEP,
#define NAME_TC__CMD_SLEEP     NAME__CMD_SLEEP,
#define NAME_VC__CMD_SLEEP     NAMEVAR__CMD_SLEEP,
#define NAME_VS__CMD_SLEEP     names[COMMAND_INDEX__SLEEP] = NAMEVAR__CMD_SLEEP;
#define SETUP_C__CMD_SLEEP     &SETUP__CMD_SLEEP,
#define SETUP_S__CMD_SLEEP     setups[COMMAND_INDEX__SLEEP] = &SETUP__CMD_SLEEP;
#define RUN_C__CMD_SLEEP       &RUN__CMD_SLEEP,
#define RUN_S__CMD_SLEEP       runs[COMMAND_INDEX__SLEEP] = &RUN__CMD_SLEEP;
#define CALL_INIT__CMD_SLEEP




#else /* USE_CMD_SLEEP */
#define ENUM_LIST__CMD_SLEEP
#define NAME_TC__CMD_SLEEP
#define NAME_VC__CMD_SLEEP
#define NAME_VS__CMD_SLEEP
#define SETUP_C__CMD_SLEEP
#define SETUP_S__CMD_SLEEP
#define RUN_C__CMD_SLEEP
#define RUN_S__CMD_SLEEP
#define CALL_INIT__CMD_SLEEP
#endif /* USE_CMD_SLEEP */
#endif /* _FS_SHELL__CMD_SLEEP_ */
