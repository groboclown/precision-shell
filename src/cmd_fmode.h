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

#ifndef _FS_SHELL__CMD_FMODE_

#include "uses.h"

#ifdef USE_CMD_FMODE

#include "command_def.h"
#include "command_common.h"


extern const char command_name_fmode[];
int cmd_fmode_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_FMODE        COMMAND_INDEX__FMODE
#define NAME__CMD_FMODE        "fmode"
#define NAMEVAR__CMD_FMODE     command_name_fmode
#define SETUP__CMD_FMODE       command_common_setup_identity
#define RUN__CMD_FMODE         cmd_fmode_run
#define INIT__CMD_FMODE


// Extrapolated defines
#define ENUM_LIST__CMD_FMODE   ENUM__CMD_FMODE,
#define NAME_TC__CMD_FMODE     NAME__CMD_FMODE,
#define NAME_VC__CMD_FMODE     NAMEVAR__CMD_FMODE,
#define NAME_VS__CMD_FMODE     names[COMMAND_INDEX__FMODE] = NAMEVAR__CMD_FMODE;
#define SETUP_C__CMD_FMODE     &SETUP__CMD_FMODE,
#define SETUP_S__CMD_FMODE     setups[COMMAND_INDEX__FMODE] = &SETUP__CMD_FMODE;
#define RUN_C__CMD_FMODE       &RUN__CMD_FMODE,
#define RUN_S__CMD_FMODE       runs[COMMAND_INDEX__FMODE] = &RUN__CMD_FMODE;
#define CALL_INIT__CMD_FMODE




#else /* USE_CMD_FMODE */
#define ENUM_LIST__CMD_FMODE
#define NAME_TC__CMD_FMODE
#define NAME_VC__CMD_FMODE
#define NAME_VS__CMD_FMODE
#define SETUP_C__CMD_FMODE
#define SETUP_S__CMD_FMODE
#define RUN_C__CMD_FMODE
#define RUN_S__CMD_FMODE
#define CALL_INIT__CMD_FMODE
#endif /* USE_CMD_FMODE */
#endif /* _FS_SHELL__CMD_FMODE_ */
