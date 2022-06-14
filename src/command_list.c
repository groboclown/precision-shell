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


// ==========================================================================
// ==========================================================================
// Command Lists
//   IT IS EXTREMELY IMPORTANT THAT THE ORDER HERE MATCHES THE ENUM ORDER EXACTLY


// ==========================================================================
// Names of each command.

const char command_list_names[][9] = {
    // COMMAND_INDEX__FIND_CMD
    "",

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
#if defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC)
    // COMMAND_INDEX__TRUNC_TOUCH__RUN,
    "",  // phoney
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
#endif
#ifdef USE_CMD_MKDEV
    // COMMAND_INDEX__MKDEV,
    "mkdev",
    // COMMAND_INDEX__MKDEV__MINOR,
    "",
#endif
#ifdef USES_MKNOD
    // COMMAND_INDEX__MKNOD_DEV__TYPE,
    "",
    // COMMAND_INDEX__MKNOD_DEV__RUN,
    "",
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
#endif

#ifdef USE_CMD_LN_H
    // COMMAND_INDEX__LN_H,
    "ln-h",
#endif

#ifdef USE_CMD_MV
    // COMMAND_INDEX__MV,
    "mv",
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
