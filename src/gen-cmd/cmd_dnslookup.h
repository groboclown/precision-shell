// GENERATED FROM cmd_dnslookup.h.in.  DO NOT EDIT.

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

#ifndef _FS_SHELL__CMD_DNSLOOKUP_
#define _FS_SHELL__CMD_DNSLOOKUP_


#ifndef HOSTS_FILEPATH
#define HOSTS_FILEPATH "/etc/hosts"
#endif

// Optional command export-host

#ifdef USE_CMD_EXPORT_HOST




        // Store the hostname to find in global_arg_cached
/* from cmd_dnslookup.h.in:32 */
extern const char cmd_name_export_host[];
#define ENUM_LIST__EXPORT_HOST \
            /* from cmd_dnslookup.h.in:32 */ \
            COMMAND_INDEX__EXPORT_HOST,
#define VIRTUAL_ENUM_LIST__EXPORT_HOST \
            /* from cmd_dnslookup.h.in:41 */ \
            COMMAND_INDEX__EXPORT_HOST__ENV,
#define GLOBAL_VARDEF__EXPORT_HOST \
            /* from cmd_dnslookup.h.in:32 */ \
            const char cmd_name_export_host[] = "export-host"; \
            /* from cmd_dnslookup.h.in:42 */ \
            const char hostfile_eol[] = "\n";
#define INITIALIZE__EXPORT_HOST \
            /* from cmd_dnslookup.h.in:32 */ \
            command_list_names[COMMAND_INDEX__EXPORT_HOST] = cmd_name_export_host;
#define STARTUP_CASE__EXPORT_HOST \
    case COMMAND_INDEX__EXPORT_HOST: \
        /* from cmd_dnslookup.h.in:32 */ \
            /* from cmd_dnslookup.h.in:34 */ \
            /* store the hostname to find*/ \
            global_cmd = COMMAND_INDEX__SHARED_STR; \
            global_arg3_i = COMMAND_INDEX__EXPORT_HOST__ENV; \
        break;
#define RUN_CASE__EXPORT_HOST \
    case COMMAND_INDEX__EXPORT_HOST__ENV: \
        /* from cmd_dnslookup.h.in:41 */ \
            /* from cmd_dnslookup.h.in:45 */ \
            /* hostname to find is in global_arg_cached*/ \
            /* env to store it in is global_arg*/ \
            /* Parse the /etc/hosts file using the argument parser.*/ \
            /*   It's good enough for what we want, which is:*/ \
            /*      find word separators + newline.*/ \
            /* Because the hosts file could potentially contain a*/ \
            /*   EOL replacement as a token, use our own EOL replacement instead.*/ \
            /*   We MUST replace it when done.*/ \
            shared_split_argv[0] = (char *) ArgReplaceNewline; \
            ArgReplaceNewline = hostfile_eol; \
            shared_split_input_argv[1] = "-f"; \
            shared_split_input_argv[2] = HOSTS_FILEPATH; \
            LOG(":: parsing "); \
            LOGLN(shared_split_input_argv[2]); \
            shared_split_arg_state = args_setup_tokenizer(3, (char **) shared_split_input_argv, 0); \
            if (shared_split_arg_state == NULL) { \
                global_err = 1; \
                ArgReplaceNewline = shared_split_argv[0]; \
                break; \
            } \
            /* Going to store the argument data in*/ \
            /*   shared_split_argv_buffer*/ \
            /* This will store at most 1 line of data.*/ \
            /* The format of the hosts file is on a per-line basis:*/ \
            /*   WHITESPACE = (' ' | '\t')+*/ \
            /*   comment line = '#' + any text*/ \
            /*   token = [a-zA-Z0-9:.-]+*/ \
            /*   name-lookup = TOKEN + WHITESPACE + TOKEN (+ WHITESPACE + TOKEN)*/ \
            /* The first token on the line is stored in shared_split_argv_buffer*/ \
            /* The current line state is stored in global_arg3_i*/ \
            global_arg3_i = 0; \
            while (1 == 1) { \
                shared_split_arg = args_advance_token(shared_split_arg_state); \
                if (shared_split_arg->state == ARG_STATE_END) { \
                    break; \
                } \
                if (shared_split_arg->state == ARG_STATE_ERR) { \
                    global_err = 1; \
                    break; \
                } \
                LOG(":: Parsed token '"); \
                LOG(shared_split_arg->arg); \
                LOG("'\n"); \
                /* Text that means EOL.*/ \
                if (strcmp(ArgReplaceNewline, shared_split_arg->arg) == 0) { \
                    /* Yep.  EOL.*/ \
                    global_arg3_i = 0; \
                } else if (global_arg3_i == 0) { \
                    /* Start of a line.*/ \
                    /* Should be at least 1 character in the string, even if it's '\0'*/ \
                    if (shared_split_arg->arg[0] == '#') { \
                        /* start comment.  Ignore everything else after here.*/ \
                        global_arg3_i = 1; \
                    } else if (shared_split_arg->arg[0] != '\0') { \
                        /* start of a token.  This is the translated address.*/ \
                        global_arg3_i = 2; \
                        strcpy(shared_split_argv_buffer, shared_split_arg->arg); \
                        LOG(":: found lookup IP "); \
                        LOGLN(shared_split_argv_buffer); \
                    } \
                } else if ( \
                        global_arg3_i == 2 \
                        /* Found a hostname.  Check if this token matches our search.*/ \
                        && strcmp(global_arg_cached, shared_split_arg->arg) == 0 \
                ) { \
                    LOG(":: matched host "); \
                    LOG(shared_split_arg->arg); \
                    LOG(" to "); \
                    LOG(shared_split_argv_buffer); \
                    LOG("; exporting as "); \
                    LOGLN(global_arg); \
                    global_err = setenv(global_arg, shared_split_argv_buffer, 1); \
                    /* stop searching*/ \
                    break; \
                } \
            } \
            global_err += args_close_tokenizer(shared_split_arg_state); \
            /* Restore the original EOL replacement text.*/ \
            ArgReplaceNewline = shared_split_argv[0]; \
        break;
#define REQUIRES_ADDL_ARG__EXPORT_HOST

#else /* USE_CMD_EXPORT_HOST */

#define ENUM_LIST__EXPORT_HOST
#define VIRTUAL_ENUM_LIST__EXPORT_HOST
#define GLOBAL_VARDEF__EXPORT_HOST
#define INITIALIZE__EXPORT_HOST
#define STARTUP_CASE__EXPORT_HOST
#define RUN_CASE__EXPORT_HOST
#define REQUIRES_ADDL_ARG__EXPORT_HOST
#endif /* USE_CMD_EXPORT_HOST */


#if defined(USE_CMD_EXPORT_HOST)

    // Nothing extra, for now.

#define ENUM_LIST__DNSLOOKUP \
            ENUM_LIST__EXPORT_HOST
#define VIRTUAL_ENUM_LIST__DNSLOOKUP \
            VIRTUAL_ENUM_LIST__EXPORT_HOST
#define GLOBAL_VARDEF__DNSLOOKUP \
            GLOBAL_VARDEF__EXPORT_HOST
#define INITIALIZE__DNSLOOKUP \
            INITIALIZE__EXPORT_HOST
#define STARTUP_CASE__DNSLOOKUP \
            STARTUP_CASE__EXPORT_HOST
#define RUN_CASE__DNSLOOKUP \
            RUN_CASE__EXPORT_HOST
#define REQUIRES_ADDL_ARG__DNSLOOKUP \
            REQUIRES_ADDL_ARG__EXPORT_HOST

#else /* defined(USE_CMD_EXPORT_HOST) */

#define ENUM_LIST__DNSLOOKUP
#define VIRTUAL_ENUM_LIST__DNSLOOKUP
#define GLOBAL_VARDEF__DNSLOOKUP
#define INITIALIZE__DNSLOOKUP
#define STARTUP_CASE__DNSLOOKUP
#define RUN_CASE__DNSLOOKUP
#define REQUIRES_ADDL_ARG__DNSLOOKUP
#endif /* defined(USE_CMD_EXPORT_HOST) */


#endif /* _FS_SHELL__CMD_DNSLOOKUP_ */
