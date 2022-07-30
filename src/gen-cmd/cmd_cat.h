// GENERATED FROM cmd_cat.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_CAT_ENV_CAT_
#define _FS_SHELL__CMD_CAT_ENV_CAT_


#define CAT_FILE_BUFFER_SIZE 4096

// Optional command cat-fd

#ifdef USE_CMD_CAT_FD


/* from cmd_cat.h.in:30 */
extern const char cmd_name_cat_fd[];
#define ENUM_LIST__CAT_FD \
            /* from cmd_cat.h.in:30 */ \
            COMMAND_INDEX__CAT_FD,
#define VIRTUAL_ENUM_LIST__CAT_FD
#define GLOBAL_VARDEF__CAT_FD \
            /* from cmd_cat.h.in:30 */ \
            const char cmd_name_cat_fd[] = "cat-fd";
#define INITIALIZE__CAT_FD \
            /* from cmd_cat.h.in:30 */ \
            command_list_names[COMMAND_INDEX__CAT_FD] = cmd_name_cat_fd;
#define STARTUP_CASE__CAT_FD \
    case COMMAND_INDEX__CAT_FD: \
        /* from cmd_cat.h.in:30 */ \
            /* from cmd_cat.h.in:31 */ \
            /* arg2: the file descriptor to write to*/ \
            global_cmd = COMMAND_INDEX__SHARED_INT2; \
            global_arg3_i = COMMAND_INDEX__CAT_FILE; \
            /* arg1: whether to perform environment variable replacement.*/ \
            global_arg1_i = 0; \
        break;
#define RUN_CASE__CAT_FD
#define REQUIRES_ADDL_ARG__CAT_FD

#else /* USE_CMD_CAT_FD */

#define ENUM_LIST__CAT_FD
#define VIRTUAL_ENUM_LIST__CAT_FD
#define GLOBAL_VARDEF__CAT_FD
#define INITIALIZE__CAT_FD
#define STARTUP_CASE__CAT_FD
#define RUN_CASE__CAT_FD
#define REQUIRES_ADDL_ARG__CAT_FD
#endif /* USE_CMD_CAT_FD */

// Optional command env-cat-fd

#ifdef USE_CMD_ENV_CAT_FD



/* from cmd_cat.h.in:44 */
extern const char cmd_name_env_cat_fd[];
#define ENUM_LIST__ENV_CAT_FD \
            /* from cmd_cat.h.in:44 */ \
            COMMAND_INDEX__ENV_CAT_FD,
#define VIRTUAL_ENUM_LIST__ENV_CAT_FD
#define GLOBAL_VARDEF__ENV_CAT_FD \
            /* from cmd_cat.h.in:44 */ \
            const char cmd_name_env_cat_fd[] = "env-cat-fd";
#define INITIALIZE__ENV_CAT_FD \
            /* from cmd_cat.h.in:44 */ \
            command_list_names[COMMAND_INDEX__ENV_CAT_FD] = cmd_name_env_cat_fd;
#define STARTUP_CASE__ENV_CAT_FD \
    case COMMAND_INDEX__ENV_CAT_FD: \
        /* from cmd_cat.h.in:44 */ \
            /* from cmd_cat.h.in:45 */ \
            /* arg2: the file descriptor to write to*/ \
            global_cmd = COMMAND_INDEX__SHARED_INT2; \
            global_arg3_i = COMMAND_INDEX__CAT_FILE; \
            /* arg1: whether to perform environment variable replacement.*/ \
            global_arg1_i = 1; \
        break;
#define RUN_CASE__ENV_CAT_FD
#define REQUIRES_ADDL_ARG__ENV_CAT_FD

#else /* USE_CMD_ENV_CAT_FD */

#define ENUM_LIST__ENV_CAT_FD
#define VIRTUAL_ENUM_LIST__ENV_CAT_FD
#define GLOBAL_VARDEF__ENV_CAT_FD
#define INITIALIZE__ENV_CAT_FD
#define STARTUP_CASE__ENV_CAT_FD
#define RUN_CASE__ENV_CAT_FD
#define REQUIRES_ADDL_ARG__ENV_CAT_FD
#endif /* USE_CMD_ENV_CAT_FD */


#if defined(USE_CMD_CAT_FD) || defined(USE_CMD_ENV_CAT_FD)


#include <stdio.h>
#include <fcntl.h>
#include "load_input.h"




#define ENUM_LIST__CAT_ENV_CAT \
            ENUM_LIST__CAT_FD \
            ENUM_LIST__ENV_CAT_FD
#define VIRTUAL_ENUM_LIST__CAT_ENV_CAT \
            VIRTUAL_ENUM_LIST__CAT_FD \
            VIRTUAL_ENUM_LIST__ENV_CAT_FD \
            /* from cmd_cat.h.in:65 */ \
            COMMAND_INDEX__CAT_FILE,
#define GLOBAL_VARDEF__CAT_ENV_CAT \
            GLOBAL_VARDEF__CAT_FD \
            GLOBAL_VARDEF__ENV_CAT_FD
#define INITIALIZE__CAT_ENV_CAT \
            INITIALIZE__CAT_FD \
            INITIALIZE__ENV_CAT_FD \
            /* from cmd_cat.h.in:66 */ \
            LoadInputContext cat_input_context; \
            LoadInputResult cat_input_result; \
            struct LoadInputState *cat_input_state;
#define STARTUP_CASE__CAT_ENV_CAT \
            STARTUP_CASE__CAT_FD \
            STARTUP_CASE__ENV_CAT_FD
#define RUN_CASE__CAT_ENV_CAT \
            RUN_CASE__CAT_FD \
            RUN_CASE__ENV_CAT_FD \
    case COMMAND_INDEX__CAT_FILE: \
        /* from cmd_cat.h.in:65 */ \
            /* from cmd_cat.h.in:72 */ \
            cat_input_context.read_buffer_size = CAT_FILE_BUFFER_SIZE; \
            cat_input_context.input_type = DATA_SRC_MANAGED_FD; \
            cat_input_context.uses_environment = global_arg1_i; \
            LOG(":: opening "); \
            LOG(global_arg); \
            LOG(": for read\n"); \
            cat_input_context.input.fd.fd = open(global_arg, O_RDONLY); \
            if (cat_input_context.input.fd.fd == -1) { \
                stderrP("ERROR opening file "); \
                stderrPLn(global_arg); \
                global_err = 1; \
                break; \
            } \
            cat_input_context.read_buffer = malloc(sizeof(char) * CAT_FILE_BUFFER_SIZE); \
            if (cat_input_context.read_buffer == NULL) { \
                stderrP(helper_str__malloc_failed); \
                close(cat_input_context.input.fd.fd); \
                global_err = 1; \
                break; \
            } \
            cat_input_state = load_input_initialize(&cat_input_context); \
            if (cat_input_state == NULL) { \
                /* initialize freed up memory & closed handles for us.*/ \
                global_err = 1; \
                break; \
            } \
            while (1 == 1) { \
                load_input_read(cat_input_state, &cat_input_result); \
                if (cat_input_result.status == LOAD_INPUT_STATUS_ERR) { \
                    global_err = 1; \
                } \
                if (cat_input_result.status >= LOAD_INPUT_STATUS_EOF) { \
                    break; \
                } \
                tmp_val = write(global_arg2_i, cat_input_result.data, cat_input_result.data_length); \
                if (tmp_val != cat_input_result.data_length) { \
                    stderrP("ERROR writing\n"); \
                    break; \
                } \
            } \
            /* load_input will close the fd for us, because we marked it as managed.*/ \
            global_err += load_input_close(cat_input_state); \
        break;
#define REQUIRES_ADDL_ARG__CAT_ENV_CAT \
            REQUIRES_ADDL_ARG__CAT_FD \
            REQUIRES_ADDL_ARG__ENV_CAT_FD

#else /* defined(USE_CMD_CAT_FD) || defined(USE_CMD_ENV_CAT_FD) */

#define ENUM_LIST__CAT_ENV_CAT
#define VIRTUAL_ENUM_LIST__CAT_ENV_CAT
#define GLOBAL_VARDEF__CAT_ENV_CAT
#define INITIALIZE__CAT_ENV_CAT
#define STARTUP_CASE__CAT_ENV_CAT
#define RUN_CASE__CAT_ENV_CAT
#define REQUIRES_ADDL_ARG__CAT_ENV_CAT
#endif /* defined(USE_CMD_CAT_FD) || defined(USE_CMD_ENV_CAT_FD) */


#endif /* _FS_SHELL__CMD_CAT_ENV_CAT_ */
