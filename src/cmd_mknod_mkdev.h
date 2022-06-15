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

#ifndef _FS_SHELL__CMD_MKNOD_MKDEV_
#define _FS_SHELL__CMD_MKNOD_MKDEV_

#include "uses.h"

#if defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV)

#include "command_def.h"
#include "command_common.h"


int cmd_mknod_dev_type();
int cmd_mknod_dev_run();


#ifdef USE_CMD_MKNOD
// mknod sets up the device stuff in the setup,
// then on the first argument uses cmd_mknod_dev_type,
// then that moves the cmd over to the mknod_dev runner.

extern const char command_name_mknod[];
int cmd_mknod_setup(int);



// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define NAME__CMD_MKNOD        "mknod"


// Extrapolated defines
#define ENUM_LIST__CMD_MKNOD   COMMAND_INDEX__MKNOD,
#define NAME_TC__CMD_MKNOD     NAME__CMD_MKNOD,
#define NAME_VC__CMD_MKNOD     command_name_mknod,
#define NAME_VS__CMD_MKNOD     names[COMMAND_INDEX__MKNOD] = command_name_mknod;
#define SETUP_C__CMD_MKNOD     &cmd_mknod_setup,
#define SETUP_S__CMD_MKNOD     setups[COMMAND_INDEX__MKNOD] = &cmd_mknod_setup;
#define RUN_C__CMD_MKNOD       &cmd_mknod_dev_type,
#define RUN_S__CMD_MKNOD       runs[COMMAND_INDEX__MKNOD] = &cmd_mknod_dev_type;


#else /* USE_CMD_MKNOD */
#define ENUM_LIST__CMD_MKNOD
#define NAME_TC__CMD_MKNOD
#define NAME_VC__CMD_MKNOD
#define NAME_VS__CMD_MKNOD
#define SETUP_C__CMD_MKNOD
#define SETUP_S__CMD_MKNOD
#define RUN_C__CMD_MKNOD
#define RUN_S__CMD_MKNOD
#define CALL_INIT__CMD_MKNOD
#endif /* USE_CMD_MKNOD */



#ifdef USE_CMD_MKDEV
// mkdev reads major then minor device numbers,
// on the minor the device is created, then it
// moves over to to read the dev type, then finally to mknod_dev runner.

extern const char command_name_mkdev[];
int cmd_mkdev_run();



// Note trailing commas and weird syntax here.
// These are used by command_list for different setups.
#define NAME__CMD_MKDEV        "mkdev"


// Extrapolated defines
#define ENUM_LIST__CMD_MKDEV   COMMAND_INDEX__MKDEV, \
                               COMMAND_INDEX__MKDEV_MINOR, \
                               COMMAND_INDEX__MKDEV_DEV_TYPE,
#define NAME_TC__CMD_MKDEV     NAME__CMD_MKDEV, \
                               EMPTY_STRING, \
                               EMPTY_STRING,
#define NAME_VC__CMD_MKDEV     command_name_mkdev, \
                               command_common_empty_name, \
                               command_common_empty_name,
#define NAME_VS__CMD_MKDEV     names[COMMAND_INDEX__MKDEV] = command_name_mkdev; \
                               names[COMMAND_INDEX__MKDEV_MINOR] = command_common_empty_name; \
                               names[COMMAND_INDEX__MKDEV_DEV_TYPE] = command_common_empty_name;
#define SETUP_C__CMD_MKDEV     &command_common_setup_identity, \
                               &command_common_setup_identity, \
                               &command_common_setup_identity,
#define SETUP_S__CMD_MKDEV     setups[COMMAND_INDEX__MKDEV] = &command_common_setup_identity; \
                               setups[COMMAND_INDEX__MKDEV_MINOR] = &command_common_setup_identity; \
                               setups[COMMAND_INDEX__MKDEV_DEV_TYPE] = &command_common_setup_identity;
#define RUN_C__CMD_MKDEV       &command_common_run_toint10, \
                               &cmd_mkdev_run, \
                               &cmd_mknod_dev_type,
#define RUN_S__CMD_MKDEV       runs[COMMAND_INDEX__MKDEV] = &command_common_run_toint10; \
                               runs[COMMAND_INDEX__MKDEV_MINOR] = &cmd_mkdev_run; \
                               runs[COMMAND_INDEX__MKDEV_DEV_TYPE] = &cmd_mknod_dev_type;


#else /* USE_CMD_MKDEV */
#define ENUM_LIST__CMD_MKDEV
#define NAME_TC__CMD_MKDEV
#define NAME_VC__CMD_MKDEV
#define NAME_VS__CMD_MKDEV
#define SETUP_C__CMD_MKDEV
#define SETUP_S__CMD_MKDEV
#define RUN_C__CMD_MKDEV
#define RUN_S__CMD_MKDEV
#endif /* USE_CMD_MKDEV */




// Extrapolated defines
#define ENUM_LIST__CMD_MKNOD_MKDEV   ENUM_LIST__CMD_MKNOD \
                                     ENUM_LIST__CMD_MKDEV \
                                     COMMAND_INDEX__MKNOD_DEV__RUN,
#define NAME_TC__CMD_MKNOD_MKDEV     NAME_TC__CMD_MKNOD \
                                     NAME_TC__CMD_MKDEV \
                                     EMPTY_STRING,
#define NAME_VC__CMD_MKNOD_MKDEV     NAME_VC__CMD_MKNOD \
                                     NAME_VC__CMD_MKDEV \
                                     command_common_empty_name,
#define NAME_VS__CMD_MKNOD_MKDEV     NAME_VS__CMD_MKNOD \
                                     NAME_VS__CMD_MKDEV \
                                     names[COMMAND_INDEX__MKNOD_DEV__RUN] = command_common_empty_name;
#define SETUP_C__CMD_MKNOD_MKDEV     SETUP_C__CMD_MKNOD \
                                     SETUP_C__CMD_MKDEV \
                                     &command_common_setup_identity,
#define SETUP_S__CMD_MKNOD_MKDEV     SETUP_S__CMD_MKNOD \
                                     SETUP_S__CMD_MKDEV \
                                     setups[COMMAND_INDEX__MKNOD_DEV__RUN] = &command_common_setup_identity;
#define RUN_C__CMD_MKNOD_MKDEV       RUN_C__CMD_MKNOD \
                                     RUN_C__CMD_MKDEV \
                                     &cmd_mknod_dev_run,
#define RUN_S__CMD_MKNOD_MKDEV       RUN_S__CMD_MKNOD \
                                     RUN_S__CMD_MKDEV \
                                     runs[COMMAND_INDEX__MKNOD_DEV__RUN] = &cmd_mknod_dev_run;
#define CALL_INIT__CMD_MKNOD_MKDEV




#else /* defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV) */
#define ENUM_LIST__CMD_MKNOD_MKDEV
#define NAME_TC__CMD_MKNOD_MKDEV
#define NAME_VC__CMD_MKNOD_MKDEV
#define NAME_VS__CMD_MKNOD_MKDEV
#define SETUP_C__CMD_MKNOD_MKDEV
#define SETUP_S__CMD_MKNOD_MKDEV
#define RUN_C__CMD_MKNOD_MKDEV
#define RUN_S__CMD_MKNOD_MKDEV
#define CALL_INIT__CMD_MKNOD_MKDEV
#endif /* defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV) */
#endif /* _FS_SHELL__CMD_MKNOD_MKDEV_ */
