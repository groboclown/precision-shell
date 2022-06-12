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

#ifndef _FS_SHELL__CMD_DUP_

#include "uses.h"


#ifdef USE_CMD_DUP_A
// cmd_dup_a_setup sets up the dup command
int cmd_dup_a_setup();
#endif /* USE_CMD_DUP_A */

#ifdef USE_CMD_DUP_W
// cmd_dup_w_setup sets up the dup command
int cmd_dup_w_setup();
#endif /* USE_CMD_DUP_W */

#ifdef USE_CMD_DUP_R
// cmd_dup_r_setup sets up the dup command
int cmd_dup_r_setup();
#endif /* USE_CMD_DUP_R */


#ifdef USES_DUP

// run the dup command
int cmd_dup_run();

#endif /* USES_DUP */

#endif /* _FS_SHELL__CMD_DUP_ */
