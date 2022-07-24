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

// Runs the first argument as an executable,
// with "-c" as the first argument to the executable, then the third argument
// will be the contents of the file from the passed-in second argument.
// This avoids needing to perform complicated shell escaping for passing arguments.

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


static char *SECOND_ARG = "-c";


int main(const int argc, char *argv[]) {
    FILE *fin;
    long int file_size;
    char *arg;
    size_t count;
    char *exec_argv[] = { argv[1], SECOND_ARG, NULL, NULL };

    if (argc != 3) {
        printf("Usage: %s (cmd name) (argument file name)\n", argv[0]);
        return 1;
    }
    // printf("Opening file %s\n", argv[2]);
    fin = fopen(argv[2], "r");
    // printf("  ... seeking to end\n");
    fseek(fin, 0L, SEEK_END);
    // printf("  ... finding position\n");
    file_size = ftell(fin);
    // printf("  ... seeking back to start\n");
    fseek(fin, 0L, SEEK_SET);
    // printf("  ... reading %ld bytes\n", file_size);

    arg = malloc(sizeof(char) * file_size);
    if (arg == NULL) {
        printf("Failed to allocate %ld bytes of RAM\n", file_size);
        return 1;
    }
    count = fread((void *) arg, sizeof(char), file_size, fin);
    fclose(fin);
    if (count != file_size) {
        printf("Expected to read %ld bytes, but read %ld bytes.\n", file_size, count);
        return 1;
    }
    exec_argv[2] = arg;

    // printf("  ... running %s %s %s\n", exec_argv[0], exec_argv[1], exec_argv[2]);

    execvp(exec_argv[0], (char * const*) exec_argv);

    // unreachable code
    return 1;
}
