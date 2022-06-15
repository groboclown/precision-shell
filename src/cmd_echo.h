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

#ifndef _FS_SHELL__CMD_ECHO_

#include "uses.h"

#ifdef USE_CMD_ECHO

#include "command_def.h"
#include "command_common.h"


extern const char command_name_echo[];
int cmd_echo_run();


// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define ENUM__CMD_ECHO        COMMAND_INDEX__ECHO
#define NAME__CMD_ECHO        "echo"
#define NAMEVAR__CMD_ECHO     command_name_echo
#define SETUP__CMD_ECHO       command_common_setup_identity
#define RUN__CMD_ECHO         cmd_echo_run
#define INIT__CMD_ECHO


// Extrapolated defines
#define ENUM_LIST__CMD_ECHO   ENUM__CMD_ECHO,
#define NAME_TC__CMD_ECHO     NAME__CMD_ECHO,
#define NAME_VC__CMD_ECHO     NAMEVAR__CMD_ECHO,
#define NAME_VS__CMD_ECHO     names[COMMAND_INDEX__ECHO] = NAMEVAR__CMD_ECHO;
#define SETUP_C__CMD_ECHO     &SETUP__CMD_ECHO,
#define SETUP_S__CMD_ECHO     setups[COMMAND_INDEX__ECHO] = &SETUP__CMD_ECHO;
#define RUN_C__CMD_ECHO       &RUN__CMD_ECHO,
#define RUN_S__CMD_ECHO       runs[COMMAND_INDEX__ECHO] = &RUN__CMD_ECHO;
#define CALL_INIT__CMD_ECHO




#else /* USE_CMD_ECHO */
#define ENUM_LIST__CMD_ECHO
#define NAME_TC__CMD_ECHO
#define NAME_VC__CMD_ECHO
#define NAME_VS__CMD_ECHO
#define SETUP_C__CMD_ECHO
#define SETUP_S__CMD_ECHO
#define RUN_C__CMD_ECHO
#define RUN_S__CMD_ECHO
#define CALL_INIT__CMD_ECHO
#endif /* USE_CMD_ECHO */
#endif /* _FS_SHELL__CMD_ECHO_ */
