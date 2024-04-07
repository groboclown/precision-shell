// GENERATED FROM cmd_shared_connect.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_SHARED_CONNECT_
#define _FS_SHELL__CMD_SHARED_CONNECT_

// Optional command shared-connect

#ifdef USES_SHARED_CONNECT


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

    // This captures 2 string arguments.
    //    The first is put into shared_connect_address
    //    The second into global_arg_cached
    //    Then the next argument is run in OnArg from the command index in global_arg3_i.


// Standard procedure for connecting to an IPv[4,6] socket.
// Returns the socket file descriptor, or, if an error, returns -1.
// Valid file descriptors must be explicitly closed (close(fd)) by the caller.
int shared_connect_to_address(
        const char *address, const char *service, int timeout_seconds,
        int protocol) {
    struct addrinfo *addrinfo_next;
    fd_set fdset;
    int error;
    int option;
    struct timeval tv;
    socklen_t len;
    short int socket_fd = -1;
    struct addrinfo *root_addrinfo = NULL;

    error = getaddrinfo(address, service, NULL, &root_addrinfo);
    if (error == 0 && root_addrinfo != NULL) {
        for (
                addrinfo_next = root_addrinfo;
                addrinfo_next != NULL;
                addrinfo_next = addrinfo_next->ai_next
        ) {

            if (protocol != 0 && protocol != addrinfo_next->ai_protocol) {
                continue;
            }


            // attempt the connection.
#ifdef DEBUG
            char converted[INET6_ADDRSTRLEN];
            if (addrinfo_next->ai_family == AF_INET6) {
                fprintf(stdout, ":: Attempting connection to IPv6 address %s port %d family %d socktype %d protocol %d\n",
                    inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)addrinfo_next->ai_addr)->sin6_addr), converted, INET6_ADDRSTRLEN),
                    ntohs(((struct sockaddr_in6 *)addrinfo_next->ai_addr)->sin6_port),
                    addrinfo_next->ai_family, addrinfo_next->ai_socktype, addrinfo_next->ai_protocol);
            } else if (addrinfo_next->ai_family == AF_INET) {
                fprintf(stdout, ":: Attempting connection to IPv4 address %s port %d\n",
                    inet_ntop(AF_INET, &(((struct sockaddr_in *)addrinfo_next->ai_addr)->sin_addr), converted, INET6_ADDRSTRLEN),
                    ntohs(((struct sockaddr_in *)addrinfo_next->ai_addr)->sin_port));
            }
            fflush(stdout);
#endif

            // Perform connection.
            socket_fd = socket(addrinfo_next->ai_family, addrinfo_next->ai_socktype, addrinfo_next->ai_protocol);
            if (socket_fd == -1) {
                LOG(":: failed to open socket\n");
#ifdef DEBUG
                fprintf(stdout, ":: - error %d\n", errno);
                fflush(stdout);
#endif
                // No need to close the socket.
                // Loop again.
                continue;
            }
            error = fcntl(socket_fd, F_SETFL, O_NONBLOCK);
            if (error == -1) {
                LOG(":: failed to set socket to non-blocking mode\n");
#ifdef DEBUG
                fprintf(stdout, ":: - error %d\n", errno);
                fflush(stdout);
#endif
                // Close the socket and loop again.
            } else {
                // Because the socket is in non-blocking mode,
                //   chances are that running connect will return an error with
                //   errno == EINPROGRESS (115), meaning connection is in-progress.
                //   Therefore, ignore this specific error.
                error = connect(socket_fd, addrinfo_next->ai_addr, addrinfo_next->ai_addrlen);
                if (error == -1 && errno != EINPROGRESS) {
                    LOG(":: socket connect error\n");
#ifdef DEBUG
                    fprintf(stdout, ":: - error %d\n", errno);
                    fflush(stdout);
#endif
                    // Close the socket and loop again.
                } else {
                    FD_ZERO(&fdset);
                    FD_SET(socket_fd, &fdset);
                    tv.tv_sec = timeout_seconds;
                    tv.tv_usec = 0;

                    // Default to not being open
                    if (select(socket_fd + 1, NULL, &fdset, NULL, &tv) == 1) {
                        len = sizeof(option);
                        error = getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &option, &len);
                        if (error == -1) {
                            LOG(":: socket option error\n");
#ifdef DEBUG
                            fprintf(stdout, ":: - getsockopt error %d\n", errno);
                            fflush(stdout);
#endif
                            // Close the socket and don't loop again.
                        } else if (option == 0) {
                            LOG(":: connection open\n");
                            // Trun the socket back to blocking mode.
                            fcntl(socket_fd, F_SETFL, 0);

                            // Exit the loop and do cleanup there.
                            // ... but don't close the socket.
                            break;
                        } else {
                            LOG(":: socket has error\n");
#ifdef DEBUG
                            fprintf(stdout, ":: - socket error %d\n", option);
                            fflush(stdout);
#endif
                            // Close the socket and try again.
                        }
                    }
                }
            }
            // Loop again
            LOG(":: (if no error, then socket did not connect in time)\n");
            // ... Note: skipping error checking on 'close'.
            close(socket_fd);
            socket_fd = -1;
        }
        // Clean up.
        freeaddrinfo(root_addrinfo);
    }
    return socket_fd;
}


#define ENUM_LIST__SHARED_CONNECT
#define VIRTUAL_ENUM_LIST__SHARED_CONNECT \
            /* from cmd_shared_connect.h.in:41 */ \
            COMMAND_INDEX__SHARED_CONNECT__ADDRESS,
#define GLOBAL_VARDEF__SHARED_CONNECT
#define INITIALIZE__SHARED_CONNECT \
            /* from cmd_shared_connect.h.in:42 */ \
            const char *shared_connect_address = NULL;
#define STARTUP_CASE__SHARED_CONNECT
#define RUN_CASE__SHARED_CONNECT \
    case COMMAND_INDEX__SHARED_CONNECT__ADDRESS: \
        /* from cmd_shared_connect.h.in:41 */ \
            /* from cmd_shared_connect.h.in:46 */ \
            shared_connect_address = global_arg; \
            global_cmd = COMMAND_INDEX__SHARED_STR; \
        break;
#define REQUIRES_ADDL_ARG__SHARED_CONNECT

#else /* USES_SHARED_CONNECT */

#define ENUM_LIST__SHARED_CONNECT
#define VIRTUAL_ENUM_LIST__SHARED_CONNECT
#define GLOBAL_VARDEF__SHARED_CONNECT
#define INITIALIZE__SHARED_CONNECT
#define STARTUP_CASE__SHARED_CONNECT
#define RUN_CASE__SHARED_CONNECT
#define REQUIRES_ADDL_ARG__SHARED_CONNECT
#endif /* USES_SHARED_CONNECT */

#endif /* _FS_SHELL__CMD_SHARED_CONNECT_ */
