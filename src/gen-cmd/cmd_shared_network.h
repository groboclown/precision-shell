// GENERATED FROM cmd_shared_network.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_SHARED_NETWORK_
#define _FS_SHELL__CMD_SHARED_NETWORK_


// Standard Network Communication Tools

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// Optional command shared-address-ipv4

#ifdef USES_SHARED_ADDRESS_IPV4

    // Collects the IPv4 address and port number into the sockaddr_in structure.
    // Pass the next command to run in global_arg2_i.
    // To connect to an address, use:
    //   my_socket = shared_connect_address(
    //     (struct sockaddr *)&shared_address_ip4, sizeof(shared_address_ip4), timeout);
    //   if (my_socket == -1) { error handling }



#define ENUM_LIST__SHARED_ADDRESS_IPV4
#define VIRTUAL_ENUM_LIST__SHARED_ADDRESS_IPV4 \
            /* from cmd_shared_network.h.in:46 */ \
            COMMAND_INDEX__SHARED_IPV4_ADDRESS,
#define GLOBAL_VARDEF__SHARED_ADDRESS_IPV4
#define INITIALIZE__SHARED_ADDRESS_IPV4 \
            /* from cmd_shared_network.h.in:47 */ \
            /* Don't forget to set the port with:*/ \
            /*  shared_address_ip4.sin_port = htons(global_arg2_i);*/ \
            struct sockaddr_in shared_address_ip4;
#define STARTUP_CASE__SHARED_ADDRESS_IPV4
#define RUN_CASE__SHARED_ADDRESS_IPV4 \
    case COMMAND_INDEX__SHARED_IPV4_ADDRESS: \
        /* from cmd_shared_network.h.in:46 */ \
            /* from cmd_shared_network.h.in:53 */ \
            LOG(":: parsing ipv4 addr "); \
            LOGLN(global_arg); \
            global_cmd = global_arg2_i; \
            shared_address_ip4.sin_family = AF_INET; \
            tmp_val = inet_pton(AF_INET, global_arg, &shared_address_ip4.sin_addr.s_addr); \
            if (tmp_val <= 0) { \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
            } \
        break;
#define REQUIRES_ADDL_ARG__SHARED_ADDRESS_IPV4

#else /* USES_SHARED_ADDRESS_IPV4 */

#define ENUM_LIST__SHARED_ADDRESS_IPV4
#define VIRTUAL_ENUM_LIST__SHARED_ADDRESS_IPV4
#define GLOBAL_VARDEF__SHARED_ADDRESS_IPV4
#define INITIALIZE__SHARED_ADDRESS_IPV4
#define STARTUP_CASE__SHARED_ADDRESS_IPV4
#define RUN_CASE__SHARED_ADDRESS_IPV4
#define REQUIRES_ADDL_ARG__SHARED_ADDRESS_IPV4
#endif /* USES_SHARED_ADDRESS_IPV4 */


// Optional command shared-address-ipv6

#ifdef USES_SHARED_ADDRESS_IPV6

    // Collects the IPv6 address and port number into the sockaddr_in6 structure.
    // Pass the next command to run in global_arg2_i.
    // To connect to an address, use:
    //   my_socket = shared_connect_address(
    //     (struct sockaddr *)&shared_address_ip6, sizeof(shared_address_ip6), timeout);
    //   if (my_socket == -1) { error handling }
    //   else {  doStuf(); close(my_socket); }



#define ENUM_LIST__SHARED_ADDRESS_IPV6
#define VIRTUAL_ENUM_LIST__SHARED_ADDRESS_IPV6 \
            /* from cmd_shared_network.h.in:81 */ \
            COMMAND_INDEX__SHARED_IPV6_ADDRESS,
#define GLOBAL_VARDEF__SHARED_ADDRESS_IPV6
#define INITIALIZE__SHARED_ADDRESS_IPV6 \
            /* from cmd_shared_network.h.in:83 */ \
            /* Don't forget to set the port with:*/ \
            /*  shared_address_ip6.sin6_port = htons(global_arg2_i);*/ \
            struct sockaddr_in6 shared_address_ip6;
#define STARTUP_CASE__SHARED_ADDRESS_IPV6
#define RUN_CASE__SHARED_ADDRESS_IPV6 \
    case COMMAND_INDEX__SHARED_IPV6_ADDRESS: \
        /* from cmd_shared_network.h.in:81 */ \
            /* from cmd_shared_network.h.in:89 */ \
            LOG(":: parsing ipv6 addr "); \
            LOGLN(global_arg); \
            global_cmd = global_arg2_i; \
            shared_address_ip6.sin6_family = AF_INET6; \
            tmp_val = inet_pton(AF_INET6, global_arg, &shared_address_ip6.sin6_addr.s6_addr); \
            if (tmp_val <= 0) { \
                global_err = 1; \
                global_cmd = COMMAND_INDEX__ERR; \
            } \
        break;
#define REQUIRES_ADDL_ARG__SHARED_ADDRESS_IPV6

#else /* USES_SHARED_ADDRESS_IPV6 */

#define ENUM_LIST__SHARED_ADDRESS_IPV6
#define VIRTUAL_ENUM_LIST__SHARED_ADDRESS_IPV6
#define GLOBAL_VARDEF__SHARED_ADDRESS_IPV6
#define INITIALIZE__SHARED_ADDRESS_IPV6
#define STARTUP_CASE__SHARED_ADDRESS_IPV6
#define RUN_CASE__SHARED_ADDRESS_IPV6
#define REQUIRES_ADDL_ARG__SHARED_ADDRESS_IPV6
#endif /* USES_SHARED_ADDRESS_IPV6 */


#if defined(USES_SHARED_ADDRESS_IPV4) || defined(USES_SHARED_ADDRESS_IPV6)


#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int shared_connect_address(
        struct sockaddr *address, size_t address_size,
        int timeout_seconds) {
    short int socket_fd;
    fd_set fdset;
    int error;
    struct timeval tv;
    socklen_t len;

    // Perform connection.
    //   The address will either be a sockaddr_in or sockaddr_in6, and
    //   in both cases the family is in the same location, so this is a safe cast.
    socket_fd = socket(
        ((struct sockaddr_in *)address)->sin_family,
        SOCK_STREAM, 0);
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    // Because the socket is in non-blocking mode,
    //   chances are that running connect will return an error with
    //   errno == EINPROGRESS, meaning connection is in-progress.
    connect(socket_fd, address, address_size);
    FD_ZERO(&fdset);
    FD_SET(socket_fd, &fdset);
    tv.tv_sec = timeout_seconds;
    tv.tv_usec = 0;

    // Default to not being open
    if (select(socket_fd + 1, NULL, &fdset, NULL, &tv) == 1)
    {
        len = sizeof(error);
        getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &error, &len);

        if (error == 0) {
            LOG(":: connection open\n");
            // Trun the socket back to blocking mode.
            fcntl(socket_fd, F_SETFL, 0);
            return socket_fd;
        } else {
            LOG(":: socket has error\n");
            // printf("socket error %d\n", error);
            return -1;
        }
    }
    LOG(":: socket did not connect in time.\n");
    return -1;
}


#define ENUM_LIST__SHARED_NETWORK \
            ENUM_LIST__SHARED_ADDRESS_IPV4 \
            ENUM_LIST__SHARED_ADDRESS_IPV6
#define VIRTUAL_ENUM_LIST__SHARED_NETWORK \
            VIRTUAL_ENUM_LIST__SHARED_ADDRESS_IPV4 \
            VIRTUAL_ENUM_LIST__SHARED_ADDRESS_IPV6
#define GLOBAL_VARDEF__SHARED_NETWORK \
            GLOBAL_VARDEF__SHARED_ADDRESS_IPV4 \
            GLOBAL_VARDEF__SHARED_ADDRESS_IPV6
#define INITIALIZE__SHARED_NETWORK \
            INITIALIZE__SHARED_ADDRESS_IPV4 \
            INITIALIZE__SHARED_ADDRESS_IPV6
#define STARTUP_CASE__SHARED_NETWORK \
            STARTUP_CASE__SHARED_ADDRESS_IPV4 \
            STARTUP_CASE__SHARED_ADDRESS_IPV6
#define RUN_CASE__SHARED_NETWORK \
            RUN_CASE__SHARED_ADDRESS_IPV4 \
            RUN_CASE__SHARED_ADDRESS_IPV6
#define REQUIRES_ADDL_ARG__SHARED_NETWORK \
            REQUIRES_ADDL_ARG__SHARED_ADDRESS_IPV4 \
            REQUIRES_ADDL_ARG__SHARED_ADDRESS_IPV6

#else /* defined(USES_SHARED_ADDRESS_IPV4) || defined(USES_SHARED_ADDRESS_IPV6) */

#define ENUM_LIST__SHARED_NETWORK
#define VIRTUAL_ENUM_LIST__SHARED_NETWORK
#define GLOBAL_VARDEF__SHARED_NETWORK
#define INITIALIZE__SHARED_NETWORK
#define STARTUP_CASE__SHARED_NETWORK
#define RUN_CASE__SHARED_NETWORK
#define REQUIRES_ADDL_ARG__SHARED_NETWORK
#endif /* defined(USES_SHARED_ADDRESS_IPV4) || defined(USES_SHARED_ADDRESS_IPV6) */


#endif /* _FS_SHELL__CMD_SHARED_NETWORK_ */
