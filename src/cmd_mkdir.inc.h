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

#ifndef _FS_SHELL__CMD_MKDIR_

// No startup execution
#define STARTUP__COMMAND_INDEX__MKDIR


#ifdef USE_CMD_MKDIR

#include <fcntl.h>

#define CASE__COMMAND_INDEX__MKDIR \
case COMMAND_INDEX__MKDIR: \
    LOG(":: mkdir "); \
    LOGLN(global_arg); \
    /* For now, assume that execution is enabled for all modes. */ \
    global_arg1_i = global_fmode | S_IXUSR | S_IXGRP | S_IXOTH; \
    global_err = mkdir(global_arg, global_arg1_i); \
    break;

#else /* USE_CMD_MKDIR */

#define CASE__COMMAND_INDEX__MKDIR


#endif /* USE_CMD_MKDIR */
#endif /* _FS_SHELL__CMD_MKDIR_ */
