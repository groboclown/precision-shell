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

const char intern__empty[] = "";
const char intern__noop[] = "noop";
const char intern__version[] = "version";
#ifdef USES_FMODE
const char intern__fmode[] = "fmode";
#endif
#ifdef USE_CMD_ECHO
const char intern__echo[] = "echo";
#endif
#ifdef USE_CMD_RM
const char intern__rm[] = "rm";
#endif
#ifdef USE_CMD_RMDIR
const char intern__rmdir[] = "rmdir";
#endif
#ifdef USE_CMD_TOUCH
const char intern__touch[] = "touch";
#endif
#ifdef USE_CMD_TRUNC
const char intern__trunc[] = "trunc";
#endif
#ifdef USE_CMD_DUP_R
const char intern__dup_r[] = "dup-r";
#endif
#ifdef USE_CMD_DUP_W
const char intern__dup_w[] = "dup-w";
#endif
#ifdef USE_CMD_DUP_A
const char intern__dup_a[] = "dup-a";
#endif
#ifdef USE_CMD_MKNOD
const char intern__mknod[] = "mknod";
#endif
#ifdef USE_CMD_MKDEV
const char intern__mkdev[] = "mkdev";
#endif
#ifdef USE_CMD_MKDIR
const char intern__mkdir[] = "mkdir";
#endif
#ifdef USE_CMD_CHOWN
const char intern__chown[] = "chown";
#endif
#ifdef USE_CMD_CHMOD
const char intern__chmod[] = "chmod";
#endif
#ifdef USE_CMD_LN_S
const char intern__ln_s[] = "ln-s";
#endif
#ifdef USE_CMD_LN_H
const char intern__ln_h[] = "ln-h";
#endif
#ifdef USE_CMD_MV
const char intern__mv[] = "mv";
#endif
#ifdef USE_CMD_SLEEP
const char intern__sleep[] = "sleep";
#endif
#ifdef USE_CMD_SIGNAL
const char intern__signal[] = "signal";
#endif
#ifdef USE_CMD_EXEC
const char intern__exec[] = "exec";
#endif

const char *intern_all_names[COMMAND_INDEX__ERR];

const char ** get_command_list_names() {
    for (int i = 0; i < COMMAND_INDEX__ERR; i++) {
        intern_all_names[i] = intern__empty;
    }
    // intern_all_names[COMMAND_INDEX__FIND_CMD] = intern__empty;
    intern_all_names[COMMAND_INDEX__NOOP] = intern__noop;
    intern_all_names[COMMAND_INDEX__VERSION] = intern__version;

#ifdef USES_FMODE
    intern_all_names[COMMAND_INDEX__FMODE] = intern__fmode;
#endif

#ifdef USE_CMD_ECHO
    intern_all_names[COMMAND_INDEX__ECHO] = intern__echo;
#endif

#ifdef USE_CMD_RM
    intern_all_names[COMMAND_INDEX__RM] = intern__rm;
#endif

#ifdef USE_CMD_RMDIR
    intern_all_names[COMMAND_INDEX__RMDIR] = intern__rmdir;
#endif

#ifdef USE_CMD_TOUCH
    intern_all_names[COMMAND_INDEX__TOUCH] = intern__touch;
#endif

#ifdef USE_CMD_TRUNC
    // ;
    intern_all_names[COMMAND_INDEX__TRUNC] = intern__trunc;
#endif
#if defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC)
    // ;
    //intern_all_names[COMMAND_INDEX__TRUNC_TOUCH__RUN] = intern__empty;
#endif

#ifdef USE_CMD_DUP_R
    // ;
    intern_all_names[COMMAND_INDEX__DUP_R] = intern__dup_r;
#endif

#ifdef USE_CMD_DUP_W
    // ;
    intern_all_names[COMMAND_INDEX__DUP_W] = intern__dup_w;
#endif

#ifdef USE_CMD_DUP_A
    // ;
    intern_all_names[COMMAND_INDEX__DUP_A] = intern__dup_a;
#endif

#ifdef USES_DUP
    // ;
    //intern_all_names[COMMAND_INDEX__DUP__FD] = intern__empty;
    // ;
    //intern_all_names[COMMAND_INDEX__DUP__TGT] = intern__empty;
#endif

#ifdef USE_CMD_MKNOD
    // ;
    intern_all_names[COMMAND_INDEX__MKNOD] = intern__mknod;
#endif
#ifdef USE_CMD_MKDEV
    // ;
    intern_all_names[COMMAND_INDEX__MKDEV] = intern__mkdev;
    // ;
    // intern_all_names[COMMAND_INDEX__MKDEV__MINOR] = intern__empty;
#endif
#ifdef USES_MKNOD
    // ;
    //intern_all_names[COMMAND_INDEX__MKNOD_DEV__TYPE] = intern__empty;
    // ;
    //intern_all_names[COMMAND_INDEX__MKNOD_DEV__RUN] = intern__empty;
#endif

#ifdef USE_CMD_MKDIR
    // ;
    intern_all_names[COMMAND_INDEX__MKDIR] = intern__mkdir;
#endif

#ifdef USE_CMD_CHOWN
    // ;
    intern_all_names[COMMAND_INDEX__CHOWN] = intern__chown;
    // ;
    // intern_all_names[COMMAND_INDEX__CHOWN__GROUP] = intern__empty;
    // ;
    // intern_all_names[COMMAND_INDEX__CHOWN__RUN] = intern__empty;
#endif

#ifdef USE_CMD_CHMOD
    // ;
    intern_all_names[COMMAND_INDEX__CHMOD] = intern__chmod;
    // ;
    // intern_all_names[COMMAND_INDEX__CHMOD__RUN] = intern__empty;
#endif

#ifdef USE_CMD_LN_S
    // ;
    intern_all_names[COMMAND_INDEX__LN_S] = intern__ln_s;
#endif

#ifdef USE_CMD_LN_H
    // ;
    intern_all_names[COMMAND_INDEX__LN_H] = intern__ln_h;
#endif

#ifdef USE_CMD_MV
    // ;
    intern_all_names[COMMAND_INDEX__MV] = intern__mv;
#endif

#ifdef USE_CMD_SLEEP
    // ;
    intern_all_names[COMMAND_INDEX__SLEEP] = intern__sleep;
#endif

#ifdef USE_CMD_SIGNAL
    // ;
    intern_all_names[COMMAND_INDEX__SIGNAL] = intern__signal;
#endif

#ifdef USE_CMD_EXEC
    // ;
    intern_all_names[COMMAND_INDEX__EXEC] = intern__exec;
#endif
    return intern_all_names;
}
