// GENERATED FROM cmd_test_ipv6.h.in.  DO NOT EDIT.

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



#ifndef _FS_SHELL__CMD_TEST_IPV6_
#define _FS_SHELL__CMD_TEST_IPV6_

// Optional command test-ipv6

#ifdef USE_CMD_TEST_IPV6




/* from cmd_test_ipv6.h.in:28 */
extern const char cmd_name_test_ipv6[];
#define ENUM_LIST__TEST_IPV6 \
            /* from cmd_test_ipv6.h.in:28 */ \
            COMMAND_INDEX__TEST_IPV6,
#define VIRTUAL_ENUM_LIST__TEST_IPV6 \
            /* from cmd_test_ipv6.h.in:45 */ \
            COMMAND_INDEX__TEST_IPV6__TIMEOUT,
#define GLOBAL_VARDEF__TEST_IPV6 \
            /* from cmd_test_ipv6.h.in:28 */ \
            const char cmd_name_test_ipv6[] = "test-ipv6";
#define INITIALIZE__TEST_IPV6 \
            /* from cmd_test_ipv6.h.in:28 */ \
            command_list_names[COMMAND_INDEX__TEST_IPV6] = cmd_name_test_ipv6;
#define STARTUP_CASE__TEST_IPV6 \
    case COMMAND_INDEX__TEST_IPV6: \
        /* from cmd_test_ipv6.h.in:28 */ \
            /* from cmd_test_ipv6.h.in:30 */ \
            LOG(":: test ipv6\n"); \
            /* Argument 1: collect address into shared_address_ip6*/ \
            global_cmd = COMMAND_INDEX__SHARED_IPV6_ADDRESS; \
            /* Argument 2: collect port into global_arg2_i.*/ \
            global_arg2_i = COMMAND_INDEX__SHARED_INT2; \
            /* Argument 3: collect timeout + make connection.*/ \
            global_arg3_i = COMMAND_INDEX__TEST_IPV6__TIMEOUT; \
        break;
#define RUN_CASE__TEST_IPV6 \
    case COMMAND_INDEX__TEST_IPV6__TIMEOUT: \
        /* from cmd_test_ipv6.h.in:45 */ \
            /* from cmd_test_ipv6.h.in:47 */ \
            LOG(":: parsing ipv6 timeout "); \
            LOGLN(global_arg); \
            global_arg3_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
            if (global_arg3_i < 0) { \
                LOG(":: invalid integer for timeout\n"); \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
                break; \
            } \
            shared_address_ip6.sin6_port = htons(global_arg2_i); \
            global_cmd = COMMAND_INDEX__ERR; \
            global_err = 0; \
            /* Perform connection.*/ \
            tmp_val = shared_connect_address( \
                (struct sockaddr *)&shared_address_ip6, sizeof(shared_address_ip6), global_arg3_i); \
            if (tmp_val == -1) { \
                global_err = 1; \
                break; \
            } \
            /* Nothing left to do with the socket.*/ \
            close(tmp_val); \
        break;
#define REQUIRES_ADDL_ARG__TEST_IPV6

#else /* USE_CMD_TEST_IPV6 */

#define ENUM_LIST__TEST_IPV6
#define VIRTUAL_ENUM_LIST__TEST_IPV6
#define GLOBAL_VARDEF__TEST_IPV6
#define INITIALIZE__TEST_IPV6
#define STARTUP_CASE__TEST_IPV6
#define RUN_CASE__TEST_IPV6
#define REQUIRES_ADDL_ARG__TEST_IPV6
#endif /* USE_CMD_TEST_IPV6 */

#endif /* _FS_SHELL__CMD_TEST_IPV6_ */
