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
#ifndef FS_SHELL_OUTPUT
#define FS_SHELL_OUTPUT

#include <stdio.h>
#include <unistd.h>
#include <string.h>

// We explicitly don't care about the return code from writing to stdout or stderr.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

#define stdoutP(text) write(STDOUT_FILENO, (text), strlen(text))
#define stdoutPLn(text) write(STDOUT_FILENO, (text), strlen(text)); write(STDOUT_FILENO, "\n", 1);
#define stderrP(text) write(STDERR_FILENO, (text), strlen(text))
#define stderrPLn(text) write(STDERR_FILENO, (text), strlen(text)); write(STDERR_FILENO, "\n", 1);

#ifdef DEBUG
#define LOG(text) stdoutP(text)
#define LOG1(text) write(STDOUT_FILENO, (text), 1)
#define LOGLN(text) stdoutPLn(text)
#else
#define LOG(text)
#define LOG1(text)
#define LOGLN(text)
#endif

#endif
