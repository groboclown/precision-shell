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

#include <stdlib.h>
#include <errno.h>
#include "globals.h"
#include "helpers.h"

const char empty_string[] = EMPTY_STRING;


// helper_arg_to_uint Converts the arg to an integer value
//   base is the integer base (8 10 or 16)
//   maxValue is the maximum value allowed.
//   implied minimum value is 0.
//   returns -1 on error.
int helper_arg_to_uint(const char *arg, int base, int max_value) {
    char *end_ptr;
    errno = 0;
    unsigned long ret = strtoul(arg, &end_ptr, base);
    if (
            // nothing was parsed.  This can happen while end_ptr[0] == 0 if the argument
            //   has length 0.
            arg == end_ptr

            // the argument had non-numeric stuff after the first numbers.
            || end_ptr[0] != 0

            // Bad number parsing, usually a value of of range problem.
            || errno != 0

            // Easy max value checking, which is a requirement for all our stuff.
            //   It also helps with unsigned long -> int conversion restrictions.
            || ret > max_value

            // || ret < 0, but we're reading unsigned, so it can't ever happen.
    ) {
        return -1;
    }
    return ret;
}
