/*
MIT License

Copyright (c) 2025 groboclown

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


#include <stdlib.h>
#include <unistd.h>
#include "tinf.h"
#include "presh-comp.bin.h"
#include "load-run.h"


int main(int argc, char *argv[], char *envp[]) {
    int ret;

    unsigned int destSize = ___presh_len;
    unsigned char *dest = malloc(destSize);
    if (!dest) {
        write(STDERR_FILENO, RUN_ERROR, RUN_ERROR_LEN);
        return 16;
    }
    ret = tinf_uncompress(dest, &destSize, ___presh_comp, ___presh_comp_len);
    if (ret < 0) {
        write(STDERR_FILENO, RUN_ERROR, RUN_ERROR_LEN);
        return 17;
    }
    if (ret != TINF_OK) {
        write(STDERR_FILENO, RUN_ERROR, RUN_ERROR_LEN);
        return 18;
    }

    // Run the embedded ELF loader with the provided data.
    return run_embedded(dest);
}
