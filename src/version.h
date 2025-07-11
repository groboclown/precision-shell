/*
MIT License

Copyright (c) 2022,2024 groboclown

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
#ifndef _FS_SHELL_VERSION_
#define _FS_SHELL_VERSION_

#ifdef DEBUG
#define _VERSION_DEBUG " +debug"
#else
#define _VERSION_DEBUG ""
#endif

#ifdef USE_STREAMING_INPUT
#define _VERSION_INPUT " +input"
#else
#define _VERSION_INPUT ""
#endif

#ifdef USE_ENVIRONMENT_INPUT
#define _VERSION_ENVIRON " +enviro"
#else
#define _VERSION_ENVIRON ""
#endif

#ifdef REQUIRE_FULL_CMD
#define _VERSION_REQARGS " +reqargs"
#else
#define _VERSION_REQARGS ""
#endif

static const char VERSION_STR[] =
    " " VERSION_NUMBER _VERSION_INPUT _VERSION_ENVIRON _VERSION_REQARGS _VERSION_DEBUG;

#endif /* _FS_SHELL_VERSION_ */
