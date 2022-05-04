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

/*
 * A simple "shell" that handles only simple file operations.
 * Intended to be used with the manipulation of file-only docker images.
 * At last count, this file compiled is huge, at 894 kilobytes.
 *
 * It is intended to be used as a non-interactive shell replacement.
 * 
 * See "commands.c" for a list of supported commands.
 */
#include <stddef.h>
#include "args.h"
#include "commands.h"


int main(const int srcArgc, char *srcArgv[]) {
    int ret = setupTokenizer(srcArgc, srcArgv);
    if (!ret) {
        ret = runCommands();
    }
    ret += closeTokenizer();
    return ret;
}
