// GENERATED FROM cmd_test_connection.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_TEST_CONNECTION_
#define _FS_SHELL__CMD_TEST_CONNECTION_



#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// The test uses:
//    parsed address: test_connection_addr
//    port number: global_arg2_i
//    address family: global_arg1_i
//    timeout: global_arg3_i


// Optional command test-ipv4

#ifdef USE_CMD_TEST_IPV4



/* from cmd_test_connection.h.in:45 */
extern const char cmd_name_test_ipv4[];
#define ENUM_LIST__TEST_IPV4 \
            /* from cmd_test_connection.h.in:45 */ \
            COMMAND_INDEX__TEST_IPV4,
#define VIRTUAL_ENUM_LIST__TEST_IPV4
#define GLOBAL_VARDEF__TEST_IPV4 \
            /* from cmd_test_connection.h.in:45 */ \
            const char cmd_name_test_ipv4[] = "test-ipv4";
#define INITIALIZE__TEST_IPV4 \
            /* from cmd_test_connection.h.in:45 */ \
            command_list_names[COMMAND_INDEX__TEST_IPV4] = cmd_name_test_ipv4;
#define STARTUP_CASE__TEST_IPV4
#define RUN_CASE__TEST_IPV4 \
    case COMMAND_INDEX__TEST_IPV4: \
        /* from cmd_test_connection.h.in:45 */ \
            /* from cmd_test_connection.h.in:47 */ \
            LOG(":: test ipv4 addr "); \
            LOGLN(global_arg); \
            global_arg3_i = COMMAND_INDEX__TEST_CONNECTION__TIMEOUT; \
            global_cmd = COMMAND_INDEX__SHARED_INT2; \
            test_connection_addr.ip4.sin_family = AF_INET; \
            tmp_val = inet_pton(AF_INET, global_arg, &test_connection_addr.ip4.sin_addr.s_addr); \
            if (tmp_val <= 0) { \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
            } \
        break;
#define REQUIRES_ADDL_ARG__TEST_IPV4

#else /* USE_CMD_TEST_IPV4 */

#define ENUM_LIST__TEST_IPV4
#define VIRTUAL_ENUM_LIST__TEST_IPV4
#define GLOBAL_VARDEF__TEST_IPV4
#define INITIALIZE__TEST_IPV4
#define STARTUP_CASE__TEST_IPV4
#define RUN_CASE__TEST_IPV4
#define REQUIRES_ADDL_ARG__TEST_IPV4
#endif /* USE_CMD_TEST_IPV4 */


// Optional command test-ipv6

#ifdef USE_CMD_TEST_IPV6



/* from cmd_test_connection.h.in:69 */
extern const char cmd_name_test_ipv6[];
#define ENUM_LIST__TEST_IPV6 \
            /* from cmd_test_connection.h.in:69 */ \
            COMMAND_INDEX__TEST_IPV6,
#define VIRTUAL_ENUM_LIST__TEST_IPV6
#define GLOBAL_VARDEF__TEST_IPV6 \
            /* from cmd_test_connection.h.in:69 */ \
            const char cmd_name_test_ipv6[] = "test-ipv6";
#define INITIALIZE__TEST_IPV6 \
            /* from cmd_test_connection.h.in:69 */ \
            command_list_names[COMMAND_INDEX__TEST_IPV6] = cmd_name_test_ipv6;
#define STARTUP_CASE__TEST_IPV6
#define RUN_CASE__TEST_IPV6 \
    case COMMAND_INDEX__TEST_IPV6: \
        /* from cmd_test_connection.h.in:69 */ \
            /* from cmd_test_connection.h.in:71 */ \
            LOG(":: test ipv6 addr "); \
            LOGLN(global_arg); \
            global_arg3_i = COMMAND_INDEX__TEST_CONNECTION__TIMEOUT; \
            global_cmd = COMMAND_INDEX__SHARED_INT2; \
            test_connection_addr.ip6.sin6_family = AF_INET6; \
            tmp_val = inet_pton(AF_INET6, global_arg, &test_connection_addr.ip6.sin6_addr.s6_addr); \
            if (tmp_val <= 0) { \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
            } \
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


#if defined(USE_CMD_TEST_IPV4) || defined(USE_CMD_TEST_IPV6)


union test_connection_ip_addr {
    struct sockaddr_in ip4;
    struct sockaddr_in6 ip6;
};



#define ENUM_LIST__TEST_CONNECTION \
            ENUM_LIST__TEST_IPV4 \
            ENUM_LIST__TEST_IPV6
#define VIRTUAL_ENUM_LIST__TEST_CONNECTION \
            VIRTUAL_ENUM_LIST__TEST_IPV4 \
            VIRTUAL_ENUM_LIST__TEST_IPV6 \
            /* from cmd_test_connection.h.in:98 */ \
            COMMAND_INDEX__TEST_CONNECTION__TIMEOUT,
#define GLOBAL_VARDEF__TEST_CONNECTION \
            GLOBAL_VARDEF__TEST_IPV4 \
            GLOBAL_VARDEF__TEST_IPV6
#define INITIALIZE__TEST_CONNECTION \
            INITIALIZE__TEST_IPV4 \
            INITIALIZE__TEST_IPV6 \
            /* from cmd_test_connection.h.in:100 */ \
            union test_connection_ip_addr test_connection_addr; \
            short int test_connection_sock = -1; \
            fd_set test_connection_fdset; \
            struct timeval test_connection_tv;
#define STARTUP_CASE__TEST_CONNECTION \
            STARTUP_CASE__TEST_IPV4 \
            STARTUP_CASE__TEST_IPV6
#define RUN_CASE__TEST_CONNECTION \
            RUN_CASE__TEST_IPV4 \
            RUN_CASE__TEST_IPV6 \
    case COMMAND_INDEX__TEST_CONNECTION__TIMEOUT: \
        /* from cmd_test_connection.h.in:98 */ \
            /* from cmd_test_connection.h.in:107 */ \
            LOG(":: parsing timeout "); \
            LOGLN(global_arg); \
            global_arg3_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
            if (global_arg3_i < 0) { \
                LOG(":: invalid integer for timeout\n"); \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
                break; \
            } \
            /* Note: with the union, the family and port for ip4 and ip6*/ \
            /*   are on the same memory address.*/ \
            test_connection_addr.ip4.sin_port = htons(global_arg2_i); \
            /* Perform connection.*/ \
            test_connection_sock = socket(test_connection_addr.ip4.sin_family, SOCK_STREAM, 0); \
            fcntl(test_connection_sock, F_SETFL, O_NONBLOCK); \
            /* Because the socket is in non-blocking mode,*/ \
            /* chances are that running connect will return an error with*/ \
            /*   errno == EINPROGRESS, meaning connection is in-progress.*/ \
            if (test_connection_addr.ip4.sin_family == AF_INET) { \
                /* printf("Connecting to %x port %d\n", test_connection_addr.ip4.sin_addr.s_addr, test_connection_addr.ip4.sin_port);*/ \
                connect( \
                    test_connection_sock, \
                    (struct sockaddr *)&test_connection_addr.ip4, \
                    sizeof(struct sockaddr_in)); \
            } else { \
                /* \
                printf("Connecting to "); \
                for (tmp_val = 0; tmp_val < sizeof(struct in6_addr); tmp_val++) { \
                    printf("%02x:", test_connection_addr.ip6.sin6_addr.s6_addr[tmp_val]); \
                } \
                printf(" port %d\n", test_connection_addr.ip4.sin_port); \
                */ \
                connect(test_connection_sock, \
                    (struct sockaddr *)&test_connection_addr.ip6, \
                    sizeof(struct sockaddr_in6)); \
            } \
            FD_ZERO(&test_connection_fdset); \
            FD_SET(test_connection_sock, &test_connection_fdset); \
            test_connection_tv.tv_sec = global_arg3_i; \
            test_connection_tv.tv_usec = 0; \
            /* Default to not being open*/ \
            global_err = 1; \
            if (select(test_connection_sock + 1, NULL, &test_connection_fdset, NULL, &test_connection_tv) == 1) \
            { \
                socklen_t len = sizeof(global_err); \
                getsockopt(test_connection_sock, SOL_SOCKET, SO_ERROR, &global_err, &len); \
                if (global_err == 0) { \
                    LOG(":: connection open\n"); \
                } else { \
                    LOG(":: socket has error\n"); \
                    /* printf("socket error %d\n", global_err);*/ \
                } \
            } else { \
                LOG(":: socket did not connect in time.\n"); \
            } \
            close(test_connection_sock); \
            global_cmd = COMMAND_INDEX__ERR; \
        break;
#define REQUIRES_ADDL_ARG__TEST_CONNECTION \
            REQUIRES_ADDL_ARG__TEST_IPV4 \
            REQUIRES_ADDL_ARG__TEST_IPV6

#else /* defined(USE_CMD_TEST_IPV4) || defined(USE_CMD_TEST_IPV6) */

#define ENUM_LIST__TEST_CONNECTION
#define VIRTUAL_ENUM_LIST__TEST_CONNECTION
#define GLOBAL_VARDEF__TEST_CONNECTION
#define INITIALIZE__TEST_CONNECTION
#define STARTUP_CASE__TEST_CONNECTION
#define RUN_CASE__TEST_CONNECTION
#define REQUIRES_ADDL_ARG__TEST_CONNECTION
#endif /* defined(USE_CMD_TEST_IPV4) || defined(USE_CMD_TEST_IPV6) */


#endif /* _FS_SHELL__CMD_TEST_CONNECTION_ */
