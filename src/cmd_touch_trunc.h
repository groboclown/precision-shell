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

#ifndef _FS_SHELL__CMD_TOUCH_TRUNC_

#include "uses.h"

#if defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC)

#include "command_def.h"
#include "command_common.h"


int cmd_touch_trunc_run();

#ifdef USE_CMD_TOUCH

extern const char command_name_touch[];
int cmd_touch_setup(int);

#define ENUM__CMD_TOUCH        COMMAND_INDEX__TOUCH
#define NAME__CMD_TOUCH        "touch"
#define NAMEVAR__CMD_TOUCH     command_name_touch
#define SETUP__CMD_TOUCH       cmd_touch_setup
#define RUN__CMD_TOUCH         cmd_touch_trunc_run


// Extrapolated defines
#define ENUM_LIST__CMD_TOUCH   ENUM__CMD_TOUCH,
#define NAME_TC__CMD_TOUCH     NAME__CMD_TOUCH,
#define NAME_VC__CMD_TOUCH     NAMEVAR__CMD_TOUCH,
#define NAME_VS__CMD_TOUCH     names[COMMAND_INDEX__TOUCH] = NAMEVAR__CMD_TOUCH;
#define SETUP_C__CMD_TOUCH     &SETUP__CMD_TOUCH,
#define SETUP_S__CMD_TOUCH     setups[COMMAND_INDEX__TOUCH] = &SETUP__CMD_TOUCH;
#define RUN_C__CMD_TOUCH       &RUN__CMD_TOUCH,
#define RUN_S__CMD_TOUCH       runs[COMMAND_INDEX__TOUCH] = &RUN__CMD_TOUCH;

#else /* USE_CMD_TOUCH */
#define ENUM_LIST__CMD_TOUCH
#define NAME_TC__CMD_TOUCH
#define NAME_VC__CMD_TOUCH
#define NAME_VS__CMD_TOUCH
#define SETUP_C__CMD_TOUCH
#define SETUP_S__CMD_TOUCH
#define RUN_C__CMD_TOUCH
#define RUN_S__CMD_TOUCH
#endif /* USE_CMD_TOUCH */



#ifdef USE_CMD_TRUNC

extern const char command_name_trunc[];
int cmd_trunc_setup(int);

#define ENUM__CMD_TRUNC        COMMAND_INDEX__TRUNC
#define NAME__CMD_TRUNC        "trunc"
#define NAMEVAR__CMD_TRUNC     command_name_trunc
#define SETUP__CMD_TRUNC       cmd_trunc_setup
#define RUN__CMD_TRUNC         cmd_touch_trunc_run


// Extrapolated defines
#define ENUM_LIST__CMD_TRUNC   ENUM__CMD_TRUNC,
#define NAME_TC__CMD_TRUNC     NAME__CMD_TRUNC,
#define NAME_VC__CMD_TRUNC     NAMEVAR__CMD_TRUNC,
#define NAME_VS__CMD_TRUNC     names[COMMAND_INDEX_TRUNC] = NAMEVAR__CMD_TRUNC;
#define SETUP_C__CMD_TRUNC     &SETUP__CMD_TRUNC,
#define SETUP_S__CMD_TRUNC     setups[COMMAND_INDEX__TRUNC] = &SETUP__CMD_TRUNC;
#define RUN_C__CMD_TRUNC       &RUN__CMD_TRUNC,
#define RUN_S__CMD_TRUNC       runs[COMMAND_INDEX__TRUNC] = &RUN__CMD_TRUNC;

#else /* USE_CMD_TRUNC */
#define ENUM_LIST__CMD_TRUNC
#define NAME_TC__CMD_TRUNC
#define NAME_VC__CMD_TRUNC
#define NAME_VS__CMD_TRUNC
#define SETUP_C__CMD_TRUNC
#define SETUP_S__CMD_TRUNC
#define RUN_C__CMD_TRUNC
#define RUN_S__CMD_TRUNC
#endif /* USE_CMD_TRUNC */




// Extrapolated defines
#define ENUM_LIST__CMD_TOUCH_TRUNC   ENUM_LIST__CMD_TOUCH \
                                     ENUM_LIST__CMD_TRUNC
#define NAME_TC__CMD_TOUCH_TRUNC     NAME_TC__CMD_TOUCH \
                                     NAME_TC__CMD_TRUNC
#define NAME_VC__CMD_TOUCH_TRUNC     NAME_VC__CMD_TOUCH \
                                     NAME_VC__CMD_TRUNC
#define NAME_VS__CMD_TOUCH_TRUNC     NAME_VS__CMD_TOUCH \
                                     NAME_VS__CMD_TRUNC
#define SETUP_C__CMD_TOUCH_TRUNC     SETUP_C__CMD_TOUCH \
                                     SETUP_C__CMD_TRUNC
#define SETUP_S__CMD_TOUCH_TRUNC     SETUP_S__CMD_TOUCH \
                                     SETUP_S__CMD_TRUNC
#define RUN_C__CMD_TOUCH_TRUNC       RUN_C__CMD_TOUCH \
                                     RUN_C__CMD_TRUNC
#define RUN_S__CMD_TOUCH_TRUNC       RUN_S__CMD_TOUCH \
                                     RUN_S__CMD_TRUNC
#define CALL_INIT__CMD_TOUCH_TRUNC




#else /* defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC) */
#define ENUM_LIST__CMD_TOUCH_TRUNC
#define NAME_TC__CMD_TOUCH_TRUNC
#define NAME_VC__CMD_TOUCH_TRUNC
#define NAME_VS__CMD_TOUCH_TRUNC
#define SETUP_C__CMD_TOUCH_TRUNC
#define SETUP_S__CMD_TOUCH_TRUNC
#define RUN_C__CMD_TOUCH_TRUNC
#define RUN_S__CMD_TOUCH_TRUNC
#define CALL_INIT__CMD_TOUCH_TRUNC
#endif /* defined(USE_CMD_TOUCH) || defined(USE_CMD_TRUNC) */
#endif /* _FS_SHELL__CMD_TOUCH_TRUNC_ */
