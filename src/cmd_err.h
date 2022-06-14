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

#ifndef _FS_SHELL__CMD_ERR_

#include "uses.h"
#include "command_def.h"
#include "command_common.h"

// err is always included

int cmd_err_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_ERR        COMMAND_INDEX__ERR
#define NAME__CMD_ERR        EMPTY_STRING
#define NAMEVAR__CMD_ERR     command_common_empty_name
#define SETUP__CMD_ERR       command_common_setup_identity
#define RUN__CMD_ERR         cmd_err_run
#define INIT__CMD_ERR


// Extrapolated defines
#define ENUM_LIST__CMD_ERR   ENUM__CMD_ERR,
#define NAME_T__CMD_ERR      NAME__CMD_ERR
#define NAME_TC__CMD_ERR     NAME__CMD_ERR,
#define NAME_VC__CMD_ERR     NAMEVAR__CMD_ERR,
#define NAME_VS__CMD_ERR     names[COMMAND_INDEX__ERR] = NAMEVAR__CMD_ERR;
#define SETUP_C__CMD_ERR     &SETUP__CMD_ERR,
#define SETUP_S__CMD_ERR     setups[COMMAND_INDEX__ERR] = &SETUP__CMD_ERR;
#define RUN_C__CMD_ERR       &RUN__CMD_ERR,
#define RUN_S__CMD_ERR       runs[COMMAND_INDEX__ERR] = &RUN__CMD_ERR;
#define CALL_INIT__CMD_ERR


#endif /* _FS_SHELL__CMD_X_ */
