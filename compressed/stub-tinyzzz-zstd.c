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

// Interface between the stub-decompress and TinyZZZ lzma decoder.

#include <stddef.h>   // size_t
#include <stdint.h>   // uint8_t, uint16_t, uint32_t

#include "stub-decompress.h"
#include "tinyzzz-zstd/zstdD.h"

int decompress(void *dest, unsigned int destLen,
               const void *source, unsigned int sourceLen) {
    // return 0 on okay, number on error.

    // Formally, it should look like this;
    // unsigned int actualDestLen = destLen;
    //if (lzmaD(source, sourceLen, dest, &destLen)) != 0) {
    //    return 1;
    //}
    //return destLen != actualDestLen;

    // This isn't good.  The library currently does not return error codes,
    // but instead just calls exit(1) on error.  This will require a bit
    // of changing.
    zstdD((uint8_t*) source, sourceLen, dest, (size_t*) &destLen);
    return 0;
}
