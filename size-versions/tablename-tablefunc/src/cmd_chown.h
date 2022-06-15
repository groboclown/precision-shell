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

#ifndef _FS_SHELL__CMD_CHOWN_

#include "uses.h"

#ifdef USE_CMD_CHOWN

#include "command_def.h"
#include "command_common.h"


extern const char command_name_chown[];
int cmd_chown_run__file();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_CHOWN        COMMAND_INDEX__CHOWN
#define NAME__CMD_CHOWN        "chown"
#define NAMEVAR__CMD_CHOWN     command_name_chown
#define SETUP__CMD_CHOWN       command_common_setup_identity
#define INIT__CMD_CHOWN


// Extrapolated defines
#define ENUM_LIST__CMD_CHOWN   ENUM__CMD_CHOWN, COMMAND_INDEX__CHOWN__GROUP, COMMAND_INDEX__CHOWN__FILE,
#define NAME_TC__CMD_CHOWN     NAME__CMD_CHOWN, EMPTY_STRING, EMPTY_STRING,
#define NAME_VC__CMD_CHOWN     NAMEVAR__CMD_CHOWN, command_common_empty_name, command_common_empty_name,
#define NAME_VS__CMD_CHOWN     names[COMMAND_INDEX__CHOWN] = NAMEVAR__CMD_CHOWN; \
                               names[COMMAND_INDEX__CHOWN__GROUP] = command_common_empty_name; \
                               names[COMMAND_INDEX__CHOWN__FILE] = command_common_empty_name;
#define SETUP_C__CMD_CHOWN     &SETUP__CMD_CHOWN, &command_common_setup_identity, &command_common_setup_identity,
#define SETUP_S__CMD_CHOWN     setups[COMMAND_INDEX__CHOWN] = &command_common_setup_identity; \
                               setups[COMMAND_INDEX__CHOWN__GROUP] = &command_common_setup_identity; \
                               setups[COMMAND_INDEX__CHOWN__FILE] = &command_common_setup_identity;
#define RUN_C__CMD_CHOWN       &command_common_run_toint10, &command_common_run_toint10, &cmd_chown_run__file,
#define RUN_S__CMD_CHOWN       runs[COMMAND_INDEX__CHOWN] = &command_common_run_toint10; \
                               runs[COMMAND_INDEX__CHOWN__GROUP] = &command_common_run_toint10; \
                               runs[COMMAND_INDEX__CHOWN__FILE] = &cmd_chown_run__file;
#define CALL_INIT__CMD_CHOWN




#else /* USE_CMD_CHOWN */
#define ENUM_LIST__CMD_CHOWN
#define NAME_TC__CMD_CHOWN
#define NAME_VC__CMD_CHOWN
#define NAME_VS__CMD_CHOWN
#define SETUP_C__CMD_CHOWN
#define SETUP_S__CMD_CHOWN
#define RUN_C__CMD_CHOWN
#define RUN_S__CMD_CHOWN
#define CALL_INIT__CMD_CHOWN
#endif /* USE_CMD_CHOWN */
#endif /* _FS_SHELL__CMD_CHOWN_ */
