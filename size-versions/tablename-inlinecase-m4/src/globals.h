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


// Name of the program running
extern const char *global_invoked_name;

// index -> command name for parsing ease.
// Some compilers generate bad memory addresses when this is a const char *[].
//   This fixed Nx9 array means there's wasted space for many commands, but it builds a
//   really easy structure for things to parse.  The alternative is a bit more memory
//   conservative, but is really funky to create correctly (at least I couldn't do it with
//   dietlibc).
extern const char **command_list_names;

// These are "globals" insomuch as they
//   can be used by in-lined macros.
#define LOCAL_GLOBALS \
    \
    /* The current argument being parsed. */ \
    const char *global_arg; \
    \
    /* The current command name being run. */ \
    const char *global_cmd_name; \
    \
    /* The current command index. */ \
    int global_cmd; \
    \
    /* Marker that there's an error in the current command. */ \
    int global_err; \
    \
    /* Common global values for cross-command storage. */ \
    const char *global_arg_cached; \
    int global_arg1_i; \
    int global_arg2_i; \
    int global_arg3_i;



#endif /* _FS_SHELL__GLOBALS_ */
