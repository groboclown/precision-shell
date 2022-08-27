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


// --------------------------------------------------------------------------
// Special commands that imply other commands must be included.


// USE_CMD_START_TIMER - this command is always included if any
//   of the timer commands is also used.
#if    defined(USE_CMD_ELAPSED_TIME) \
    || defined(USE_CMD_ELAPSED_TIME_UNDER) \
    || defined(USE_CMD_EXPORT_ELAPSED_TIME)

#define USE_CMD_START_TIMER 1

#endif


// --------------------------------------------------------------------------
// Derived uses

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


// USES_SHARED_STR - shared logic for capturing an argument for later use.
#if    defined(USE_CMD_LN_S) \
    || defined(USE_CMD_LN_H) \
    || defined(USE_CMD_MV) \
    || defined(USE_CMD_EXPORT_HOST_LOOKUP) \
    || defined(USE_CMD_EXPORT) \
    || defined(USE_CMD_WHILE_NO_ERROR) \
    || defined(USE_CMD_WHILE_ERROR) \
    || defined(USE_CMD_FOR_EACH) 

#define USES_SHARED_STR

#endif

// USES_SHARED_INT - shared logic for capturing an argument for later use.
#if    defined(USES_DUP) \
    || defined(USE_CMD_LN_H) \
    || defined(USE_CMD_MV) \
    || defined(USE_CMD_MKDEV) \
    || defined(USE_CMD_EXPORT) \
    || defined(USE_CMD_CHOWN) \
    || defined(USE_CMD_WRITE_FD) \
    || defined(USE_CMD_CAT_FD) \
    || defined(USE_CMD_ENV_CAT_FD) \
    || defined(USE_CMD_SU_EXEC) \
    || defined(USE_CMD_SU_SPAWN) \
    || defined(USE_CMD_TEST_IPV4) \
    || defined(USE_CMD_TEST_IPV6) \
    || defined(USE_CMD_EXPECT_HTTP_GET_RESPONSE) \
    || defined(USE_CMD_EXPECT_HTTP_GET_RESPONSE_NOT)

#define USES_SHARED_INT

#endif


// USES_FD_READ - any command or capability that allows for reading from a file descriptor.
#if    defined(USE_STREAMING_INPUT) \
    || defined(USE_CMD_CAT_FD) \
    || defined(USE_CMD_EXPORT_HOST_LOOKUP) \
    || defined(USE_CMD_ENV_CAT_FD)

#define USES_FD_READ

#endif


// USES_ENVIRONMENT - any command or capability that performs management of environment variables.
#if    defined(USE_ENVIROMENT_INPUT) \
    || defined(USE_CMD_ENV_CAT_FD)

#define USES_ENVIRONMENT

// Even though these use setenv, it itself doesn't require
//   the environment parser.
//    || defined(USE_CMD_EXPORT)
//    || defined(USE_CMD_SPAWN)
//    || defined(USE_CMD_PWD)
//    || defined(USE_CMD_WAIT_PID)
//    || defined(USE_CMD_EXPORT_HOST_LOOKUP)

#endif


// USES_SHARED_SPLIT_ARG - shared data for splitting one arg into multiple args.
#if    defined(USE_CMD_EXEC) \
    || defined(USE_CMD_SPAWN) \
    || defined(USE_CMD_WAIT_PID) \
    || defined(USE_CMD_SUBCMD) \
    || defined(USE_CMD_EXPORT_HOST_LOOKUP) \
    || defined(USE_CMD_IF_ELSE) \
    || defined(USE_CMD_WHILE_NO_ERROR) \
    || defined(USE_CMD_WHILE_ERROR) \
    || defined(USE_CMD_FOR_EACH) \
    || defined(USE_CMD_SU_EXEC) \
    || defined(USE_CMD_SU_SPAWN)

#define USES_SHARED_SPLIT_ARG

#endif


// USES_SHARED_ITOA - shared integer to ascii code.
#if    defined(USE_CMD_SPAWN) \
    || defined(USE_CMD_SU_SPAWN) \
    || defined(USE_CMD_WAIT_PID) \
    || defined(USE_CMD_FILE_STAT) \
    || defined(USE_CMD_LS_L) \
    || defined(USE_CMD_ELAPSED_TIME) \
    || defined(USE_CMD_EXPORT_ELAPSED_TIME)

#define USES_SHARED_ITOA

#endif


// USES_SHARED_ADDRESS_IPV4 - allow connecting to ipv6 sockets and parsing ipv4 addresses
#if    defined(USE_CMD_TEST_IPV4) \
    || defined(USE_CMD_EXPECT_HTTP_GET_RESPONSE) \
    || defined(USE_CMD_EXPECT_HTTP_GET_RESPONSE_NOT)

#define USES_SHARED_ADDRESS_IPV4

#endif


// USES_SHARED_ADDRESS_IPV6 - allow connecting to ipv6 sockets and parsing ipv6 addresses
#if    defined(USE_CMD_TEST_IPV6)

#define USES_SHARED_ADDRESS_IPV6

#endif


#endif /* _FS_SHELL__USES_ */
