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

#include "uses.h"
#include "command_list.h"
#include "globals.h"
#include "helpers.h"
#include "output.h"

// Include each command.  It already has ifdefs around it internally.
#include "cmd_chmod.h"
#include "cmd_dup.h"
#include "cmd_echo.h"
#include "cmd_fmode.h"
#include "cmd_version.h"

// ==========================================================================
// Common helper functions.

// cmdl_identity_setup a common setup that uses the invoked command index.
int cmdl_identity_setup(int idx) {
    return idx;
}

// cmdl_toint10_arg__runner Useful command magic.
//   - Shuffles global_arg1_i to global_arg2_i
//   - parses global_arg as a base 10 integer into global_arg1_i, with error checking.
//   - increments global_cmd
int cmdl_toint10_arg__runner() {
    int val = helper_arg_to_uint(10, 0xffff);
    if (val < 0) {
        return 1;
    }
    global_arg2_i = global_arg1_i;
    global_arg1_i = val;
    global_cmd++;
    return 0;
}

// cmdl_store_arg__runner Use command magic.
//   - sets global_arg_cached to the current argument
//   - increments global_cmd
int cmdl_store_arg__runner() {
    global_arg_cached = global_arg;
    global_cmd++;
    return 0;
}

// cmdl_ok a command runner that just returns no-error.
int cmdl_ok() {
    return 0;
}

// cmdl_FIXME a command runner that indicates it needs to be replaced.
int cmdl_FIXME() {
    stdoutP("TODO implement ");
    stdoutPLn(global_cmd_name);
    return 0;
}


// ==========================================================================
// ==========================================================================
// Command Lists
//   IT IS EXTREMELY IMPORTANT THAT THE ORDER HERE MATCHES THE ENUM ORDER EXACTLY


// ==========================================================================
// Names of each command.
const char *command_list_names[] = {
    // COMMAND_INDEX__NOOP
    "noop",

    // COMMAND_INDEX__VERSION,
    "version",

#ifdef USES_FMODE
    // COMMAND_INDEX__FMODE,
    "fmode",
#endif

#ifdef USE_CMD_ECHO
    // COMMAND_INDEX__ECHO,
    "echo",
#endif

#ifdef USE_CMD_RM
    // COMMAND_INDEX__RM,
    "rm",
#endif

#ifdef USE_CMD_RMDIR
    // COMMAND_INDEX__RMDIR,
    "rmdir",
#endif

#ifdef USE_CMD_TOUCH
    // COMMAND_INDEX__TOUCH,
    "touch",
#endif

#ifdef USE_CMD_TRUNC
    // COMMAND_INDEX__TRUNC,
    "trunc",
#endif

#ifdef USE_CMD_DUP_R
    // COMMAND_INDEX__DUP_R,
    "dup-r",
#endif

#ifdef USE_CMD_DUP_W
    // COMMAND_INDEX__DUP_W,
    "dup-w",
#endif

#ifdef USE_CMD_DUP_A
    // COMMAND_INDEX__DUP_A,
    "dup-a",
#endif

#ifdef USES_DUP
    // COMMAND_INDEX__DUP__FD,
    "",  // phoney
    // COMMAND_INDEX__DUP__TGT,
    "",  // phoney
#endif

#ifdef USE_CMD_MKNOD
    // COMMAND_INDEX__MKNOD,
    "mknod",
    // COMMAND_INDEX__MKNOD__RUN,
    "",  // phoney
#endif

#ifdef USE_CMD_MKDEV
    // COMMAND_INDEX__MKDEV,
    "mkdev",
    // COMMAND_INDEX__MKDEV__MAJOR,
    "",  // phoney
    // COMMAND_INDEX__MKDEV__MINOR,
    "",  // phoney
    // COMMAND_INDEX__MKDEV__RUN,
    "",  // phoney
#endif

#ifdef USE_CMD_MKDIR
    // COMMAND_INDEX__MKDIR,
    "mkdir",
#endif

#ifdef USE_CMD_CHOWN
    // COMMAND_INDEX__CHOWN,
    "chown",
    // COMMAND_INDEX__CHOWN__GROUP,
    "",  // phoney
    // COMMAND_INDEX__CHOWN__RUN,
    "",  // phoney
#endif

#ifdef USE_CMD_CHMOD
    // COMMAND_INDEX__CHMOD,
    "chmod",
    // COMMAND_INDEX__CHMOD__RUN,
    "",  // phoney
#endif

#ifdef USE_CMD_LN_S
    // COMMAND_INDEX__LN_S,
    "ln-s",
    // COMMAND_INDEX__LN_S__RUN,
    "",  // phoney
#endif

#ifdef USE_CMD_LN_H
    // COMMAND_INDEX__LN_H,
    "ln-h",
    // COMMAND_INDEX__LN_H__RUN,
    "",  // phoney
#endif

#ifdef USE_CMD_MV
    // COMMAND_INDEX__MV,
    "mv",
    // COMMAND_INDEX__MV__RUN,
    "",  // phoney
#endif

#ifdef USE_CMD_SLEEP
    // COMMAND_INDEX__SLEEP,
    "sleep",
#endif

#ifdef USE_CMD_SIGNAL
    // COMMAND_INDEX__SIGNAL,
    "signal",
#endif

#ifdef USE_CMD_EXEC
    // COMMAND_INDEX__EXEC,
    "exec",
#endif

    // COMMAND_INDEX__ERR
    //   End List marker
};

CommandSetup command_setup[] = {
    // COMMAND_INDEX__NOOP
    &cmdl_identity_setup,

    // COMMAND_INDEX__VERSION,
    &cmd_version_setup,

#ifdef USES_FMODE
    // COMMAND_INDEX__FMODE,
    &cmdl_identity_setup,
#endif

#ifdef USE_CMD_ECHO
    // COMMAND_INDEX__ECHO,
    &cmdl_identity_setup,
#endif
#ifdef USE_CMD_RM
    // COMMAND_INDEX__RM,
    &cmdl_identity_setup,
#endif
#ifdef USE_CMD_RMDIR
    // COMMAND_INDEX__RMDIR,
    &cmdl_identity_setup,
#endif
#ifdef USE_CMD_TOUCH
    // COMMAND_INDEX__TOUCH,
    &cmdl_identity_setup,
#endif
#ifdef USE_CMD_TRUNC
    // COMMAND_INDEX__TRUNC,
    &cmdl_identity_setup,
#endif
#ifdef USE_CMD_DUP_R
    // COMMAND_INDEX__DUP_R,
    &cmd_dup_r_setup,
#endif
#ifdef USE_CMD_DUP_W
    // COMMAND_INDEX__DUP_W,
    &cmd_dup_w_setup,
#endif
#ifdef USE_CMD_DUP_A
    // COMMAND_INDEX__DUP_A,
    &cmd_dup_a_setup,
#endif

#ifdef USES_DUP
    // COMMAND_INDEX__DUP__FD,
    NULL,  // phoney
    // COMMAND_INDEX__DUP__TGT,
    NULL,  // phoney
#endif

#ifdef USE_CMD_MKNOD
    // COMMAND_INDEX__MKNOD,
    &cmdl_identity_setup,
    // COMMAND_INDEX__MKNOD__RUN,
    NULL,  // phoney
#endif
#ifdef USE_CMD_MKDEV
    // COMMAND_INDEX__MKDEV,
    &cmdl_identity_setup,
    // COMMAND_INDEX__MKDEV__MAJOR,
    NULL,  // phoney
    // COMMAND_INDEX__MKDEV__MINOR,
    NULL,  // phoney
    // COMMAND_INDEX__MKDEV__RUN,
    NULL,  // phoney
#endif
#ifdef USE_CMD_MKDIR
    // COMMAND_INDEX__MKDIR,
    &cmdl_identity_setup,
#endif
#ifdef USE_CMD_CHOWN
    // COMMAND_INDEX__CHOWN,
    &cmdl_identity_setup,
    // COMMAND_INDEX__CHOWN__GROUP,
    NULL,  // phoney
    // COMMAND_INDEX__CHOWN__RUN,
    NULL,  // phoney
#endif
#ifdef USE_CMD_CHMOD
    // COMMAND_INDEX__CHMOD,
    &cmdl_identity_setup,
    // COMMAND_INDEX__CHMOD__RUN,
    NULL,  // phoney
#endif
#ifdef USE_CMD_LN_S
    // COMMAND_INDEX__LN_S,
    &cmdl_identity_setup,
    // COMMAND_INDEX__LN_S__RUN,
    NULL,  // phoney
#endif
#ifdef USE_CMD_LN_H
    // COMMAND_INDEX__LN_H,
    &cmdl_identity_setup,
    // COMMAND_INDEX__LN_H__RUN,
    NULL,  // phoney
#endif
#ifdef USE_CMD_MV
    // COMMAND_INDEX__MV,
    &cmdl_identity_setup,
    // COMMAND_INDEX__MV__RUN,
    NULL,  // phoney
#endif
#ifdef USE_CMD_SLEEP
    // COMMAND_INDEX__SLEEP,
    &cmdl_identity_setup,
#endif
#ifdef USE_CMD_SIGNAL
    // COMMAND_INDEX__SIGNAL,
    &cmdl_identity_setup,
#endif
#ifdef USE_CMD_EXEC
    // COMMAND_INDEX__EXEC,
    &cmdl_identity_setup,
#endif
    // COMMAND_INDEX__ERR
    //   End List marker
};

CommandFunc command_function[] = {
    // COMMAND_INDEX__NOOP
    &cmdl_ok,

    // COMMAND_INDEX__VERSION,
    NULL,  // version has no runner

#ifdef USES_FMODE
    // COMMAND_INDEX__FMODE,
    &cmd_fmode_run,
#endif

#ifdef USE_CMD_ECHO
    // COMMAND_INDEX__ECHO,
    &cmd_echo_run,
#endif
#ifdef USE_CMD_RM
    // COMMAND_INDEX__RM,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_RMDIR
    // COMMAND_INDEX__RMDIR,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_TOUCH
    // COMMAND_INDEX__TOUCH,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_TRUNC
    // COMMAND_INDEX__TRUNC,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_DUP_R
    // COMMAND_INDEX__DUP_R,
    NULL,  // special command that is never run directly
#endif
#ifdef USE_CMD_DUP_W
    // COMMAND_INDEX__DUP_W,
    NULL,  // special command that is never run directly
#endif
#ifdef USE_CMD_DUP_A
    // COMMAND_INDEX__DUP_A,
    NULL,  // special command that is never run directly
#endif

#ifdef USES_DUP
    // COMMAND_INDEX__DUP__FD,
    &cmdl_toint10_arg__runner,
    // COMMAND_INDEX__DUP__TGT,
    &cmd_dup_run,
#endif

#ifdef USE_CMD_MKNOD
    // COMMAND_INDEX__MKNOD,
    &cmdl_FIXME,
    // COMMAND_INDEX__MKNOD__RUN,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_MKDEV
    // COMMAND_INDEX__MKDEV,
    &cmdl_FIXME,
    // COMMAND_INDEX__MKDEV__MAJOR,
    &cmdl_toint10_arg__runner,
    // COMMAND_INDEX__MKDEV__MINOR,
    &cmdl_toint10_arg__runner,
    // COMMAND_INDEX__MKDEV__RUN,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_MKDIR
    // COMMAND_INDEX__MKDIR,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_CHOWN
    // COMMAND_INDEX__CHOWN,
    &cmdl_toint10_arg__runner,
    // COMMAND_INDEX__CHOWN__GROUP,
    &cmdl_toint10_arg__runner,
    // COMMAND_INDEX__CHOWN__RUN,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_CHMOD
    // COMMAND_INDEX__CHMOD,
    &cmd_chmod_mod_arg,
    // COMMAND_INDEX__CHMOD__RUN,
    &cmd_chmod_run,
#endif
#ifdef USE_CMD_LN_S
    // COMMAND_INDEX__LN_S,
    &cmdl_store_arg__runner,
    // COMMAND_INDEX__LN_S__RUN,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_LN_H
    // COMMAND_INDEX__LN_H,
    &cmdl_store_arg__runner,
    // COMMAND_INDEX__LN_H__RUN,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_MV
    // COMMAND_INDEX__MV,
    &cmdl_store_arg__runner,
    // COMMAND_INDEX__MV__RUN,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_SLEEP
    // COMMAND_INDEX__SLEEP,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_SIGNAL
    // COMMAND_INDEX__SIGNAL,
    &cmdl_FIXME,
#endif
#ifdef USE_CMD_EXEC
    // COMMAND_INDEX__EXEC,
    NULL,  // cmd is parsed very specially in its setup.
#endif
    // COMMAND_INDEX__ERR
    //   End List marker
};
