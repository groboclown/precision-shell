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
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


union shared_network_sockaddr_t {
    // struct sockaddr_in6:
    //    sa_family_t     sin6_family;
    //    in_port_t       sin6_port;
    //    uint32_t        sin6_flowinfo;
    //    struct in6_addr sin6_addr;
    //    (address structure: unsigned char s6_addr[16];)
    //    uint32_t        sin6_scope_id;
    // struct sockaddr_in:
    //    sa_family_t    sin_family;
    //    in_port_t      sin_port;
    //    struct in_addr sin_addr;
    //    (address structure: uint32_t s_addr;)

    // So, for this union, these are the same location:
    //    ip4.sin_family and ip6.sin6_family
    //    ip4.sin6_port and ip6.sin6_port

    struct sockaddr_in ip4;
    struct sockaddr_in6 ip6;
    struct sockaddr any_address;
};

typedef struct {
    union shared_network_sockaddr_t addr;
    int family;
    int sock_type;
    int protocol;
    socklen_t addr_len;
} shared_network_socketaddr_t;

#define IP_ADDRESS_STRING_LEN INET6_ADDRSTRLEN


// Optional command shared-address

#ifdef USES_SHARED_ADDRESS


    // Collects the current argument as the IP address into the
    // union_shared_sockaddr_t structure.
    // Pass the next command to run in global_arg2_i.
    // Don't forget to set the port number:
    //      shared_network_addr.ip4.sin_port = htos(port number);

#define ENUM_LIST__SHARED_ADDRESS
#define VIRTUAL_ENUM_LIST__SHARED_ADDRESS \
            /* from cmd_shared_network.h.in:81 */ \
            COMMAND_INDEX__SHARED_ADDRESS,
#define GLOBAL_VARDEF__SHARED_ADDRESS
#define INITIALIZE__SHARED_ADDRESS
#define STARTUP_CASE__SHARED_ADDRESS
#define RUN_CASE__SHARED_ADDRESS \
    case COMMAND_INDEX__SHARED_ADDRESS: \
        /* from cmd_shared_network.h.in:81 */ \
            /* from cmd_shared_network.h.in:83 */ \
            LOG(":: discovering ip address for "); \
            LOGLN(global_arg); \
            /* Assume an error until we get the value.*/ \
            global_err = 1; \
            global_cmd = COMMAND_INDEX__ERR; \
            shared_network_addrinfo = NULL; \
            tmp_val = getaddrinfo(global_arg, NULL, NULL, &shared_network_addrinfo); \
            if (tmp_val == 0 && shared_network_addrinfo != NULL) { \
                for ( \
                        shared_network_addrinfo_next = shared_network_addrinfo; \
                        shared_network_addrinfo_next != NULL; \
                        shared_network_addrinfo_next = shared_network_addrinfo_next->ai_next) { \
                    if (shared_network_addrinfo_next->ai_addrlen <= sizeof(union shared_network_sockaddr_t)) { \
                        memcpy(&shared_network_addr.addr.any_address, shared_network_addrinfo_next->ai_addr, shared_network_addrinfo_next->ai_addrlen); \
                        shared_network_addr.addr_len = shared_network_addrinfo_next->ai_addrlen; \
                        shared_network_addr.family = shared_network_addrinfo_next->ai_family; \
                        shared_network_addr.sock_type = shared_network_addrinfo_next->ai_socktype; \
                        shared_network_addr.protocol = shared_network_addrinfo_next->ai_protocol; \
                        /* Not 100% needed in all cases, but very useful for debugging.*/ \
                        if (shared_network_addr.family == AF_INET6) { \
                            inet_ntop(shared_network_addr.family, &shared_network_addr.addr.ip6.sin6_addr, shared_network_addr_str, IP_ADDRESS_STRING_LEN); \
                        } else if (shared_network_addr.family == AF_INET) { \
                            inet_ntop(shared_network_addr.family, &shared_network_addr.addr.ip4.sin_addr, shared_network_addr_str, IP_ADDRESS_STRING_LEN); \
                        } else { \
                            /* Keep looking*/ \
                            continue; \
                        } \
                        LOG(":: mapped to "); \
                        LOGLN(shared_network_addr_str); \
                        global_cmd = global_arg2_i; \
                        global_err = 0; \
                        break; \
                    } \
                } \
                freeaddrinfo(shared_network_addrinfo); \
            } \
        break;
#define REQUIRES_ADDL_ARG__SHARED_ADDRESS

#else /* USES_SHARED_ADDRESS */

#define ENUM_LIST__SHARED_ADDRESS
#define VIRTUAL_ENUM_LIST__SHARED_ADDRESS
#define GLOBAL_VARDEF__SHARED_ADDRESS
#define INITIALIZE__SHARED_ADDRESS
#define STARTUP_CASE__SHARED_ADDRESS
#define RUN_CASE__SHARED_ADDRESS
#define REQUIRES_ADDL_ARG__SHARED_ADDRESS
#endif /* USES_SHARED_ADDRESS */


// Optional command shared-service

#ifdef USES_SHARED_SERVICE

    // With global_arg_cached containing the address and global_arg
    //   containing the port number or service name, loads these values
    //   into the  union_shared_sockaddr_t structure.
    // Pass the next command to run in global_arg2_i.



#define ENUM_LIST__SHARED_SERVICE
#define VIRTUAL_ENUM_LIST__SHARED_SERVICE \
            /* from cmd_shared_network.h.in:135 */ \
            COMMAND_INDEX__SHARED_SERVICE,
#define GLOBAL_VARDEF__SHARED_SERVICE
#define INITIALIZE__SHARED_SERVICE
#define STARTUP_CASE__SHARED_SERVICE
#define RUN_CASE__SHARED_SERVICE \
    case COMMAND_INDEX__SHARED_SERVICE: \
        /* from cmd_shared_network.h.in:135 */ \
            /* from cmd_shared_network.h.in:136 */ \
            LOG(":: discovering service for address "); \
            LOG(global_arg_cached); \
            LOG(", service "); \
            LOGLN(global_arg); \
            /* Assume an error until we get the value.*/ \
            global_err = 1; \
            global_cmd = COMMAND_INDEX__ERR; \
            shared_network_addrinfo = NULL; \
            tmp_val = getaddrinfo(global_arg_cached, global_arg, NULL, &shared_network_addrinfo); \
            if (tmp_val == 0 && shared_network_addrinfo != NULL) { \
                for ( \
                        shared_network_addrinfo_next = shared_network_addrinfo; \
                        shared_network_addrinfo_next != NULL; \
                        shared_network_addrinfo_next = shared_network_addrinfo_next->ai_next) { \
                    if (shared_network_addrinfo_next->ai_addrlen <= sizeof(union shared_network_sockaddr_t)) { \
                        memcpy(&shared_network_addr.addr.any_address, shared_network_addrinfo_next->ai_addr, shared_network_addrinfo_next->ai_addrlen); \
                        shared_network_addr.addr_len = shared_network_addrinfo_next->ai_addrlen; \
                        shared_network_addr.family = shared_network_addrinfo_next->ai_family; \
                        shared_network_addr.sock_type = shared_network_addrinfo_next->ai_socktype; \
                        shared_network_addr.protocol = shared_network_addrinfo_next->ai_protocol; \
                        /* Not 100% needed in all cases, but very useful for debugging.*/ \
                        if (shared_network_addr.family == AF_INET6) { \
                            inet_ntop(shared_network_addr.family, &shared_network_addr.addr.ip6.sin6_addr, shared_network_addr_str, IP_ADDRESS_STRING_LEN); \
                            continue; \
                        } else if (shared_network_addr.family == AF_INET) { \
                            inet_ntop(shared_network_addr.family, &shared_network_addr.addr.ip4.sin_addr, shared_network_addr_str, IP_ADDRESS_STRING_LEN); \
                        } else { \
                            /* Keep looking*/ \
                            continue; \
                        } \
                        LOG(":: mapped to "); \
                        LOGLN(shared_network_addr_str); \
                        printf(":: -> %s %d\n", shared_network_addr_str, ntohs(shared_network_addr.addr.ip4.sin_port)); \
                        global_cmd = global_arg2_i; \
                        global_err = 0; \
                        break; \
                    } \
                } \
                freeaddrinfo(shared_network_addrinfo); \
            } \
        break;
#define REQUIRES_ADDL_ARG__SHARED_SERVICE

#else /* USES_SHARED_SERVICE */

#define ENUM_LIST__SHARED_SERVICE
#define VIRTUAL_ENUM_LIST__SHARED_SERVICE
#define GLOBAL_VARDEF__SHARED_SERVICE
#define INITIALIZE__SHARED_SERVICE
#define STARTUP_CASE__SHARED_SERVICE
#define RUN_CASE__SHARED_SERVICE
#define REQUIRES_ADDL_ARG__SHARED_SERVICE
#endif /* USES_SHARED_SERVICE */



#if defined(USES_SHARED_ADDRESS) || defined(USES_SHARED_SERVICE)





// Standard procedure for connecting to an IPv[4,6] socket.
// Returns the socket file descriptor, or, if an error, returns -1.
// Valid file descriptors must be explicitly closed (close(fd)) by the caller.
int shared_network_connect_address(
        shared_network_socketaddr_t *address,
        int timeout_seconds) {
    short int socket_fd;
    fd_set fdset;
    int error;
    struct timeval tv;
    socklen_t len;

    // Perform connection.
    socket_fd = socket(address->family, address->sock_type, address->protocol);
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    // Because the socket is in non-blocking mode,
    //   chances are that running connect will return an error with
    //   errno == EINPROGRESS, meaning connection is in-progress.
    connect(socket_fd, &address->addr.any_address, address->addr_len);
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
            printf("socket error %d\n", error);
            return -1;
        }
    }
    LOG(":: socket did not connect in time.\n");
    return -1;
}


#define ENUM_LIST__SHARED_NETWORK \
            ENUM_LIST__SHARED_ADDRESS \
            ENUM_LIST__SHARED_SERVICE
#define VIRTUAL_ENUM_LIST__SHARED_NETWORK \
            VIRTUAL_ENUM_LIST__SHARED_ADDRESS \
            VIRTUAL_ENUM_LIST__SHARED_SERVICE \
            /* from cmd_shared_network.h.in:192 */ \
            COMMAND_INDEX__DO_NOT_USE_SHARED_NETWORK,
#define GLOBAL_VARDEF__SHARED_NETWORK \
            GLOBAL_VARDEF__SHARED_ADDRESS \
            GLOBAL_VARDEF__SHARED_SERVICE
#define INITIALIZE__SHARED_NETWORK \
            INITIALIZE__SHARED_ADDRESS \
            INITIALIZE__SHARED_SERVICE \
            /* from cmd_shared_network.h.in:193 */ \
            shared_network_socketaddr_t shared_network_addr; \
            struct addrinfo *shared_network_addrinfo; \
            struct addrinfo *shared_network_addrinfo_next; \
            char shared_network_addr_str[IP_ADDRESS_STRING_LEN];
#define STARTUP_CASE__SHARED_NETWORK \
            STARTUP_CASE__SHARED_ADDRESS \
            STARTUP_CASE__SHARED_SERVICE
#define RUN_CASE__SHARED_NETWORK \
            RUN_CASE__SHARED_ADDRESS \
            RUN_CASE__SHARED_SERVICE
#define REQUIRES_ADDL_ARG__SHARED_NETWORK \
            REQUIRES_ADDL_ARG__SHARED_ADDRESS \
            REQUIRES_ADDL_ARG__SHARED_SERVICE

#else /* defined(USES_SHARED_ADDRESS) || defined(USES_SHARED_SERVICE) */

#define ENUM_LIST__SHARED_NETWORK
#define VIRTUAL_ENUM_LIST__SHARED_NETWORK
#define GLOBAL_VARDEF__SHARED_NETWORK
#define INITIALIZE__SHARED_NETWORK
#define STARTUP_CASE__SHARED_NETWORK
#define RUN_CASE__SHARED_NETWORK
#define REQUIRES_ADDL_ARG__SHARED_NETWORK
#endif /* defined(USES_SHARED_ADDRESS) || defined(USES_SHARED_SERVICE) */


#endif /* _FS_SHELL__CMD_SHARED_NETWORK_ */
