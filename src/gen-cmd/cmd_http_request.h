// GENERATED FROM cmd_http_request.h.in.  DO NOT EDIT.

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


#ifndef _FS_SHELL__CMD_EXPECT_HTTP_GET_RESPONSE_
#define _FS_SHELL__CMD_EXPECT_HTTP_GET_RESPONSE_

// Optional command expect-http-get-response

#ifdef USE_CMD_EXPECT_HTTP_GET_RESPONSE



#define HTTP_TIMEOUT 60
#define HTTP_STATUS_LINE_BUFFER_SIZE 20

// host port path expected-code
//  host -> shared_network_addr
//  port -> shared_network_addr
//  path -> global_arg_cached
//  code -> global_arg






        // host port path expected-code

        //  host -> shared_connect_address

        //  port -> global_arg_cached

        //  path -> expect_http_path

        //  code -> global_arg

        //  client socket -> tmp_val

/* from cmd_http_request.h.in:38 */
extern const char cmd_name_expect_http_get_response[];
#define ENUM_LIST__EXPECT_HTTP_GET_RESPONSE \
            /* from cmd_http_request.h.in:38 */ \
            COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE,
#define VIRTUAL_ENUM_LIST__EXPECT_HTTP_GET_RESPONSE \
            /* from cmd_http_request.h.in:56 */ \
            COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE__PATH, \
            /* from cmd_http_request.h.in:66 */ \
            COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE__CODE,
#define GLOBAL_VARDEF__EXPECT_HTTP_GET_RESPONSE \
            /* from cmd_http_request.h.in:38 */ \
            const char cmd_name_expect_http_get_response[] = "expect-http-get-response";
#define INITIALIZE__EXPECT_HTTP_GET_RESPONSE \
            /* from cmd_http_request.h.in:38 */ \
            command_list_names[COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE] = cmd_name_expect_http_get_response; \
            /* from cmd_http_request.h.in:67 */ \
            const char *expect_http_path = NULL; \
            /* Need just enough space to read in the first line.*/ \
            /*   We don't need the whole line, just enough to get the status code.*/ \
            /*   But this should be more than sufficient.*/ \
            char http_response_code_parse[HTTP_STATUS_LINE_BUFFER_SIZE];
#define STARTUP_CASE__EXPECT_HTTP_GET_RESPONSE \
    case COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE: \
        /* from cmd_http_request.h.in:38 */ \
            /* from cmd_http_request.h.in:40 */ \
            LOG(":: check http get response code equals\n"); \
            /* expect equals*/ \
            global_arg1_i = 0; \
            /* Argument 1: collect address shared_connect_address*/ \
            /* Argument 2: collect port/service into global_arg_cached*/ \
            global_cmd = COMMAND_INDEX__SHARED_CONNECT__ADDRESS; \
            /* Argument 3: path*/ \
            global_arg3_i = COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE__PATH; \
        break;
#define RUN_CASE__EXPECT_HTTP_GET_RESPONSE \
    case COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE__PATH: \
        /* from cmd_http_request.h.in:56 */ \
            /* from cmd_http_request.h.in:57 */ \
            /* Save off the path.*/ \
            expect_http_path = global_arg; \
            /* get the expected code and make the request.*/ \
            global_cmd = COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE__CODE; \
        break; \
    case COMMAND_INDEX__EXPECT_HTTP_GET_RESPONSE__CODE: \
        /* from cmd_http_request.h.in:66 */ \
            /* from cmd_http_request.h.in:83 */ \
            LOG(":: expecting http response code "); \
            LOGLN(global_arg); \
            /* don't actually parse it.  we're going to just do a string comparison.*/ \
            global_cmd = COMMAND_INDEX__ERR; \
            /* Initialize global_error to the opposite of the expected*/ \
            /*   equality state.  If we're expecting no match, then every error*/ \
            /*   condition is considered an "ok" error response.*/ \
            /*   expects errr -> global_arg1_i == 1; 1 - global_arg1_i == 0.*/ \
            /*   expects ok -> global_arg1_i == 0; 1 - global_arg1_i == 1.*/ \
            global_err = 1 - global_arg1_i; \
            /* Perform connection.*/ \
            tmp_val = shared_connect_to_address( \
                shared_connect_address, global_arg_cached, HTTP_TIMEOUT, \
                /* HTTP/3 allows UDP connections, but we aren't checking for that.*/ \
                IPPROTO_TCP); \
            if (tmp_val == -1) { \
                break; \
            } \
            /* send the data.*/ \
            LOG(":: connected to "); \
            LOG(shared_connect_address); \
            LOG(", port/service "); \
            LOG(global_arg_cached); \
            LOG("; sending GET "); \
            LOG(expect_http_path); \
            LOG(" HTTP/1.1\n"); \
            write(tmp_val, "GET ", 4); \
            write(tmp_val, expect_http_path, strlen(expect_http_path)); \
            write(tmp_val, " HTTP/1.1\r\n\r\n", 13); \
            fsync(tmp_val); \
            /* read in the response.  Just enough to get the protocol and the status code.*/ \
            /* Make sure we read in at most 1 less than the size of the buffer.*/ \
            LOG(":: Reading response\n"); \
            global_arg2_i = read(tmp_val, (void *)http_response_code_parse, HTTP_STATUS_LINE_BUFFER_SIZE - 1); \
            /* Nothing left to do with the socket.*/ \
            /* Close it; we don't care about what's left to read.*/ \
            close(tmp_val); \
            if (global_arg2_i <= 0) { \
                LOG(":: failed to read from HTTP response.\n"); \
                break; \
            } \
            /* Force a zero-terminated string where the read finished.*/ \
            /* Because of the other checks, this isn't strictly necessary,*/ \
            /*   but adds a bit of extra safety.*/ \
            http_response_code_parse[global_arg2_i] = '\0'; \
            LOG(":: retrieved http response ["); \
            LOG(http_response_code_parse); \
            LOG("]\n"); \
            /* find the status code.  If this is a valid http response, then*/ \
            /*  it's between spaces.  Never at index 0.*/ \
            global_arg3_i = 0; \
            for (tmp_val = 0; tmp_val < global_arg2_i; tmp_val++) { \
                if (http_response_code_parse[tmp_val] == ' ') { \
                    http_response_code_parse[tmp_val] = '\0'; \
                    if (global_arg3_i == 0 || global_arg3_i == tmp_val - 1) { \
                        /* The start of the status code hasn't been set, or there was*/ \
                        /*   an extra space between HTTP/1 and the status code.  We'll be*/ \
                        /*   nice and extra lenient, even though the spec requires just 1*/ \
                        /*   space.*/ \
                        global_arg3_i = tmp_val + 1; \
                    } else { \
                        /* At this point, we know the pointer to the status code is*/ \
                        /* before the current position.  So check if they are equal.*/ \
                        if (strcmp(&http_response_code_parse[global_arg3_i], global_arg) == 0) { \
                            LOG(":: response code matches expected value.\n"); \
                            /* It matched.  The error state should now be set to the*/ \
                            /*   if-equal state.*/ \
                            global_err = 1 - global_err; \
                        } \
                        break; \
                    } \
                } \
            } \
            /* Only for debugging purposes.  Because the LOG statement is stripped out*/ \
            /*   for non-debug builds, this if statement will also be removed.*/ \
            if (global_err != 0) { \
                LOG(":: did not match expected value.\n"); \
            } \
        break;
#define REQUIRES_ADDL_ARG__EXPECT_HTTP_GET_RESPONSE

#else /* USE_CMD_EXPECT_HTTP_GET_RESPONSE */

#define ENUM_LIST__EXPECT_HTTP_GET_RESPONSE
#define VIRTUAL_ENUM_LIST__EXPECT_HTTP_GET_RESPONSE
#define GLOBAL_VARDEF__EXPECT_HTTP_GET_RESPONSE
#define INITIALIZE__EXPECT_HTTP_GET_RESPONSE
#define STARTUP_CASE__EXPECT_HTTP_GET_RESPONSE
#define RUN_CASE__EXPECT_HTTP_GET_RESPONSE
#define REQUIRES_ADDL_ARG__EXPECT_HTTP_GET_RESPONSE
#endif /* USE_CMD_EXPECT_HTTP_GET_RESPONSE */

#endif /* _FS_SHELL__CMD_EXPECT_HTTP_GET_RESPONSE_ */
