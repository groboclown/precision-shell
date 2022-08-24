// GENERATED FILE.  DO NOT MODIFY.

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

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, ERMPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__COMMAND_LIST_
#define _FS_SHELL__COMMAND_LIST_

#include "cmd_find_cmd.h"
#include "cmd_cat.h"
#include "cmd_cd.h"
#include "cmd_chmod.h"
#include "cmd_chown.h"
#include "cmd_dup.h"
#include "cmd_echo.h"
#include "cmd_exec.h"
#include "cmd_exit.h"
#include "cmd_export.h"
#include "cmd_file_stat.h"
#include "cmd_fmode.h"
#include "cmd_for_each.h"
#include "cmd_if_else.h"
#include "cmd_ln_h.h"
#include "cmd_ln_s.h"
#include "cmd_ls_ls.h"
#include "cmd_kill_pid.h"
#include "cmd_mkdir.h"
#include "cmd_mknod_mkdev.h"
#include "cmd_mv.h"
#include "cmd_noop.h"
#include "cmd_pwd.h"
#include "cmd_rm.h"
#include "cmd_rmdir.h"
#include "cmd_shared_sub_args.h"
#include "cmd_shared_virtual.h"
#include "cmd_signal.h"
#include "cmd_sleep.h"
#include "cmd_spawn.h"
#include "cmd_su_exec.h"
#include "cmd_su_spawn.h"
#include "cmd_subcmd.h"
#include "cmd_test_connection.h"
#include "cmd_timer.h"
#include "cmd_touch_trunc.h"
#include "cmd_wait_pid.h"
#include "cmd_while.h"
#include "cmd_write_fd.h"
#include "cmd_version.h"
#include "cmd_err.h"


#define CMD_GLOBAL_VARDEF \
            GLOBAL_VARDEF__FIND_CMD \
            GLOBAL_VARDEF__CAT_ENV_CAT \
            GLOBAL_VARDEF__CD \
            GLOBAL_VARDEF__CHMOD \
            GLOBAL_VARDEF__CHOWN \
            GLOBAL_VARDEF__DUP \
            GLOBAL_VARDEF__ECHO \
            GLOBAL_VARDEF__EXEC \
            GLOBAL_VARDEF__EXIT \
            GLOBAL_VARDEF__EXPORT \
            GLOBAL_VARDEF__FILE_STAT \
            GLOBAL_VARDEF__FMODE \
            GLOBAL_VARDEF__FOR_EACH \
            GLOBAL_VARDEF__IF_ELSE \
            GLOBAL_VARDEF__LN_H \
            GLOBAL_VARDEF__LN_S \
            GLOBAL_VARDEF__LS_LS_F \
            GLOBAL_VARDEF__KILL_PID \
            GLOBAL_VARDEF__MKDIR \
            GLOBAL_VARDEF__MKNOD_MKDEV \
            GLOBAL_VARDEF__MV \
            GLOBAL_VARDEF__NOOP \
            GLOBAL_VARDEF__PWD \
            GLOBAL_VARDEF__RM \
            GLOBAL_VARDEF__RMDIR \
            GLOBAL_VARDEF__SHARED_SUB_ARGS \
            GLOBAL_VARDEF__SHARED_VIRTUAL \
            GLOBAL_VARDEF__SIGNAL \
            GLOBAL_VARDEF__SLEEP \
            GLOBAL_VARDEF__SPAWN \
            GLOBAL_VARDEF__SU_EXEC \
            GLOBAL_VARDEF__SU_SPAWN \
            GLOBAL_VARDEF__SUBCMD \
            GLOBAL_VARDEF__TEST_CONNECTION \
            GLOBAL_VARDEF__TIMER_FUNCS \
            GLOBAL_VARDEF__TOUCH_TRUNC \
            GLOBAL_VARDEF__WAIT_PID \
            GLOBAL_VARDEF__WHILE \
            GLOBAL_VARDEF__WRITE_FD \
            GLOBAL_VARDEF__VERSION \
            GLOBAL_VARDEF__ERR
#define CMD_INITIALIZE \
            INITIALIZE__FIND_CMD \
            INITIALIZE__CAT_ENV_CAT \
            INITIALIZE__CD \
            INITIALIZE__CHMOD \
            INITIALIZE__CHOWN \
            INITIALIZE__DUP \
            INITIALIZE__ECHO \
            INITIALIZE__EXEC \
            INITIALIZE__EXIT \
            INITIALIZE__EXPORT \
            INITIALIZE__FILE_STAT \
            INITIALIZE__FMODE \
            INITIALIZE__FOR_EACH \
            INITIALIZE__IF_ELSE \
            INITIALIZE__LN_H \
            INITIALIZE__LN_S \
            INITIALIZE__LS_LS_F \
            INITIALIZE__KILL_PID \
            INITIALIZE__MKDIR \
            INITIALIZE__MKNOD_MKDEV \
            INITIALIZE__MV \
            INITIALIZE__NOOP \
            INITIALIZE__PWD \
            INITIALIZE__RM \
            INITIALIZE__RMDIR \
            INITIALIZE__SHARED_SUB_ARGS \
            INITIALIZE__SHARED_VIRTUAL \
            INITIALIZE__SIGNAL \
            INITIALIZE__SLEEP \
            INITIALIZE__SPAWN \
            INITIALIZE__SU_EXEC \
            INITIALIZE__SU_SPAWN \
            INITIALIZE__SUBCMD \
            INITIALIZE__TEST_CONNECTION \
            INITIALIZE__TIMER_FUNCS \
            INITIALIZE__TOUCH_TRUNC \
            INITIALIZE__WAIT_PID \
            INITIALIZE__WHILE \
            INITIALIZE__WRITE_FD \
            INITIALIZE__VERSION \
            INITIALIZE__ERR
#define CMD_STARTUP_CASE \
            STARTUP_CASE__FIND_CMD \
            STARTUP_CASE__CAT_ENV_CAT \
            STARTUP_CASE__CD \
            STARTUP_CASE__CHMOD \
            STARTUP_CASE__CHOWN \
            STARTUP_CASE__DUP \
            STARTUP_CASE__ECHO \
            STARTUP_CASE__EXEC \
            STARTUP_CASE__EXIT \
            STARTUP_CASE__EXPORT \
            STARTUP_CASE__FILE_STAT \
            STARTUP_CASE__FMODE \
            STARTUP_CASE__FOR_EACH \
            STARTUP_CASE__IF_ELSE \
            STARTUP_CASE__LN_H \
            STARTUP_CASE__LN_S \
            STARTUP_CASE__LS_LS_F \
            STARTUP_CASE__KILL_PID \
            STARTUP_CASE__MKDIR \
            STARTUP_CASE__MKNOD_MKDEV \
            STARTUP_CASE__MV \
            STARTUP_CASE__NOOP \
            STARTUP_CASE__PWD \
            STARTUP_CASE__RM \
            STARTUP_CASE__RMDIR \
            STARTUP_CASE__SHARED_SUB_ARGS \
            STARTUP_CASE__SHARED_VIRTUAL \
            STARTUP_CASE__SIGNAL \
            STARTUP_CASE__SLEEP \
            STARTUP_CASE__SPAWN \
            STARTUP_CASE__SU_EXEC \
            STARTUP_CASE__SU_SPAWN \
            STARTUP_CASE__SUBCMD \
            STARTUP_CASE__TEST_CONNECTION \
            STARTUP_CASE__TIMER_FUNCS \
            STARTUP_CASE__TOUCH_TRUNC \
            STARTUP_CASE__WAIT_PID \
            STARTUP_CASE__WHILE \
            STARTUP_CASE__WRITE_FD \
            STARTUP_CASE__VERSION \
            STARTUP_CASE__ERR
#define CMD_RUN_CASE \
            RUN_CASE__FIND_CMD \
            RUN_CASE__CAT_ENV_CAT \
            RUN_CASE__CD \
            RUN_CASE__CHMOD \
            RUN_CASE__CHOWN \
            RUN_CASE__DUP \
            RUN_CASE__ECHO \
            RUN_CASE__EXEC \
            RUN_CASE__EXIT \
            RUN_CASE__EXPORT \
            RUN_CASE__FILE_STAT \
            RUN_CASE__FMODE \
            RUN_CASE__FOR_EACH \
            RUN_CASE__IF_ELSE \
            RUN_CASE__LN_H \
            RUN_CASE__LN_S \
            RUN_CASE__LS_LS_F \
            RUN_CASE__KILL_PID \
            RUN_CASE__MKDIR \
            RUN_CASE__MKNOD_MKDEV \
            RUN_CASE__MV \
            RUN_CASE__NOOP \
            RUN_CASE__PWD \
            RUN_CASE__RM \
            RUN_CASE__RMDIR \
            RUN_CASE__SHARED_SUB_ARGS \
            RUN_CASE__SHARED_VIRTUAL \
            RUN_CASE__SIGNAL \
            RUN_CASE__SLEEP \
            RUN_CASE__SPAWN \
            RUN_CASE__SU_EXEC \
            RUN_CASE__SU_SPAWN \
            RUN_CASE__SUBCMD \
            RUN_CASE__TEST_CONNECTION \
            RUN_CASE__TIMER_FUNCS \
            RUN_CASE__TOUCH_TRUNC \
            RUN_CASE__WAIT_PID \
            RUN_CASE__WHILE \
            RUN_CASE__WRITE_FD \
            RUN_CASE__VERSION \
            RUN_CASE__ERR
#define CMD_REQUIRES_ADDL_ARG \
            REQUIRES_ADDL_ARG__FIND_CMD \
            REQUIRES_ADDL_ARG__CAT_ENV_CAT \
            REQUIRES_ADDL_ARG__CD \
            REQUIRES_ADDL_ARG__CHMOD \
            REQUIRES_ADDL_ARG__CHOWN \
            REQUIRES_ADDL_ARG__DUP \
            REQUIRES_ADDL_ARG__ECHO \
            REQUIRES_ADDL_ARG__EXEC \
            REQUIRES_ADDL_ARG__EXIT \
            REQUIRES_ADDL_ARG__EXPORT \
            REQUIRES_ADDL_ARG__FILE_STAT \
            REQUIRES_ADDL_ARG__FMODE \
            REQUIRES_ADDL_ARG__FOR_EACH \
            REQUIRES_ADDL_ARG__IF_ELSE \
            REQUIRES_ADDL_ARG__LN_H \
            REQUIRES_ADDL_ARG__LN_S \
            REQUIRES_ADDL_ARG__LS_LS_F \
            REQUIRES_ADDL_ARG__KILL_PID \
            REQUIRES_ADDL_ARG__MKDIR \
            REQUIRES_ADDL_ARG__MKNOD_MKDEV \
            REQUIRES_ADDL_ARG__MV \
            REQUIRES_ADDL_ARG__NOOP \
            REQUIRES_ADDL_ARG__PWD \
            REQUIRES_ADDL_ARG__RM \
            REQUIRES_ADDL_ARG__RMDIR \
            REQUIRES_ADDL_ARG__SHARED_SUB_ARGS \
            REQUIRES_ADDL_ARG__SHARED_VIRTUAL \
            REQUIRES_ADDL_ARG__SIGNAL \
            REQUIRES_ADDL_ARG__SLEEP \
            REQUIRES_ADDL_ARG__SPAWN \
            REQUIRES_ADDL_ARG__SU_EXEC \
            REQUIRES_ADDL_ARG__SU_SPAWN \
            REQUIRES_ADDL_ARG__SUBCMD \
            REQUIRES_ADDL_ARG__TEST_CONNECTION \
            REQUIRES_ADDL_ARG__TIMER_FUNCS \
            REQUIRES_ADDL_ARG__TOUCH_TRUNC \
            REQUIRES_ADDL_ARG__WAIT_PID \
            REQUIRES_ADDL_ARG__WHILE \
            REQUIRES_ADDL_ARG__WRITE_FD \
            REQUIRES_ADDL_ARG__VERSION \
            REQUIRES_ADDL_ARG__ERR

// All the commands, by index.
enum CommandIndex {


            ENUM_LIST__FIND_CMD
            ENUM_LIST__CAT_ENV_CAT
            ENUM_LIST__CD
            ENUM_LIST__CHMOD
            ENUM_LIST__CHOWN
            ENUM_LIST__DUP
            ENUM_LIST__ECHO
            ENUM_LIST__EXEC
            ENUM_LIST__EXIT
            ENUM_LIST__EXPORT
            ENUM_LIST__FILE_STAT
            ENUM_LIST__FMODE
            ENUM_LIST__FOR_EACH
            ENUM_LIST__IF_ELSE
            ENUM_LIST__LN_H
            ENUM_LIST__LN_S
            ENUM_LIST__LS_LS_F
            ENUM_LIST__KILL_PID
            ENUM_LIST__MKDIR
            ENUM_LIST__MKNOD_MKDEV
            ENUM_LIST__MV
            ENUM_LIST__NOOP
            ENUM_LIST__PWD
            ENUM_LIST__RM
            ENUM_LIST__RMDIR
            ENUM_LIST__SHARED_SUB_ARGS
            ENUM_LIST__SHARED_VIRTUAL
            ENUM_LIST__SIGNAL
            ENUM_LIST__SLEEP
            ENUM_LIST__SPAWN
            ENUM_LIST__SU_EXEC
            ENUM_LIST__SU_SPAWN
            ENUM_LIST__SUBCMD
            ENUM_LIST__TEST_CONNECTION
            ENUM_LIST__TIMER_FUNCS
            ENUM_LIST__TOUCH_TRUNC
            ENUM_LIST__WAIT_PID
            ENUM_LIST__WHILE
            ENUM_LIST__WRITE_FD
            ENUM_LIST__VERSION
            ENUM_LIST__ERR

    COMMAND_INDEX__LAST_NAMED_CMD,


            VIRTUAL_ENUM_LIST__FIND_CMD
            VIRTUAL_ENUM_LIST__CAT_ENV_CAT
            VIRTUAL_ENUM_LIST__CD
            VIRTUAL_ENUM_LIST__CHMOD
            VIRTUAL_ENUM_LIST__CHOWN
            VIRTUAL_ENUM_LIST__DUP
            VIRTUAL_ENUM_LIST__ECHO
            VIRTUAL_ENUM_LIST__EXEC
            VIRTUAL_ENUM_LIST__EXIT
            VIRTUAL_ENUM_LIST__EXPORT
            VIRTUAL_ENUM_LIST__FILE_STAT
            VIRTUAL_ENUM_LIST__FMODE
            VIRTUAL_ENUM_LIST__FOR_EACH
            VIRTUAL_ENUM_LIST__IF_ELSE
            VIRTUAL_ENUM_LIST__LN_H
            VIRTUAL_ENUM_LIST__LN_S
            VIRTUAL_ENUM_LIST__LS_LS_F
            VIRTUAL_ENUM_LIST__KILL_PID
            VIRTUAL_ENUM_LIST__MKDIR
            VIRTUAL_ENUM_LIST__MKNOD_MKDEV
            VIRTUAL_ENUM_LIST__MV
            VIRTUAL_ENUM_LIST__NOOP
            VIRTUAL_ENUM_LIST__PWD
            VIRTUAL_ENUM_LIST__RM
            VIRTUAL_ENUM_LIST__RMDIR
            VIRTUAL_ENUM_LIST__SHARED_SUB_ARGS
            VIRTUAL_ENUM_LIST__SHARED_VIRTUAL
            VIRTUAL_ENUM_LIST__SIGNAL
            VIRTUAL_ENUM_LIST__SLEEP
            VIRTUAL_ENUM_LIST__SPAWN
            VIRTUAL_ENUM_LIST__SU_EXEC
            VIRTUAL_ENUM_LIST__SU_SPAWN
            VIRTUAL_ENUM_LIST__SUBCMD
            VIRTUAL_ENUM_LIST__TEST_CONNECTION
            VIRTUAL_ENUM_LIST__TIMER_FUNCS
            VIRTUAL_ENUM_LIST__TOUCH_TRUNC
            VIRTUAL_ENUM_LIST__WAIT_PID
            VIRTUAL_ENUM_LIST__WHILE
            VIRTUAL_ENUM_LIST__WRITE_FD
            VIRTUAL_ENUM_LIST__VERSION
            VIRTUAL_ENUM_LIST__ERR

    COMMAND_INDEX__LAST_VIRTUAL_CMD
};

#endif /* _FS_SHELL__COMMAND_LIST_ */
