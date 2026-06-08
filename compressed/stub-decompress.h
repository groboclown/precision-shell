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

// Standard interface for the stubs to perform decompression.

#ifndef STUB_DECOMPRESS_H
#define STUB_DECOMPRESS_H

#define DECOMPRESS_OK 0
#define DECOMPRESS_ERR 1

/**
 * Decompress `sourceLen` bytes of compressed data from `source` into `dest`.
 *
 * The variable `destLen` points to must contain the size of `dest` on entry,
 * and must equal to the final uncompressed size.  If this is not the case,
 * then the function returns an error.
 *
 * Reads at most `sourceLen` bytes from `source`.
 * Writes exactly `destLen` bytes to `dest`.
 *
 * @param dest pointer to where to place decompressed data
 * @param destLen pointer to variable containing size of `dest`
 * @param source pointer to compressed data
 * @param sourceLen size of compressed data
 * @return `DECOMPRESS_OK` on success, and anything else means error.
 */
int decompress(void *dest, unsigned int destLen,
                           const void *source, unsigned int sourceLen);


#endif // STUB_DECOMPRESS_H
