// GENERATED FROM cmd_signal.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_SIGNAL_
#define _FS_SHELL__CMD_SIGNAL_

// Optional command signal

#ifdef USE_CMD_SIGNAL



#include <signal.h>

void signal_handler(int);

#ifdef USES_ENVIRONMENT

char signal_env_name[PARSED_ARG_SIZE] = "";

#define SIGNAL_ENV_CAPTURE \
    char signal_itoa[(3 * sizeof(long int)) + 8]; \
    if (signal_env_name[0] != '\0') { \
        LOG(":: Set env "); \
        LOG(signal_env_name); \
        LOG(" to captured signal "); \
        LOGLN(shared_itoa(signal, signal_itoa)); \
        setenv(signal_env_name, shared_itoa(signal, signal_itoa), 1); \
    }

#define SIGNAL_ENV_CALL \
    signal_handler(global_arg1_i);

#define SIGNAL_ENV_STORE \
    if (global_arg[0] == '*') { \
        strcpy(signal_env_name, global_arg + 1); \
        setenv(signal_env_name, "0", 1); \
        LOG(":: storing signal number into "); \
        LOGLN(signal_env_name); \
        break; \
    }

#else
#define SIGNAL_ENV_CAPTURE
#define SIGNAL_ENV_CALL
#define SIGNAL_ENV_STORE
#endif


/* from cmd_signal.h.in:63 */
extern const char cmd_name_signal[];
#define ENUM_LIST__SIGNAL \
            /* from cmd_signal.h.in:63 */ \
            COMMAND_INDEX__SIGNAL,
#define VIRTUAL_ENUM_LIST__SIGNAL
#define GLOBAL_VARDEF__SIGNAL \
            /* from cmd_signal.h.in:63 */ \
            const char cmd_name_signal[] = "signal"; \
            /* from cmd_signal.h.in:64 */ \
void signal_handler(int signal) { \
    SIGNAL_ENV_CAPTURE; \
    LOG(":: handled signal\n"); \
}
#define INITIALIZE__SIGNAL \
            /* from cmd_signal.h.in:63 */ \
            command_list_names[COMMAND_INDEX__SIGNAL] = cmd_name_signal; \
            /* from cmd_signal.h.in:72 */ \
            sigset_t global_signal_set;
#define STARTUP_CASE__SIGNAL \
    case COMMAND_INDEX__SIGNAL: \
        /* from cmd_signal.h.in:63 */ \
            /* from cmd_signal.h.in:76 */ \
            LOG(":: Initializing for signal handling\n"); \
            sigemptyset(&global_signal_set); \
        break;
#define RUN_CASE__SIGNAL \
    case COMMAND_INDEX__SIGNAL: \
        /* from cmd_signal.h.in:63 */ \
            /* from cmd_signal.h.in:81 */ \
            /* The "wait" string indicates the end of the signals.*/ \
            if (strequal("wait", global_arg)) { \
                LOG(":: start signal wait\n"); \
                /* Early POSIX Draft 6 has sigwait() return the signal number.*/ \
                /* New standard has sigwait() return 0 for no error, or the error number.*/ \
                /* Dietlibc uses the old return code.*/ \
                /* There's a small chance that an error occurs and that error code*/ \
                /*   matches the signal.*/ \
                global_arg1_i = 0; \
                global_err = sigwait(&global_signal_set, &global_arg1_i); \
/*#ifdef DEBUG */ \
/*printf(":: sigwait() returned %i, signal %i\n", global_err, global_arg1_i);*/ \
/*#endif*/ \
                if (global_err != 0 && global_err != global_arg1_i) { \
                    /* Due to the initialization and the old way of working,*/ \
                    /*   this situation means that this is a real error situation.*/ \
                    global_err = 1; \
                } else { \
                    /* Looks like a success.*/ \
                    global_err = 0; \
                    SIGNAL_ENV_CALL; \
                } \
                LOG(":: wait complete\n"); \
                /* Doesn't make sense to keep parsing signals at this point.*/ \
                global_cmd = COMMAND_INDEX__ERR; \
                break; \
            } \
            /* "wait" hasn't been found yet.*/ \
            /* If environment usage is on, then extra handling for*/ \
            /* capturing the signal number to an env can be done.*/ \
            SIGNAL_ENV_STORE; \
            /* Otherwise, the argument is a signal number.*/ \
            global_arg1_i = helper_arg_to_uint(global_arg, 10, 0xffff); \
            if (global_arg1_i < 0) { \
                global_err = 1; \
                /* Do not allow the signal to wait.  This can lead an invalid operations.*/ \
                global_cmd = COMMAND_INDEX__ERR; \
                break; \
            } \
            LOG(":: signal "); \
            LOGLN(global_arg); \
            sigaddset(&global_signal_set, global_arg1_i); \
            signal(global_arg1_i, &signal_handler); \
        break;
#define REQUIRES_ADDL_ARG__SIGNAL

#else /* USE_CMD_SIGNAL */

#define ENUM_LIST__SIGNAL
#define VIRTUAL_ENUM_LIST__SIGNAL
#define GLOBAL_VARDEF__SIGNAL
#define INITIALIZE__SIGNAL
#define STARTUP_CASE__SIGNAL
#define RUN_CASE__SIGNAL
#define REQUIRES_ADDL_ARG__SIGNAL
#endif /* USE_CMD_SIGNAL */

#endif /* _FS_SHELL__CMD_SIGNAL_ */
