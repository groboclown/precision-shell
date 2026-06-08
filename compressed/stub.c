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

// A unified main function.
// The 'make' changes this up by replacing the implementation of the
// stub-run and stub-decompress files during the link stage.
// The payload file must contain the binary data that's compatible
// with the decompress function, and can be run by the run function.

#include <stdlib.h>
#include "stub-common.h"
#include "stub-decompress.h"
#include "stub-run.h"
#include "data-payload.h"


int main(int argc, char *argv[], char *envp[]) {
    unsigned char *dest = malloc(___presh_len);
    if (!dest) {
        WRITE_LAUNCH_ERROR();
        return 50;
    };
    if (DECOMPRESS_OK != decompress(dest, ___presh_len, ___presh_z, ___presh_z_len)) {
        WRITE_LAUNCH_ERROR();
        return 51;
    }

    return run_embedded(dest, ___presh_len, argc, argv, envp);
}
