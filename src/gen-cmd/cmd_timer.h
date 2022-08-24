// GENERATED FROM cmd_timer.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_TIMER_FUNCS_
#define _FS_SHELL__CMD_TIMER_FUNCS_


#include <time.h>
#include "output.h"


// Optional command start-timer

#ifdef USE_CMD_START_TIMER



/* from cmd_timer.h.in:32 */
extern const char cmd_name_start_timer[];
#define ENUM_LIST__START_TIMER \
            /* from cmd_timer.h.in:32 */ \
            COMMAND_INDEX__START_TIMER,
#define VIRTUAL_ENUM_LIST__START_TIMER
#define GLOBAL_VARDEF__START_TIMER \
            /* from cmd_timer.h.in:32 */ \
            const char cmd_name_start_timer[] = "start-timer";
#define INITIALIZE__START_TIMER \
            /* from cmd_timer.h.in:32 */ \
            command_list_names[COMMAND_INDEX__START_TIMER] = cmd_name_start_timer;
#define STARTUP_CASE__START_TIMER \
    case COMMAND_INDEX__START_TIMER: \
        /* from cmd_timer.h.in:32 */ \
            /* from cmd_timer.h.in:33 */ \
            LOG(":: start global timer\n"); \
            timer_val = time(0); \
            global_cmd = COMMAND_INDEX__ERR; \
        break;
#define RUN_CASE__START_TIMER
#define REQUIRES_ADDL_ARG__START_TIMER

#else /* USE_CMD_START_TIMER */

#define ENUM_LIST__START_TIMER
#define VIRTUAL_ENUM_LIST__START_TIMER
#define GLOBAL_VARDEF__START_TIMER
#define INITIALIZE__START_TIMER
#define STARTUP_CASE__START_TIMER
#define RUN_CASE__START_TIMER
#define REQUIRES_ADDL_ARG__START_TIMER
#endif /* USE_CMD_START_TIMER */


// Optional command elapsed-time

#ifdef USE_CMD_ELAPSED_TIME



/* from cmd_timer.h.in:45 */
extern const char cmd_name_elapsed_time[];
#define ENUM_LIST__ELAPSED_TIME \
            /* from cmd_timer.h.in:45 */ \
            COMMAND_INDEX__ELAPSED_TIME,
#define VIRTUAL_ENUM_LIST__ELAPSED_TIME
#define GLOBAL_VARDEF__ELAPSED_TIME \
            /* from cmd_timer.h.in:45 */ \
            const char cmd_name_elapsed_time[] = "elapsed-time";
#define INITIALIZE__ELAPSED_TIME \
            /* from cmd_timer.h.in:45 */ \
            command_list_names[COMMAND_INDEX__ELAPSED_TIME] = cmd_name_elapsed_time;
#define STARTUP_CASE__ELAPSED_TIME \
    case COMMAND_INDEX__ELAPSED_TIME: \
        /* from cmd_timer.h.in:45 */ \
            /* from cmd_timer.h.in:46 */ \
            LOG(":: report elapsed time since timer start\n"); \
            /* difftime introduces a floating point dependency, which adds bloat.*/ \
            /* and all we need is just a simple subtraction.*/ \
            /* stdoutPLn(shared_itoa(difftime(time(0), timer_val), global_itoa));*/ \
            stdoutPLn(shared_itoa(time(0) - timer_val, global_itoa)); \
            global_cmd = COMMAND_INDEX__ERR; \
        break;
#define RUN_CASE__ELAPSED_TIME
#define REQUIRES_ADDL_ARG__ELAPSED_TIME

#else /* USE_CMD_ELAPSED_TIME */

#define ENUM_LIST__ELAPSED_TIME
#define VIRTUAL_ENUM_LIST__ELAPSED_TIME
#define GLOBAL_VARDEF__ELAPSED_TIME
#define INITIALIZE__ELAPSED_TIME
#define STARTUP_CASE__ELAPSED_TIME
#define RUN_CASE__ELAPSED_TIME
#define REQUIRES_ADDL_ARG__ELAPSED_TIME
#endif /* USE_CMD_ELAPSED_TIME */


// Optional command export-elapsed-time

#ifdef USE_CMD_EXPORT_ELAPSED_TIME



/* from cmd_timer.h.in:61 */
extern const char cmd_name_export_elapsed_time[];
#define ENUM_LIST__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:61 */ \
            COMMAND_INDEX__EXPORT_ELAPSED_TIME,
#define VIRTUAL_ENUM_LIST__EXPORT_ELAPSED_TIME
#define GLOBAL_VARDEF__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:61 */ \
            const char cmd_name_export_elapsed_time[] = "export-elapsed-time";
#define INITIALIZE__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:61 */ \
            command_list_names[COMMAND_INDEX__EXPORT_ELAPSED_TIME] = cmd_name_export_elapsed_time;
#define STARTUP_CASE__EXPORT_ELAPSED_TIME
#define RUN_CASE__EXPORT_ELAPSED_TIME \
    case COMMAND_INDEX__EXPORT_ELAPSED_TIME: \
        /* from cmd_timer.h.in:61 */ \
            /* from cmd_timer.h.in:62 */ \
            LOG(":: storing elapsed time "); \
            /* difftime introduces a floating point dependency, which adds bloat.*/ \
            /* and all we need is just a simple subtraction.*/ \
            /* global_itoa_ptr = shared_itoa(difftime(time(0), timer_val), global_itoa);*/ \
            global_itoa_ptr = shared_itoa(time(0) - timer_val, global_itoa); \
            LOG(global_itoa_ptr); \
            LOG(" into env variable "); \
            LOGLN(global_arg); \
            global_err = setenv(global_arg, global_itoa_ptr, 1); \
        break;
#define REQUIRES_ADDL_ARG__EXPORT_ELAPSED_TIME

#else /* USE_CMD_EXPORT_ELAPSED_TIME */

#define ENUM_LIST__EXPORT_ELAPSED_TIME
#define VIRTUAL_ENUM_LIST__EXPORT_ELAPSED_TIME
#define GLOBAL_VARDEF__EXPORT_ELAPSED_TIME
#define INITIALIZE__EXPORT_ELAPSED_TIME
#define STARTUP_CASE__EXPORT_ELAPSED_TIME
#define RUN_CASE__EXPORT_ELAPSED_TIME
#define REQUIRES_ADDL_ARG__EXPORT_ELAPSED_TIME
#endif /* USE_CMD_EXPORT_ELAPSED_TIME */


#if defined(USE_CMD_START_TIMER) || defined(USE_CMD_ELAPSED_TIME) || defined(USE_CMD_EXPORT_ELAPSED_TIME)



#define ENUM_LIST__TIMER_FUNCS \
            ENUM_LIST__START_TIMER \
            ENUM_LIST__ELAPSED_TIME \
            ENUM_LIST__EXPORT_ELAPSED_TIME
#define VIRTUAL_ENUM_LIST__TIMER_FUNCS \
            VIRTUAL_ENUM_LIST__START_TIMER \
            VIRTUAL_ENUM_LIST__ELAPSED_TIME \
            VIRTUAL_ENUM_LIST__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:80 */ \
            COMMAND_INDEX__DO_NOT_USE_TIMER,
#define GLOBAL_VARDEF__TIMER_FUNCS \
            GLOBAL_VARDEF__START_TIMER \
            GLOBAL_VARDEF__ELAPSED_TIME \
            GLOBAL_VARDEF__EXPORT_ELAPSED_TIME
#define INITIALIZE__TIMER_FUNCS \
            INITIALIZE__START_TIMER \
            INITIALIZE__ELAPSED_TIME \
            INITIALIZE__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:81 */ \
            time_t timer_val = 0;
#define STARTUP_CASE__TIMER_FUNCS \
            STARTUP_CASE__START_TIMER \
            STARTUP_CASE__ELAPSED_TIME \
            STARTUP_CASE__EXPORT_ELAPSED_TIME
#define RUN_CASE__TIMER_FUNCS \
            RUN_CASE__START_TIMER \
            RUN_CASE__ELAPSED_TIME \
            RUN_CASE__EXPORT_ELAPSED_TIME
#define REQUIRES_ADDL_ARG__TIMER_FUNCS \
            REQUIRES_ADDL_ARG__START_TIMER \
            REQUIRES_ADDL_ARG__ELAPSED_TIME \
            REQUIRES_ADDL_ARG__EXPORT_ELAPSED_TIME

#else /* defined(USE_CMD_START_TIMER) || defined(USE_CMD_ELAPSED_TIME) || defined(USE_CMD_EXPORT_ELAPSED_TIME) */

#define ENUM_LIST__TIMER_FUNCS
#define VIRTUAL_ENUM_LIST__TIMER_FUNCS
#define GLOBAL_VARDEF__TIMER_FUNCS
#define INITIALIZE__TIMER_FUNCS
#define STARTUP_CASE__TIMER_FUNCS
#define RUN_CASE__TIMER_FUNCS
#define REQUIRES_ADDL_ARG__TIMER_FUNCS
#endif /* defined(USE_CMD_START_TIMER) || defined(USE_CMD_ELAPSED_TIME) || defined(USE_CMD_EXPORT_ELAPSED_TIME) */


#endif /* _FS_SHELL__CMD_TIMER_FUNCS_ */
