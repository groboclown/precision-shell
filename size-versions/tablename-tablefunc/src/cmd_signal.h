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

#ifndef _FS_SHELL__CMD_SIGNAL_

#include "uses.h"

#ifdef USE_CMD_SIGNAL

#include "command_def.h"
#include "command_common.h"


extern const char command_name_signal[];
int cmd_signal_setup(int);
int cmd_signal_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_SIGNAL        COMMAND_INDEX__SIGNAL
#define NAME__CMD_SIGNAL        "signal"
#define NAMEVAR__CMD_SIGNAL     command_name_signal
#define SETUP__CMD_SIGNAL       cmd_signal_setup
#define RUN__CMD_SIGNAL         cmd_signal_run
#define INIT__CMD_SIGNAL


// Extrapolated defines
#define ENUM_LIST__CMD_SIGNAL   ENUM__CMD_SIGNAL,
#define NAME_TC__CMD_SIGNAL     NAME__CMD_SIGNAL,
#define NAME_VC__CMD_SIGNAL     NAMEVAR__CMD_SIGNAL,
#define NAME_VS__CMD_SIGNAL     names[COMMAND_INDEX__SIGNAL] = NAMEVAR__CMD_SIGNAL;
#define SETUP_C__CMD_SIGNAL     &SETUP__CMD_SIGNAL,
#define SETUP_S__CMD_SIGNAL     setups[COMMAND_INDEX__SIGNAL] = &SETUP__CMD_SIGNAL;
#define RUN_C__CMD_SIGNAL       &RUN__CMD_SIGNAL,
#define RUN_S__CMD_SIGNAL       runs[COMMAND_INDEX__SIGNAL] = &RUN__CMD_SIGNAL;
#define CALL_INIT__CMD_SIGNAL




#else /* USE_CMD_SIGNAL */
#define ENUM_LIST__CMD_SIGNAL
#define NAME_TC__CMD_SIGNAL
#define NAME_VC__CMD_SIGNAL
#define NAME_VS__CMD_SIGNAL
#define SETUP_C__CMD_SIGNAL
#define SETUP_S__CMD_SIGNAL
#define RUN_C__CMD_SIGNAL
#define RUN_S__CMD_SIGNAL
#define CALL_INIT__CMD_SIGNAL
#endif /* USE_CMD_SIGNAL */
#endif /* _FS_SHELL__CMD_SIGNAL_ */
