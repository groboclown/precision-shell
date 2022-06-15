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
#ifndef _FS_SHELL__USES_
#define _FS_SHELL__USES_

// The first includes.


// USES_SIGNALS - signal processing and special global variables are used.
#ifdef USE_CMD_SIGNAL

#define USES_SIGNALS 1

#endif


// USES_DUP - uses at least one of the file descriptor dup commands.
#if defined(USE_CMD_DUP_A) || defined(USE_CMD_DUP_R) || defined(USE_CMD_DUP_W)

#define USES_DUP 1

#endif


#if defined(USE_CMD_MKNOD) || defined(USE_CMD_MKDEV)

#define USES_MKNOD 1

#endif


// USES_FMODE - special file mode processing and handling.
//   Note that chmod does not use this!
#if    defined(USE_CMD_MKDIR) \
    || defined(USE_CMD_TOUCH) \
    || defined(USE_CMD_TRUNC) \
    || defined(USES_MKNOD) \
    || defined(USES_DUP)

#define USES_FMODE 1

#endif


#endif /* _FS_SHELL__USES_ */
