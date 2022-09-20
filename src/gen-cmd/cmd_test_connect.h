// GENERATED FROM cmd_test_connect.h.in.  DO NOT EDIT.

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



#ifndef _FS_SHELL__CMD_TEST_CONNECT_
#define _FS_SHELL__CMD_TEST_CONNECT_

// Optional command test-connect

#ifdef USE_CMD_TEST_CONNECT



/* from cmd_test_connect.h.in:28 */
extern const char cmd_name_test_connect[];
#define ENUM_LIST__TEST_CONNECT \
            /* from cmd_test_connect.h.in:28 */ \
            COMMAND_INDEX__TEST_CONNECT,
#define VIRTUAL_ENUM_LIST__TEST_CONNECT
#define GLOBAL_VARDEF__TEST_CONNECT \
            /* from cmd_test_connect.h.in:28 */ \
            const char cmd_name_test_connect[] = "test-connect";
#define INITIALIZE__TEST_CONNECT \
            /* from cmd_test_connect.h.in:28 */ \
            command_list_names[COMMAND_INDEX__TEST_CONNECT] = cmd_name_test_connect;
#define STARTUP_CASE__TEST_CONNECT \
    case COMMAND_INDEX__TEST_CONNECT: \
        /* from cmd_test_connect.h.in:28 */ \
            /* from cmd_test_connect.h.in:30 */ \
            LOG(":: test connection\n"); \
            /* Argument 1: collect address shared_connect_address*/ \
            /* Argument 2: collect port/service into global_arg_cached*/ \
            global_cmd = COMMAND_INDEX__SHARED_CONNECT__ADDRESS; \
            /* Argument 3: collect timeout + make connection.*/ \
            global_arg3_i = COMMAND_INDEX__TEST_CONNECT; \
        break;
#define RUN_CASE__TEST_CONNECT \
    case COMMAND_INDEX__TEST_CONNECT: \
        /* from cmd_test_connect.h.in:28 */ \
            /* from cmd_test_connect.h.in:41 */ \
            LOG(":: parsing connection timeout "); \
            LOGLN(global_arg); \
            global_arg3_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
            if (global_arg3_i < 0) { \
                LOG(":: invalid integer for timeout\n"); \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
                break; \
            } \
            global_cmd = COMMAND_INDEX__ERR; \
            global_err = 0; \
            /* Perform connection.*/ \
            tmp_val = shared_connect_to_address( \
                shared_connect_address, global_arg_cached, global_arg3_i, \
                /* For now, only check for TCP connections.*/ \
                IPPROTO_TCP); \
            if (tmp_val == -1) { \
                global_err = 1; \
                break; \
            } \
            /* Completed.  Close off the socket.*/ \
            close(tmp_val); \
        break;
#define REQUIRES_ADDL_ARG__TEST_CONNECT

#else /* USE_CMD_TEST_CONNECT */

#define ENUM_LIST__TEST_CONNECT
#define VIRTUAL_ENUM_LIST__TEST_CONNECT
#define GLOBAL_VARDEF__TEST_CONNECT
#define INITIALIZE__TEST_CONNECT
#define STARTUP_CASE__TEST_CONNECT
#define RUN_CASE__TEST_CONNECT
#define REQUIRES_ADDL_ARG__TEST_CONNECT
#endif /* USE_CMD_TEST_CONNECT */

#endif /* _FS_SHELL__CMD_TEST_CONNECT_ */
