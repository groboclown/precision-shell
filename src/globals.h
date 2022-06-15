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
#ifndef _FS_SHELL__GLOBALS_
#define _FS_SHELL__GLOBALS_


#include "uses.h"

#ifdef USES_SIGNALS
#include <signal.h>
#endif

// Global variables loaded by special argument parsing functions and shread
//   by commands.

// General use argument -> integer values.
extern int global_arg1_i;
extern int global_arg2_i;
extern int global_arg3_i;

// General use argument storage value
extern const char *global_arg_cached;

// Name of the program running
extern const char *global_invoked_name;

// Current command name.
extern const char *global_cmd_name;

// Current argument value
extern const char *global_arg;

// Current command index.
//   This can change while the cmd_name should remain the same.
extern int global_cmd;


// Global file mode.  Set by the fmode command, but used by all kinds of
//   commands.
#ifdef USES_FMODE
extern int global_fmode;
#endif

#ifdef USES_SIGNALS
extern sigset_t global_signal_set;
#endif


#endif /* _FS_SHELL__GLOBALS_ */
