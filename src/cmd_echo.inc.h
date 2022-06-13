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

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EECHOPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__CMD_ECHO_

// No startup execution
#define STARTUP__COMMAND_INDEX__ECHO


#ifdef USE_CMD_ECHO

#define CASE__COMMAND_INDEX__ECHO \
case COMMAND_INDEX__ECHO: \
    LOG(":: echo "); \
    LOGLN(global_arg); \
    stdoutPLn(global_arg); \
    break;


#else /* USE_CMD_ECHO */

#define CASE__COMMAND_INDEX__ECHO


#endif /* USE_CMD_ECHO */
#endif /* _FS_SHELL__CMD_ECHO_ */
