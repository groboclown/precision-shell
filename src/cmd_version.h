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

#ifndef _FS_SHELL__CMD_VERSION_
#define _FS_SHELL__CMD_VERSION_

#include "uses.h"
#include "command_def.h"
#include "command_common.h"

// version is required

extern const char command_name_version[];
int cmd_version_setup(int);


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_VERSION        COMMAND_INDEX__VERSION
#define NAME__CMD_VERSION        "version"
#define NAMEVAR__CMD_VERSION     command_name_version
#define SETUP__CMD_VERSION       cmd_version_setup
#define RUN__CMD_VERSION         command_common_run_ok
#define INIT__CMD_VERSION


// Extrapolated defines
#define ENUM_LIST__CMD_VERSION   ENUM__CMD_VERSION,
#define NAME_T__CMD_VERSION      NAME__CMD_VERSION
#define NAME_TC__CMD_VERSION     NAME__CMD_VERSION,
#define NAME_VC__CMD_VERSION     NAMEVAR__CMD_VERSION,
#define NAME_VS__CMD_VERSION     names[COMMAND_INDEX__VERSION] = NAMEVAR__CMD_VERSION;
#define SETUP_C__CMD_VERSION     &SETUP__CMD_VERSION,
#define SETUP_S__CMD_VERSION     setups[COMMAND_INDEX__VERSION] = &SETUP__CMD_VERSION;
#define RUN_C__CMD_VERSION       &RUN__CMD_VERSION,
#define RUN_S__CMD_VERSION       runs[COMMAND_INDEX__VERSION] = &RUN__CMD_VERSION,
#define CALL_INIT__CMD_VERSION




#endif /* _FS_SHELL__CMD_VERSION_ */
