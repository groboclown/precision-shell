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

#include <string.h>
#include <stdlib.h>

#ifdef USE_STREAMING_INPUT
#include <stdio.h>
#include <fcntl.h>
#endif

#include "output.h"
#include "helpers.h"
#include "args.h"
#include "load_input.h"


// When a newline character is encountered, the
// parsed arg points to this string instead of where
// the newline happens.  In Bash, the '-e' flag will
// turn this into '&&'.
// Changing this behavior is part of issue #14.
const char *ArgReplaceNewline = ";";


// Configurable buffer size.
//   By making this configurable, it means the tool is better
//   able to be sized into memory constrained environments.
#ifndef INPUT_BUFFER_SIZE
#define INPUT_BUFFER_SIZE  4096
#endif

// Parse states
enum ParseState {
    // Searching for an argument start
    PARSE_SEARCH = 0,

    // Inside a plain text argument
    PARSE_PLAIN,

    // Previous character was an escape character.
    PARSE_ESCAPE,
};


struct ArgState {
    // output is an allocated memory string.
    char *preserve_args[PARSED_ARG_SAVE_COUNT];
    int preserve_index;
    int put_back;
    LoadInputResult input_result;
    struct LoadInputState *input_state;
    char *read_buffer;
    int read_buffer_pos;
    int read_buffer_len;
    Argument arg_value;
};


#define PUT_BACK_NOPE -1
#define PUT_BACK_EOF 0


static const char MSG_ARG_LEN_TOO_LONG[] = "ERROR Argument length too long\n";


// ----------------------------------------------
// Creator

struct ArgState *args_setup_tokenizer(const int src_argc, char *src_argv[], int uses_environment) {
    int i, j;
    LoadInputContext input_context;
    struct ArgState *ret;

    ret = malloc(sizeof(struct ArgState));
    if (ret == NULL) {
        stderrP(helper_str__malloc_failed);
        return NULL;
    }

    // this read buffer memory has its ownership passed to the load_input.
    ret->read_buffer = malloc(sizeof(char) * PARSED_ARG_SIZE);
    if (ret->read_buffer == NULL) {
        stderrP(helper_str__malloc_failed);
        return NULL;
    }
    input_context.read_buffer = ret->read_buffer;
    input_context.read_buffer_size = PARSED_ARG_SIZE;

    // Force a read from input at the loop start.
    ret->read_buffer_pos = 0;
    ret->read_buffer_len = 0;

    for (i = 0; i < PARSED_ARG_SAVE_COUNT; i++) {
        ret->preserve_args[i] = malloc(sizeof(char) * PARSED_ARG_SIZE);
        if (ret->preserve_args[i] == NULL) {
            stderrP(helper_str__malloc_failed);
            while (--i >= 0) {
                free(ret->preserve_args[i]);
            }
            free(ret->read_buffer);
            free(ret);
            return NULL;
        }
    }
    ret->preserve_index = 0;
    ret->put_back = PUT_BACK_NOPE;

#ifdef USES_ENVIRONMENT
    input_context.uses_environment = uses_environment;
#endif

    if (src_argc < 2) {
        // no argument worth noting.
        input_context.input_type = DATA_SRC_NONE;
        
#ifdef USE_STREAMING_INPUT
    } else if (src_argc == 2 && strcmp("-", src_argv[1]) == 0) {
        // Read from stdin
        LOG(":: Reading input from stdin\n");
        input_context.input_type = DATA_SRC_STATIC_FD;
        input_context.input.fd.fd = STDIN_FILENO;

    } else if (src_argc == 3 && strcmp("-f", src_argv[1]) == 0) {
        // Read from a file.
        LOG(":: Running script ");
        LOGLN(src_argv[2]);
        input_context.input_type = DATA_SRC_MANAGED_FD;
        input_context.input.fd.fd = open(src_argv[2], O_RDONLY);
        if (input_context.input.fd.fd == -1) {
            stderrP("ERROR opening file ");
            stderrPLn(src_argv[2]);
            // load_input not called yet, so need to explicitly free the read buffer.
            free(ret->read_buffer);
            goto OnError;
        }
#endif /* USE_STREAMING_INPUT */

    } else if (src_argc == 3 && strcmp("-c", src_argv[1]) == 0) {
        // Use the src_argv[2] as a full script.
        input_context.input_type = DATA_SRC_STATIC_STRING;
        input_context.input.string.source_chars = (char *) src_argv[2];

    } else {
        // Process the args as though there's a single space between
        // each argument.
        // argv must start in a parsable way.  We know it's parsable, because src_arg >= 2.
        input_context.input_type = DATA_SRC_ARGV;
        input_context.input.argv.argc = src_argc;
        input_context.input.argv.argv = src_argv;
    }

    ret->input_state = load_input_initialize(&input_context);
    if (ret->input_state == NULL) {
        // Error already reported.
        goto OnError;
    }
    
    return ret;

OnError:
    // Does not close fd or free the read buffer, because the
    // load_input may have already done that.
    for (i = 0; i < PARSED_ARG_SAVE_COUNT; i++) {
        free(ret->preserve_args[i]);
    }
    free(ret);
    return NULL;
}

// ---------------------------------------------
// Close function

int args_close_tokenizer(struct ArgState *state) {
    int i;
    int ret = 0;
    for (i = 0; i < PARSED_ARG_SAVE_COUNT; i++) {
        if (state->preserve_args[i] != NULL) {
            free(state->preserve_args[i]);
        }
    }
    ret += load_input_close(state->input_state);
    free(state);
    return ret;
}


// ----------------------------------------------
// Tokenize functions


// Parse a single token from the input and return it.
const Argument *args_advance_token(struct ArgState *state) {
    // Due to the way the parser is written, this always
    // just reads a single token.  So parsing state can be
    // contained just here.
    char to_parse = 0;
    enum ParseState parse_state = PARSE_SEARCH;
    int quote_depth = 0;
    state->arg_value.state = ARG_STATE_NORMAL;

    // Loop through the preserved argument list.
    state->preserve_index = (state->preserve_index + 1) % PARSED_ARG_SAVE_COUNT;

    // Write to this position.
    char *write_pos = state->preserve_args[state->preserve_index];
    state->arg_value.arg = write_pos;

    // End at this position
    char *write_pos_end = &(write_pos[PARSED_ARG_SIZE]);

    while (1 == 1) {
        // The very first write of this loop using:
        //    *(write_pos++)
        // is guaranteed because of this check.
        // Any additional write will require checks first.

        if (write_pos >= write_pos_end) {
            stderrP(MSG_ARG_LEN_TOO_LONG);
            state->arg_value.state = ARG_STATE_ERR;
            return &state->arg_value;
        }

        // --------------------------------------
        // Read the next character.
        if (state->put_back == PUT_BACK_EOF) {
            // Something had finished up an argument, meaning that the next
            //   call needs to report the EOF.
            // Tell the requestor that this is the end.
            state->arg_value.state = ARG_STATE_END;
            return &state->arg_value;
        } else if (state->put_back != PUT_BACK_NOPE) {
            to_parse = (char) state->put_back;
            state->put_back = PUT_BACK_NOPE;
        } else {
            if (state->read_buffer_pos >= state->read_buffer_len) {
                // Read from the input to get more data.
                load_input_read(state->input_state, &state->input_result);
                state->read_buffer_len = state->input_result.data_length;
                state->read_buffer_pos = 0;
                if (state->input_result.status == LOAD_INPUT_STATUS_ERR) {
                    state->arg_value.state = ARG_STATE_ERR;
                    return &state->arg_value;
                }

                // ------------------------------
                // Handle end-of-stream
                if (state->input_result.status == LOAD_INPUT_STATUS_EOF) {
                    // end of input string marker.  Does not fall out of if.
                    //   This might be before the full input buffer is read,
                    //   but we'll still recognize it as a termination of the
                    //   input.
                    if (state == PARSE_SEARCH) {
                        // Still searching for an argument, so return that this is
                        // an EOF.
                        state->put_back = PUT_BACK_EOF;
                        continue;
                    }

                    // Note - end-of-stream in the middle of escape sequences or
                    //   environment variables are considered "unsupported states"
                    //   and are not handled in any nice way.

                    // Some kind of argument is being parsed, so wrap it up &
                    // tell the parser the next time around that it's EOF.
                    // Should report an error if in ESCAPE mode.

                    // Note that there's no need to increment the target here.
                    *write_pos = 0;
                    LOG(":: Parsed argument '");
                    LOG(state->arg_value.arg);
                    LOG("'\n");
                    // When we come back, return an EOF.
                    state->put_back = PUT_BACK_EOF;
                    return &state->arg_value;
                }
                // Retry reading.  It might have returned a valid 0 length buffer.
                continue;
            }
            to_parse = state->read_buffer[state->read_buffer_pos++];
        }

//printf("[trace arg] arg char %d (%c)\n", to_parse, to_parse);

        // --------------------------------------
        // Handle escaping
        if (parse_state == PARSE_ESCAPE) {
// printf("[trace arg] parsing escaped char %d (%c)\n", to_parse, to_parse);
            switch (to_parse) {
                case 'n':
                    *write_pos = '\n';
                    break;
                case 'r':
                    *write_pos = '\r';
                    break;
                case 't':
                    *write_pos = '\t';
                    break;
                default:
                    *write_pos = to_parse;
                    break;
            }
            write_pos++;
            // We will always be out of search at this point.
            parse_state = PARSE_PLAIN;
            continue;
        }


        switch (to_parse) {
            // case '\0': not needed to special handle.
            case '\n':
// printf("[trace arg] parsing newline\n");
                // Newline.
                if (parse_state == PARSE_SEARCH) {
                    // Outside text.  This is like a ';' or '&&' depending on the setup.
                    // Use that value instead of writing it to the argument value.
                    state->arg_value.arg = ArgReplaceNewline;
                    LOG(":: Parsed newline as '");
                    LOG(state->arg_value.arg);
                    LOG("'\n");
                    return &state->arg_value;
                }
                if (parse_state == PARSE_PLAIN) {
                    // Already parsing an argument.  Mark this as the end of the argument,
                    // and put it back so it can be correctly interpreted when parsing
                    // the next argument.
                    state->put_back = '\n';

                    // Note that there's no need to increment the target here.
                    *write_pos = 0;
                    LOG(":: Parsed argument '");
                    LOG(state->arg_value.arg);
                    LOG("' before the newline\n");
                    return &state->arg_value;
                } else {
                    // In quoted text, so keep it in the buffer.
                    *(write_pos++) = '\n';
                }
                break;
            case '[':
// printf("[trace arg] parsing quote start\n");
                // Enter a quote context.
                // Either searching for the start of an argument, or already in
                // an argument.  Either case, enter a quoted context.
                state->arg_value.state = ARG_STATE_ESCAPED;
                parse_state = PARSE_PLAIN;
                if (quote_depth > 0) {
                    // Keep this character only if it's inside an already quoted context.
                    *(write_pos++) = '[';
                }
                quote_depth++;
                break;
            case ']':
// printf("[trace arg] parsing quote end\n");
                // Exit a quote context.
                // This will always move us out of search.
                parse_state = PARSE_PLAIN;
                if (quote_depth == 1) {
                    // Out of quoting.
                    quote_depth = 0;
                }
                if (quote_depth > 1) {
                    quote_depth--;
                    // Keep this character only if it's inside an already quoted context.
                    *(write_pos++) = ']';
                }
                break;
            case ' ':
            case '\r':
            case '\t':
// printf("[trace arg] parsing whitespace char %d\n", to_parse);
                if (parse_state == PARSE_PLAIN && quote_depth <= 0) {
                    // Inside an unquoted section of an argument.  This ends it.
                    // No need to increment the write position.
                    *write_pos = 0;
                    LOG(":: Parsed argument '");
                    LOG(state->arg_value.arg);
                    LOG("'\n");
                    return &state->arg_value;
                } else if (parse_state != PARSE_SEARCH) {
                    // Inside a quoted part.  Make it part of the argument.
                    *(write_pos++) = to_parse;
                }
                // else still searching for the argument to start.
                break;
            case '\\':
// printf("[trace arg] parsing escape\n");
                // Standard escaping.
                // Turn on the escape mode for the next time around.
                parse_state = PARSE_ESCAPE;
                break;
            default:
// printf("[trace arg] parsing normal char %d\n", to_parse);
                // looking for an argument start, and we found it.
                parse_state = PARSE_PLAIN;

                // Always keep the character.
                *(write_pos++) = to_parse;
                break;
        }
    }
}
