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



// Optional command export-host-lookup

#ifdef USE_CMD_EXPORT_HOST_LOOKUP



        // Store the hostname to find in global_arg_cached

/* from cmd_dnslookup.h.in:29 */
extern const char cmd_name_export_host_lookup[];
#define ENUM_LIST__EXPORT_HOST_LOOKUP \
            /* from cmd_dnslookup.h.in:29 */ \
            COMMAND_INDEX__EXPORT_HOST_LOOKUP,
#define VIRTUAL_ENUM_LIST__EXPORT_HOST_LOOKUP
#define GLOBAL_VARDEF__EXPORT_HOST_LOOKUP \
            /* from cmd_dnslookup.h.in:29 */ \
            const char cmd_name_export_host_lookup[] = "export-host-lookup";
#define INITIALIZE__EXPORT_HOST_LOOKUP \
            /* from cmd_dnslookup.h.in:29 */ \
            command_list_names[COMMAND_INDEX__EXPORT_HOST_LOOKUP] = cmd_name_export_host_lookup;
#define STARTUP_CASE__EXPORT_HOST_LOOKUP \
    case COMMAND_INDEX__EXPORT_HOST_LOOKUP: \
        /* from cmd_dnslookup.h.in:29 */ \
            /* from cmd_dnslookup.h.in:32 */ \
            /* store the hostname to find*/ \
            global_cmd = COMMAND_INDEX__SHARED_ADDRESS; \
            global_arg2_i = COMMAND_INDEX__EXPORT_HOST_LOOKUP; \
        break;
#define RUN_CASE__EXPORT_HOST_LOOKUP \
    case COMMAND_INDEX__EXPORT_HOST_LOOKUP: \
        /* from cmd_dnslookup.h.in:29 */ \
            /* from cmd_dnslookup.h.in:38 */ \
            /* export the discovered shared_network_addr_str into the environment variable.*/ \
            setenv(global_arg, shared_network_addr_str, 1); \
        break;
#define REQUIRES_ADDL_ARG__EXPORT_HOST_LOOKUP

#else /* USE_CMD_EXPORT_HOST_LOOKUP */

#define ENUM_LIST__EXPORT_HOST_LOOKUP
#define VIRTUAL_ENUM_LIST__EXPORT_HOST_LOOKUP
#define GLOBAL_VARDEF__EXPORT_HOST_LOOKUP
#define INITIALIZE__EXPORT_HOST_LOOKUP
#define STARTUP_CASE__EXPORT_HOST_LOOKUP
#define RUN_CASE__EXPORT_HOST_LOOKUP
#define REQUIRES_ADDL_ARG__EXPORT_HOST_LOOKUP
#endif /* USE_CMD_EXPORT_HOST_LOOKUP */


#if defined(USE_CMD_EXPORT_HOST_LOOKUP)

    // Nothing extra, for now.

#define ENUM_LIST__DNSLOOKUP \
            ENUM_LIST__EXPORT_HOST_LOOKUP
#define VIRTUAL_ENUM_LIST__DNSLOOKUP \
            VIRTUAL_ENUM_LIST__EXPORT_HOST_LOOKUP
#define GLOBAL_VARDEF__DNSLOOKUP \
            GLOBAL_VARDEF__EXPORT_HOST_LOOKUP
#define INITIALIZE__DNSLOOKUP \
            INITIALIZE__EXPORT_HOST_LOOKUP
#define STARTUP_CASE__DNSLOOKUP \
            STARTUP_CASE__EXPORT_HOST_LOOKUP
#define RUN_CASE__DNSLOOKUP \
            RUN_CASE__EXPORT_HOST_LOOKUP
#define REQUIRES_ADDL_ARG__DNSLOOKUP \
            REQUIRES_ADDL_ARG__EXPORT_HOST_LOOKUP

#else /* defined(USE_CMD_EXPORT_HOST_LOOKUP) */

#define ENUM_LIST__DNSLOOKUP
#define VIRTUAL_ENUM_LIST__DNSLOOKUP
#define GLOBAL_VARDEF__DNSLOOKUP
#define INITIALIZE__DNSLOOKUP
#define STARTUP_CASE__DNSLOOKUP
#define RUN_CASE__DNSLOOKUP
#define REQUIRES_ADDL_ARG__DNSLOOKUP
#endif /* defined(USE_CMD_EXPORT_HOST_LOOKUP) */


#endif /* _FS_SHELL__CMD_DNSLOOKUP_ */
