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

#ifndef _FS_SHELL__CMD_DUP_

#include "uses.h"

#if defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R)


#include "command_def.h"
#include "command_common.h"

#ifdef USE_CMD_DUP_A
extern const char command_name_dup_a[];
int cmd_dup_setup_a(int);

#define ENUM__CMD_DUP_A        COMMAND_INDEX__DUP_A
#define NAME__CMD_DUP_A        "dup-a"
#define NAMEVAR__CMD_DUP_A     command_name_dup_a
#define SETUP__CMD_DUP_A       cmd_dup_setup_a

// Extrapolated defines
#define ENUM_LIST__CMD_DUP_A   ENUM__CMD_DUP_A,
#define NAME_TC__CMD_DUP_A     NAME__CMD_DUP_A,
#define NAME_VC__CMD_DUP_A     NAMEVAR__CMD_DUP_A,
#define NAME_VS__CMD_DUP_A     names[COMMAND_INDEX__DUP_A] = NAMEVAR__CMD_DUP_A;
#define SETUP_C__CMD_DUP_A     &SETUP__CMD_DUP_A,
#define SETUP_S__CMD_DUP_A     setups[COMMAND_INDEX__DUP_A] = &SETUP__CMD_DUP_A;
#define RUN_C__CMD_DUP_A       &command_common_run_ok,
#define RUN_S__CMD_DUP_A       runs[COMMAND_INDEX__DUP_A] = &command_common_run_ok;
#else /* USE_CMD_DUP_A */
#define ENUM_LIST__CMD_DUP_A
#define NAME_TC__CMD_DUP_A
#define NAME_VC__CMD_DUP_A
#define NAME_VS__CMD_DUP_A
#define SETUP_C__CMD_DUP_A
#define SETUP_S__CMD_DUP_A
#define RUN_C__CMD_DUP_A
#define RUN_S__CMD_DUP_A
#endif /* USE_CMD_DUP_A */


#ifdef USE_CMD_DUP_W
extern const char command_name_dup_w[];
int cmd_dup_setup_w(int);

#define ENUM__CMD_DUP_W        COMMAND_INDEX__DUP_W
#define NAME__CMD_DUP_W        "dup-w"
#define NAMEVAR__CMD_DUP_W     command_name_dup_w
#define SETUP__CMD_DUP_W       cmd_dup_setup_w

// Extrapolated defines
#define ENUM_LIST__CMD_DUP_W   ENUM__CMD_DUP_W,
#define NAME_TC__CMD_DUP_W     NAME__CMD_DUP_W,
#define NAME_VC__CMD_DUP_W     NAMEVAR__CMD_DUP_W,
#define NAME_VS__CMD_DUP_W     names[COMMAND_INDEX__DUP_W] = NAMEVAR__CMD_DUP_W;
#define SETUP_C__CMD_DUP_W     &SETUP__CMD_DUP_W,
#define SETUP_S__CMD_DUP_W     setups[COMMAND_INDEX__DUP_W] = &SETUP__CMD_DUP_W;
#define RUN_C__CMD_DUP_W       &command_common_run_ok,
#define RUN_S__CMD_DUP_W       runs[COMMAND_INDEX__DUP_W] = &command_common_run_ok;
#else /* USE_CMD_DUP_W */
#define ENUM_LIST__CMD_DUP_W
#define NAME_TC__CMD_DUP_W
#define NAME_VC__CMD_DUP_W
#define NAME_VS__CMD_DUP_W
#define SETUP_C__CMD_DUP_W
#define SETUP_S__CMD_DUP_W
#define RUN_C__CMD_DUP_W
#define RUN_S__CMD_DUP_W
#endif /* USE_CMD_DUP_W */


#ifdef USE_CMD_DUP_R
extern const char command_name_dup_r[];
int cmd_dup_setup_r(int);

#define ENUM__CMD_DUP_R        COMMAND_INDEX__DUP_R
#define NAME__CMD_DUP_R        "dup-r"
#define NAMEVAR__CMD_DUP_R     command_name_dup_r
#define SETUP__CMD_DUP_R       cmd_dup_setup_r

// Extrapolated defines
#define ENUM_LIST__CMD_DUP_R   ENUM__CMD_DUP_R,
#define NAME_TC__CMD_DUP_R     NAME__CMD_DUP_R,
#define NAME_VC__CMD_DUP_R     NAMEVAR__CMD_DUP_R,
#define NAME_VS__CMD_DUP_R     names[COMMAND_INDEX__DUP_R] = NAMEVAR__CMD_DUP_R;
#define SETUP_C__CMD_DUP_R     &SETUP__CMD_DUP_R,
#define SETUP_S__CMD_DUP_R     setups[COMMAND_INDEX__DUP_R] = &SETUP__CMD_DUP_R;
#define RUN_C__CMD_DUP_R       &command_common_run_ok,
#define RUN_S__CMD_DUP_R       runs[COMMAND_INDEX__DUP_R] = &command_common_run_ok;
#else /* USE_CMD_DUP_R */
#define ENUM_LIST__CMD_DUP_R
#define NAME_TC__CMD_DUP_R
#define NAME_VC__CMD_DUP_R
#define NAME_VS__CMD_DUP_R
#define SETUP_C__CMD_DUP_R
#define SETUP_S__CMD_DUP_R
#define RUN_C__CMD_DUP_R
#define RUN_S__CMD_DUP_R
#endif /* USE_CMD_DUP_R */


int cmd_dup_run_tgt();


// Extrapolated defines
#define ENUM_LIST__CMD_DUP   ENUM_LIST__CMD_DUP_A \
                             ENUM_LIST__CMD_DUP_W \
                             ENUM_LIST__CMD_DUP_R \
                             COMMAND_INDEX__DUP_FD, \
                             COMMAND_INDEX__DUP_TGT,
#define NAME_TC__CMD_DUP     NAME_TC__CMD_DUP_A \
                             NAME_TC__CMD_DUP_W \
                             NAME_TC__CMD_DUP_R \
                             EMPTY_STRING, EMPTY_STRING,
#define NAME_VC__CMD_DUP     NAMEVAR__CMD_DUP_A \
                             NAMEVAR__CMD_DUP_W \
                             NAMEVAR__CMD_DUP_R \
                             command_common_empty_name, command_common_empty_name,
#define NAME_VS__CMD_DUP     NAME_VS__CMD_DUP_A \
                             NAME_VS__CMD_DUP_W \
                             NAME_VS__CMD_DUP_R \
                             names[COMMAND_INDEX__DUP_FD] = command_common_empty_name; \
                             names[COMMAND_INDEX__DUP_TGT] = command_common_empty_name;
#define SETUP_C__CMD_DUP     SETUP_C__CMD_DUP_A \
                             SETUP_C__CMD_DUP_W \
                             SETUP_C__CMD_DUP_R \
                             &command_common_setup_identity, &command_common_setup_identity,
#define SETUP_S__CMD_DUP     SETUP_S__CMD_DUP_A \
                             SETUP_S__CMD_DUP_W \
                             SETUP_S__CMD_DUP_R \
                             setups[COMMAND_INDEX__DUP_FD] = &command_common_setup_identity; \
                             setups[COMMAND_INDEX__DUP_TGT] = &command_common_setup_identity;
#define RUN_C__CMD_DUP       RUN_C__CMD_DUP_A \
                             RUN_C__CMD_DUP_W \
                             RUN_C__CMD_DUP_R \
                             &command_common_run_toint10, &cmd_dup_run_tgt,
#define RUN_S__CMD_DUP       RUN_S__CMD_DUP_A \
                             RUN_S__CMD_DUP_W \
                             RUN_S__CMD_DUP_R \
                             runs[COMMAND_INDEX__DUP_FD] = &command_common_run_toint10; \
                             runs[COMMAND_INDEX__DUP_TGT] = &cmd_dup_run_tgt;
#define CALL_INIT__CMD_DUP


#else /* defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R) */
#define ENUM_LIST__CMD_DUP
#define NAME_TC__CMD_DUP
#define NAME_VC__CMD_DUP
#define NAME_VS__CMD_DUP
#define SETUP_C__CMD_DUP
#define SETUP_S__CMD_DUP
#define RUN_C__CMD_DUP
#define RUN_S__CMD_DUP
#define CALL_INIT__CMD_DUP
#endif /* defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_W) || defined(USE_CMD_DUP_R) */

#endif /* _FS_SHELL__CMD_DUP_ */
