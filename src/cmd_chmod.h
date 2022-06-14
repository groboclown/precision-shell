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

#ifndef _FS_SHELL__CMD_CHMOD_

#include "uses.h"

#ifdef USE_CMD_CHMOD

#include "command_def.h"
#include "command_common.h"


extern const char command_name_chmod[];
int cmd_chmod_init();
int cmd_chmod_run();
int cmd_chmod_run__file();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_CHMOD        COMMAND_INDEX__CHMOD__FMODE
#define NAME__CMD_CHMOD        "chmod"
#define NAMEVAR__CMD_CHMOD     command_name_chmod
#define SETUP__CMD_CHMOD       command_common_setup_identity
#define RUN__CMD_CHMOD         cmd_chmod_run
#define INIT__CMD_CHMOD


// Extrapolated defines
#define ENUM_LIST__CMD_CHMOD   ENUM__CMD_CHMOD, COMMAND_INDEX__CHMOD__FILE,
#define NAME_TC__CMD_CHMOD     NAME__CMD_CHMOD, EMPTY_STRING,
#define NAME_VC__CMD_CHMOD     NAMEVAR__CMD_CHMOD, command_common_empty_name,
#define NAME_VS__CMD_CHMOD     names[COMMAND_INDEX__CHMOD] = NAMEVAR__CMD_CHMOD; \
                               names[COMMAND_INDEX__CHMOD__FILE] = EMPTY_STRING;
#define SETUP_C__CMD_CHMOD     &SETUP__CMD_CHMOD, &command_common_setup_identity,
#define SETUP_S__CMD_CHMOD     setups[COMMAND_INDEX__CHMOD] = &SETUP__CMD_CHMOD; \
                               setups[COMMAND_INDEX__CHMOD__FILE] = &command_common_setup_identity;
#define RUN_C__CMD_CHMOD       &RUN__CMD_CHMOD, &cmd_chmod_run__file,
#define RUN_S__CMD_CHMOD       runs[COMMAND_INDEX__CHMOD] = &RUN__CMD_CHMOD; \
                               runs[COMMAND_INDEX__CHMOD__FILE] = &cmd_chmod_run__file;
#define CALL_INIT__CMD_CHMOD




#else /* USE_CMD_CHMOD */
#define ENUM_LIST__CMD_CHMOD
#define NAME_TC__CMD_CHMOD
#define NAME_VC__CMD_CHMOD
#define NAME_VS__CMD_CHMOD
#define SETUP_C__CMD_CHMOD
#define SETUP_S__CMD_CHMOD
#define RUN_C__CMD_CHMOD
#define RUN_S__CMD_CHMOD
#define CALL_INIT__CMD_CHMOD
#endif /* USE_CMD_CHMOD */
#endif /* _FS_SHELL__CMD_CHMOD_ */
