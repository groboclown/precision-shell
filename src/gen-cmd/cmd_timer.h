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

// difftime introduces a floating point dependency, which adds bloat.
// and all we need is just a simple subtraction.
#define SECONDS_SINCE_START ((int) (time(NULL) - timer_val))

// Optional command start-timer

#ifdef USE_CMD_START_TIMER



/* from cmd_timer.h.in:35 */
extern const char cmd_name_start_timer[];
#define ENUM_LIST__START_TIMER \
            /* from cmd_timer.h.in:35 */ \
            COMMAND_INDEX__START_TIMER,
#define VIRTUAL_ENUM_LIST__START_TIMER
#define GLOBAL_VARDEF__START_TIMER \
            /* from cmd_timer.h.in:35 */ \
            const char cmd_name_start_timer[] = "start-timer";
#define INITIALIZE__START_TIMER \
            /* from cmd_timer.h.in:35 */ \
            command_list_names[COMMAND_INDEX__START_TIMER] = cmd_name_start_timer;
#define STARTUP_CASE__START_TIMER \
    case COMMAND_INDEX__START_TIMER: \
        /* from cmd_timer.h.in:35 */ \
            /* from cmd_timer.h.in:36 */ \
            LOG(":: start global timer\n"); \
            timer_val = time(NULL); \
            global_cmd = COMMAND_INDEX__ERR; \
            /* DEBUG*/ \
            /* printf(":: timer start %ld\n", timer_val);*/ \
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



/* from cmd_timer.h.in:51 */
extern const char cmd_name_elapsed_time[];
#define ENUM_LIST__ELAPSED_TIME \
            /* from cmd_timer.h.in:51 */ \
            COMMAND_INDEX__ELAPSED_TIME,
#define VIRTUAL_ENUM_LIST__ELAPSED_TIME
#define GLOBAL_VARDEF__ELAPSED_TIME \
            /* from cmd_timer.h.in:51 */ \
            const char cmd_name_elapsed_time[] = "elapsed-time";
#define INITIALIZE__ELAPSED_TIME \
            /* from cmd_timer.h.in:51 */ \
            command_list_names[COMMAND_INDEX__ELAPSED_TIME] = cmd_name_elapsed_time;
#define STARTUP_CASE__ELAPSED_TIME \
    case COMMAND_INDEX__ELAPSED_TIME: \
        /* from cmd_timer.h.in:51 */ \
            /* from cmd_timer.h.in:52 */ \
            LOG(":: report elapsed time since timer start\n"); \
            /* stdoutPLn(shared_itoa(difftime(time(0), timer_val), global_itoa));*/ \
            stdoutPLn(shared_itoa(SECONDS_SINCE_START, global_itoa)); \
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


// Optional command elapsed-time-under

#ifdef USE_CMD_ELAPSED_TIME_UNDER



/* from cmd_timer.h.in:65 */
extern const char cmd_name_elapsed_time_under[];
#define ENUM_LIST__ELAPSED_TIME_UNDER \
            /* from cmd_timer.h.in:65 */ \
            COMMAND_INDEX__ELAPSED_TIME_UNDER,
#define VIRTUAL_ENUM_LIST__ELAPSED_TIME_UNDER
#define GLOBAL_VARDEF__ELAPSED_TIME_UNDER \
            /* from cmd_timer.h.in:65 */ \
            const char cmd_name_elapsed_time_under[] = "elapsed-time-under";
#define INITIALIZE__ELAPSED_TIME_UNDER \
            /* from cmd_timer.h.in:65 */ \
            command_list_names[COMMAND_INDEX__ELAPSED_TIME_UNDER] = cmd_name_elapsed_time_under;
#define STARTUP_CASE__ELAPSED_TIME_UNDER
#define RUN_CASE__ELAPSED_TIME_UNDER \
    case COMMAND_INDEX__ELAPSED_TIME_UNDER: \
        /* from cmd_timer.h.in:65 */ \
            /* from cmd_timer.h.in:66 */ \
            LOG(":: check if the elapsed time is less than "); \
            LOGLN(global_arg); \
            global_cmd = COMMAND_INDEX__ERR; \
            global_err = 1; \
            global_arg1_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
            if (global_arg1_i >= 0) { \
                /* difftime introduces a floating point dependency, which adds bloat.*/ \
                if (SECONDS_SINCE_START < (time_t) global_arg1_i) { \
                    LOG("::  Yes, it is less than the expected value.\n"); \
                    global_err = 0; \
                /* DEBUG*/ \
                /* } else {*/ \
                /*     printf(":: No: %ld - %ld = %d >= %d\n", time(NULL), timer_val, SECONDS_SINCE_START, global_arg1_i);*/ \
                } \
            } \
        break;
#define REQUIRES_ADDL_ARG__ELAPSED_TIME_UNDER

#else /* USE_CMD_ELAPSED_TIME_UNDER */

#define ENUM_LIST__ELAPSED_TIME_UNDER
#define VIRTUAL_ENUM_LIST__ELAPSED_TIME_UNDER
#define GLOBAL_VARDEF__ELAPSED_TIME_UNDER
#define INITIALIZE__ELAPSED_TIME_UNDER
#define STARTUP_CASE__ELAPSED_TIME_UNDER
#define RUN_CASE__ELAPSED_TIME_UNDER
#define REQUIRES_ADDL_ARG__ELAPSED_TIME_UNDER
#endif /* USE_CMD_ELAPSED_TIME_UNDER */


// Optional command export-elapsed-time

#ifdef USE_CMD_EXPORT_ELAPSED_TIME



/* from cmd_timer.h.in:94 */
extern const char cmd_name_export_elapsed_time[];
#define ENUM_LIST__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:94 */ \
            COMMAND_INDEX__EXPORT_ELAPSED_TIME,
#define VIRTUAL_ENUM_LIST__EXPORT_ELAPSED_TIME
#define GLOBAL_VARDEF__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:94 */ \
            const char cmd_name_export_elapsed_time[] = "export-elapsed-time";
#define INITIALIZE__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:94 */ \
            command_list_names[COMMAND_INDEX__EXPORT_ELAPSED_TIME] = cmd_name_export_elapsed_time;
#define STARTUP_CASE__EXPORT_ELAPSED_TIME
#define RUN_CASE__EXPORT_ELAPSED_TIME \
    case COMMAND_INDEX__EXPORT_ELAPSED_TIME: \
        /* from cmd_timer.h.in:94 */ \
            /* from cmd_timer.h.in:95 */ \
            LOG(":: storing elapsed time "); \
            /* difftime introduces a floating point dependency, which adds bloat.*/ \
            /* and all we need is just a simple subtraction.*/ \
            /* global_itoa_ptr = shared_itoa(difftime(time(0), timer_val), global_itoa);*/ \
            global_itoa_ptr = shared_itoa(SECONDS_SINCE_START, global_itoa); \
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


#if defined(USE_CMD_START_TIMER) || defined(USE_CMD_ELAPSED_TIME) || defined(USE_CMD_ELAPSED_TIME_UNDER) || defined(USE_CMD_EXPORT_ELAPSED_TIME)



#define ENUM_LIST__TIMER_FUNCS \
            ENUM_LIST__START_TIMER \
            ENUM_LIST__ELAPSED_TIME \
            ENUM_LIST__ELAPSED_TIME_UNDER \
            ENUM_LIST__EXPORT_ELAPSED_TIME
#define VIRTUAL_ENUM_LIST__TIMER_FUNCS \
            VIRTUAL_ENUM_LIST__START_TIMER \
            VIRTUAL_ENUM_LIST__ELAPSED_TIME \
            VIRTUAL_ENUM_LIST__ELAPSED_TIME_UNDER \
            VIRTUAL_ENUM_LIST__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:113 */ \
            COMMAND_INDEX__DO_NOT_USE_TIMER,
#define GLOBAL_VARDEF__TIMER_FUNCS \
            GLOBAL_VARDEF__START_TIMER \
            GLOBAL_VARDEF__ELAPSED_TIME \
            GLOBAL_VARDEF__ELAPSED_TIME_UNDER \
            GLOBAL_VARDEF__EXPORT_ELAPSED_TIME \
            /* from cmd_timer.h.in:114 */ \
            /* For many use cases, the timer must go outside the sub-command,*/ \
            /*   and so it can't be in the "OnInit", which is sub-command local.*/ \
            /*   So instead, it is put in the global space.*/ \
            /*   For an example, the delay-until-ready.Dockerfile only works*/ \
            /*   when this is global.  Otherwise, the timer check inside the loop*/ \
            /*   always fails.*/ \
            time_t timer_val = 0;
#define INITIALIZE__TIMER_FUNCS \
            INITIALIZE__START_TIMER \
            INITIALIZE__ELAPSED_TIME \
            INITIALIZE__ELAPSED_TIME_UNDER \
            INITIALIZE__EXPORT_ELAPSED_TIME
#define STARTUP_CASE__TIMER_FUNCS \
            STARTUP_CASE__START_TIMER \
            STARTUP_CASE__ELAPSED_TIME \
            STARTUP_CASE__ELAPSED_TIME_UNDER \
            STARTUP_CASE__EXPORT_ELAPSED_TIME
#define RUN_CASE__TIMER_FUNCS \
            RUN_CASE__START_TIMER \
            RUN_CASE__ELAPSED_TIME \
            RUN_CASE__ELAPSED_TIME_UNDER \
            RUN_CASE__EXPORT_ELAPSED_TIME
#define REQUIRES_ADDL_ARG__TIMER_FUNCS \
            REQUIRES_ADDL_ARG__START_TIMER \
            REQUIRES_ADDL_ARG__ELAPSED_TIME \
            REQUIRES_ADDL_ARG__ELAPSED_TIME_UNDER \
            REQUIRES_ADDL_ARG__EXPORT_ELAPSED_TIME

#else /* defined(USE_CMD_START_TIMER) || defined(USE_CMD_ELAPSED_TIME) || defined(USE_CMD_ELAPSED_TIME_UNDER) || defined(USE_CMD_EXPORT_ELAPSED_TIME) */

#define ENUM_LIST__TIMER_FUNCS
#define VIRTUAL_ENUM_LIST__TIMER_FUNCS
#define GLOBAL_VARDEF__TIMER_FUNCS
#define INITIALIZE__TIMER_FUNCS
#define STARTUP_CASE__TIMER_FUNCS
#define RUN_CASE__TIMER_FUNCS
#define REQUIRES_ADDL_ARG__TIMER_FUNCS
#endif /* defined(USE_CMD_START_TIMER) || defined(USE_CMD_ELAPSED_TIME) || defined(USE_CMD_ELAPSED_TIME_UNDER) || defined(USE_CMD_EXPORT_ELAPSED_TIME) */


#endif /* _FS_SHELL__CMD_TIMER_FUNCS_ */
