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

// A simple, simple report-on-connect IPv4 & IPv6 server.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    unsigned short port = 29446;
    int server_sock, client_sock;
    socklen_t client_size;
    int on;
    int read_size;
    struct sockaddr_in6 server, client;
    char client_data[1];
    char last_read = '\0';

    if (argc >= 2) {
        port = atoi(argv[1]);
    }
    
    // Create the server socket.
    server_sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (server_sock < 0) {
        printf("Could not create socket\n");
        return 1;
    }
    // Allow the local address to be reused when the server is restarted
    // before the required wait time expires.
    on = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        printf("setsocket(SO_REUSEADDR) failed\n");
        close(server_sock);
        return 1;
    }

    // Listen to incoming requests.
    memset(&server, 0, sizeof(server));
    server.sin6_family = AF_INET6;
    server.sin6_addr = in6addr_any;
    server.sin6_port = htons(port);

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Bind failed.\n");
        close(server_sock);
        return 1;
    }

    listen(server_sock, 3);

    // For each connection...
    while (last_read != 'Q') {
        client_size = sizeof(client);
        client_sock = accept(server_sock, (struct sockaddr *)&client, &client_size);
        if (client_sock < 0) {
            printf("Accept failed.\n");
            continue;
        }
        printf("Connected\n");

        // Read data from the client, write it out to stdout and send it back.
        //  One.  Byte.  At.  A.  Time.
        while ((read_size = recv(client_sock, client_data, 1, 0)) > 0) {
            last_read = client_data[0];
            write(STDERR_FILENO, client_data, read_size);
            fsync(STDERR_FILENO);
            write(client_sock, client_data, read_size);
            fsync(client_sock);
        }
        close(client_sock);
    }
    close(server_sock);
}
