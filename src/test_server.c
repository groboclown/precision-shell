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

// A IPv4 & IPv6 server.
// Behavior:
//   - Single threaded, single process.
//     Therefore, should never be used in a production environment.
//   - Reports to STDOUT the text "Connected\n" when a new connection is accepted.
//   - Reports to STDOUT the text "Disconnected\n" when a connection closes.
//   - Supports different client handling implementations.
//     - noop: reads in client data, but ignores it all.
//     - echo: sends back the data the client sends.
//     - http1: the first time the client sends "\r\n\r\n", the file input
//       is sent back to the client.
//   - Quits execution when the final character sent to the server from a client
//     connection is 'Q'.

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_FILE_SIZE (10 * 1024)


void print_out(const char *text) {
    write(STDOUT_FILENO, (text), strlen(text));
    fsync(STDOUT_FILENO);
}


void print_err(const char *text) {
    write(STDERR_FILENO, (text), strlen(text));
}

void signal_callback_handler(int signum) {
    // Ignore the signal.
}


struct file_data {
    char *data;
    size_t size;
};

// handler_data Data sent to the connection handlers.
//   handlers can force a disconnect by setting
//      "force_disconnect" to non-zero.
struct handler_data {
    // Managed by initialization.
    struct file_data input_file;

    // Managed by server.  Set once per connection.
    int client_sock;

    // Managed by server.  Set when data read.
    int client_read;

    // Managed by handler.  Initialized to 0 at client connection time.
    int force_disconnect;
    int handler_state;
};
const char hex_digits[] = "0123456789abcdef";


/* Reads the not-encoded data from the file.
  This allocates the data in the structure.  Caller must free that memory.
  Returns 0 on OK.  If it returns 0, then the caller must free the memory,
  otherwise the memory is already freed.
*/
int read_file(const char *filename, struct file_data *file_data) {
    size_t count;
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        print_err("Failed opening file ");
        print_err(filename);
        print_err("\n");
        return 1;
    }
    file_data->size = lseek(fd, 0, SEEK_END);
    if (file_data->size > MAX_FILE_SIZE) {
        close(fd);
        print_err("File too big to use.\n");
        return 1;
    }
    file_data->data = malloc(file_data->size * sizeof(char));
    if (file_data->data == NULL) {
        close(fd);
        print_err("Failed to allocate memory.");
        return 1;
    }
    lseek(fd, 0, SEEK_SET);
    count = read(fd, file_data->data, file_data->size);
    close(fd);
    if (count == -1) {
        free(file_data->data);
        file_data->data = NULL;
        print_err("Failed to read file ");
        print_err(filename);
        print_err("\n");
        return 1;
    }

    return 0;
}



void handle_noop(struct handler_data *data) {
    return;
}


void handle_echo(struct handler_data *data) {
    const char buf[] = { data->client_read };
    write(STDERR_FILENO, buf, 1);
    fsync(STDERR_FILENO);
    write(data->client_sock, buf, 1);
    fsync(data->client_sock);
}


void handle_http1(struct handler_data *data) {
    // Ignores body.
    const char buf[] = { data->client_read };
    write(STDERR_FILENO, buf, 1);
    fsync(STDERR_FILENO);

    switch (data->handler_state) {
        case 0:
            if (data->client_read == '\r') {
                // write(STDERR_FILENO, "-1", 2);
                data->handler_state = 1;
            }
            break;
        case 1:
            if (data->client_read == '\n') {
                // write(STDERR_FILENO, "-2", 2);
                data->handler_state = 2;
            } else {
                data->handler_state = 0;
            }
            break;
        case 2:
            if (data->client_read == '\r') {
                // write(STDERR_FILENO, "-3", 2);
                data->handler_state = 3;
            } else {
                data->handler_state = 0;
            }
            break;
        case 3:
            if (data->client_read == '\n') {
                // write(STDERR_FILENO, "-4", 2);
                if (data->input_file.data != NULL) {
                    // print_err("Transmitting [");
                    // write(STDERR_FILENO, data->input_file.data, data->input_file.size);
                    // print_err("]\n");
                    // fsync(STDERR_FILENO);
                    write(data->client_sock, data->input_file.data, data->input_file.size);
                    fsync(data->client_sock);
                } else {
                    // print_err("No data to send back to the client.\n");
                }
                data->handler_state = 4;
                data->force_disconnect = 1;
            } else {
                data->handler_state = 0;
            }
            break;
        // default: keep handler_state the same,
        //  which means don't double-send the response data.
    }
}


int main(int argc, const char *argv[]) {
    int port = 29446;
    int server_sock;
    socklen_t client_size;
    int on;
    int read_size;
    int total_read_count;
    struct sockaddr_in6 server, client;
    char client_data[1];
    char str_int[16];
    char last_read = '\0';
    struct handler_data handler_state;
    void (*handler_func)(struct handler_data *) = NULL;

    if (argc < 3) {
        print_err("Usage: " );
        print_err(argv[0]);
        print_err(" (mode) (port number) (response data file)\n\n");
        print_err("Mode: one of 'noop', 'echo', 'http1'.\n");
        return 1;
    }

    port = atoi(argv[2]);
    if (port <= 0) {
        print_err("Invalid port number.\n");
        return 1;
    }
    if (strcmp("noop", argv[1]) == 0) {
        print_err("Running as noop server\n");
        handler_func = &handle_noop;
    } else if (strcmp("echo", argv[1]) == 0) {
        print_err("Running as echo server\n");
        handler_func = &handle_echo;
    } else if (strcmp("http1", argv[1]) == 0) {
        handler_func = &handle_http1;
        if (argc >= 4) {
            if (read_file(argv[3], &handler_state.input_file)) {
                // already reported the errror.
                return 1;
            }
        } else {
            print_err("http1 requires an input file argument.\n");
            return 1;
        }
        print_err("Running as http/1.1 server\n");
    } else if (strcmp("http2", argv[1]) == 0) {
        print_err("http2 not implemented yet.\n");
        return 1;
    } else {
        print_err("Unsupported server mode '");
        print_err(argv[1]);
        print_err("'.\n");
        return 1;
    }

    // Prevent "SIGPIPE" (write to a closed FD) from killing our
    //   process.  This can happen due to any number of reasons.
    signal(SIGPIPE, signal_callback_handler);
    
    // Create the server socket.
    server_sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (server_sock < 0) {
        print_err("Could not create socket\n");
        return 1;
    }
    // Allow the local address to be reused when the server is restarted
    // before the required wait time expires.
    on = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        print_err("setsocket(SO_REUSEADDR) failed\n");
        close(server_sock);
        return 1;
    }

    // Listen to incoming requests.
    memset(&server, 0, sizeof(server));
    server.sin6_family = AF_INET6;
    server.sin6_addr = in6addr_any;
    server.sin6_port = htons(port);

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        print_err("Bind failed.\n");
        close(server_sock);
        return 1;
    }

    listen(server_sock, 3);

    // For each connection...
    while (last_read != 'Q') {
        client_size = sizeof(client);
        handler_state.client_sock = accept(server_sock, (struct sockaddr *)&client, &client_size);
        if (handler_state.client_sock < 0) {
            // This error message can end up meaning that something else is
            // using the server.  Rather than continuing to retry accepting a socket,
            // just exit.
            print_err("Accept failed.\n");
            break;
        }
        print_out("Connected\n");

        // Read data from the client, write it out to stdout and send it back.
        //  One.  Byte.  At.  A.  Time.
        handler_state.handler_state = 0;
        handler_state.force_disconnect = 0;
        total_read_count = 0;
        while (
                handler_state.force_disconnect == 0
                && (read_size = recv(handler_state.client_sock, client_data, 1, 0)) > 0
        ) {
            total_read_count++;
            handler_state.client_read = client_data[0];
            (*handler_func)(&handler_state);
            last_read = handler_state.client_read;
        }
        itoa(total_read_count, str_int, 10);
        print_out("Disconnected; read ");
        print_out(str_int);
        print_out(" bytes.\n");
        close(handler_state.client_sock);
    }
    close(server_sock);
    if (handler_state.input_file.data != NULL) {
        free(handler_state.input_file.data);
    }
}
