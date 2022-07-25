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


// When a newline character is encountered, the
// parsed arg points to this string instead of where
// the newline happens.  In Bash, the '-e' flag will
// turn this into '&&'.
// Changing this behavior is part of issue #14.
static const char *_ReplaceNewline = ";";


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

    // Inside a double-quote part of an argument
    PARSE_DOUBLE,

    // Inside a single-quote part of an argument
    PARSE_SINGLE,

    // Just found an escape character
    //   Set up such that x - PARSE_ESCAPE_PLAIN + PARSE_PLAIN gives the original state,
    //   and x + PARSE_ESCAPE_PLAIN - PARSE_PLAIN gives the escaped state
    PARSE_ESCAPE_PLAIN,
    PARSE_ESCAPE_DOUBLE,
    PARSE_ESCAPE_SINGLE
};

enum ParseSrc {
    // Things with closable actions are first.
    PARSE_SRC_ALLOCATED_STRING = 0,

#ifdef USE_STREAMING_INPUT
    PARSE_SRC_CLOSABLE_FD,
#endif

    // These have no close actions.
#ifdef USE_STREAMING_INPUT
    PARSE_SRC_EXTERN_FD,
#endif
    PARSE_SRC_NONE,
    PARSE_SRC_ARGV,
    PARSE_SRC_STATIC_STRING
};

// Buffers:
//    size = allocated size (cannot exceed this length).
//    len = number of usable values.


typedef struct {
    // Not allocated memory
    char **argv;
    int argc;
    int argv_index;
    int str_pos;
} _ParseSrcArgv;


typedef struct {
    // source is allocated
    char *source_chars;
    int str_pos;
} _ParseSrcString;


#ifdef USE_STREAMING_INPUT
typedef struct {
    // buffer is an allocated memory string.
    char *buffer;
    int str_pos;
    int length;

    // if source_type is PARSE_SRC_CLOSABLE_FD, then this needs to be closed.
    int fd;
} _ParseSrcFd;
#endif


// When the sub-command parsing happens, this can take
// a parent reference structure, too.


typedef union {
    // struct none {};  "none" has no data.
    _ParseSrcArgv argv;
    _ParseSrcString string;
#ifdef USE_STREAMING_INPUT
    _ParseSrcFd fd;
#endif
} _ParseSrc;


typedef struct {
    // output is an allocated memory string.
    char *preserve_args[PARSED_ARG_SAVE_COUNT];
    int preserve_index;
    int put_back;
    enum ParseSrc source_type;
    _ParseSrc source;
} _ParseData;


#define PUT_BACK_NOPE -1
#define PUT_BACK_EOF 0

static _ParseData _parse_data;
static Argument _arg_value;


void args_tokenize_buffer(int);

// ---------------------------------------------
// Close function

int args_close_tokenizer() {
    int i;
    int ret = 0;
    for (i = 0; i < PARSED_ARG_SAVE_COUNT; i++) {
        if (_parse_data.preserve_args[i] != NULL) {
            free(_parse_data.preserve_args[i]);
        }
    }

    // Parse source types that need freeing.
    if (_parse_data.source_type == PARSE_SRC_ALLOCATED_STRING) {
        free(_parse_data.source.string.source_chars);
#ifdef USE_STREAMING_INPUT
    } else if (_parse_data.source_type == PARSE_SRC_CLOSABLE_FD) {
        ret = close(_parse_data.source.fd.fd);
#endif
    }
#ifdef USE_STREAMING_INPUT
    if (
            (
                _parse_data.source_type == PARSE_SRC_CLOSABLE_FD
                || _parse_data.source_type == PARSE_SRC_EXTERN_FD
            ) && _parse_data.source.fd.buffer != NULL
    ) {
        free(_parse_data.source.fd.buffer);
        _parse_data.source.fd.buffer = NULL;
    }
#endif

    // Mark the data as freed
    _parse_data.source_type = PARSE_SRC_NONE;
    return ret;
}


// ----------------------------------------------
// Tokenize functions


// Parse a single token from the input and return it.
const Argument *args_advance_token() {
    // Due to the way the parser is written, this always
    // just reads a single token.  So parsing state can be
    // contained just here.
    char to_parse = 0;
    enum ParseState state = PARSE_SEARCH;
    _arg_value.state = ARG_STATE_NORMAL;

    // Loop through the preserved argument list.
    _parse_data.preserve_index = (_parse_data.preserve_index + 1) % PARSED_ARG_SAVE_COUNT;

    // Write to this position.
    char *write_pos = _parse_data.preserve_args[_parse_data.preserve_index];
    _arg_value.arg = write_pos;

    // End at this position
    char *write_pos_end = &(write_pos[PARSED_ARG_SIZE]);

    while (1 == 1) {
        if (write_pos >= write_pos_end) {
            stderrP("Argument length too long");
            _arg_value.state = ARG_STATE_ERR;
            return &_arg_value;
        }

        // --------------------------------------
        // Read the next character.
        if (_parse_data.put_back == PUT_BACK_EOF) {
            // Something had finished up an argument, meaning that the next
            //   call needs to report the EOF.
            // Tell the requestor that this is the end.
            _arg_value.state = ARG_STATE_END;
            // This NULL line may be omitted.
            _arg_value.arg = NULL;
            return &_arg_value;
        } else if (_parse_data.put_back != PUT_BACK_NOPE) {
            to_parse = (char) _parse_data.put_back;
            _parse_data.put_back = PUT_BACK_NOPE;
        } else {
            switch (_parse_data.source_type) {
                // case PARSE_SRC_NONE: initialized with put_back EOF which is handled above.
                case PARSE_SRC_ALLOCATED_STRING:
                case PARSE_SRC_STATIC_STRING:
                    to_parse = _parse_data.source.string.source_chars[_parse_data.source.string.str_pos++];
                    break;
                case PARSE_SRC_ARGV:
                    // argv must start in a parsable way.
                    to_parse = _parse_data.source.argv.argv[_parse_data.source.argv.argv_index][_parse_data.source.argv.str_pos++];
                    if (to_parse == 0) {
                        if (++_parse_data.source.argv.argv_index < _parse_data.source.argv.argc) {
                            _parse_data.source.argv.str_pos = 0;
                            to_parse = ' ';
                        }
                        // else we're at the end of the argv, so keep to_parse == 0.
                    }
                    break;

#ifdef USE_STREAMING_INPUT
                case PARSE_SRC_CLOSABLE_FD:
                case PARSE_SRC_EXTERN_FD:
                    // Starts without anything written.
                    if (_parse_data.source.fd.str_pos >= _parse_data.source.fd.length) {
                        _parse_data.source.fd.str_pos = 0;

                        // Write over the whole buffer
                        _parse_data.source.fd.length = read(
                                _parse_data.source.fd.fd,
                                _parse_data.source.fd.buffer,
                                INPUT_BUFFER_SIZE);
                    }
                    if (_parse_data.source.fd.length <= 0) {
                        // end of parsing.
                        // This may need an argument termination.
                        to_parse = 0;
                    } else {
                        to_parse = _parse_data.source.fd.buffer[_parse_data.source.fd.str_pos++];
                    }
                    break;
#endif
            }
        }

        if (to_parse == 0) {
            // end of input string marker.
            //   This might be before the full input buffer is read,
            //   but we'll still recognize it as a termination of the
            //   input.
            if (state == PARSE_SEARCH) {
                // Still searching for an argument, so return that this is
                // an EOF.
                _parse_data.put_back = PUT_BACK_EOF;
                continue;
            }

            // Some kind of argument is being parsed, so wrap it up &
            // tell the parser the next time around that it's EOF.
            // Should report an error if in ESCAPE mode.

            // Note that there's no need to increment the target here.
            *write_pos = 0;
            LOG(":: Parsed argument '");
            LOG(_arg_value.arg);
            LOG("'\n");
            // When we come back, return an EOF.
            _parse_data.put_back = PUT_BACK_EOF;
            return &_arg_value;
        }
        if (state >= PARSE_ESCAPE_PLAIN) {
            switch (to_parse) {
                case 'n':
                    *(write_pos++) = '\n';
                    break;
                case 'r':
                    *(write_pos++) = '\r';
                    break;
                case 't':
                    *(write_pos++) = '\t';
                    break;
                default:
                    *(write_pos++) = to_parse;
                    break;
            }
            state = state - PARSE_ESCAPE_PLAIN + PARSE_PLAIN;
            continue;
        }


        switch (to_parse) {
            // case 0: already checked above.
            case '\n':
                // Newline.
                if (state == PARSE_SEARCH) {
                    // Outside text.  This is like a ';' or '&&' depending on the setup.
                    // Use that value instead of writing it to the argument value.
                    _arg_value.arg = _ReplaceNewline;
                    LOG(":: Parsed newline as '");
                    LOG(_arg_value.arg);
                    LOG("'\n");
                    return &_arg_value;
                }
                if (state == PARSE_PLAIN) {
                    // Already parsing an argument.  Mark this as the end of the argument,
                    // and put it back so it can be correctly interpreted when parsing
                    // the next argument.
                    _parse_data.put_back = '\n';

                    // Note that there's no need to increment the target here.
                    *write_pos = 0;
                    LOG(":: Parsed argument '");
                    LOG(_arg_value.arg);
                    LOG("' before the newline\n");
                    return &_arg_value;
                } else {
                    // In quoted text, so keep it in the buffer.
                    *(write_pos++) = '\n';
                }
                break;
            case '\'':
                if (state == PARSE_SINGLE) {
                    // Inside a single quoted string already, so this ends it.
                    // Do not keep this character, and set state to looking at
                    // plain text.
                    state = PARSE_PLAIN;
                } else if (state == PARSE_DOUBLE) {
                    // Keep this character.
                    *(write_pos++) = '\'';
                } else {
                    // Either searching for the start of an argument, or already in
                    // an argument.  Either case, enter a quoted context.
                    _arg_value.state = ARG_STATE_ESCAPED;
                    state = PARSE_SINGLE;
                }
                break;
            case '"':
                if (state == PARSE_DOUBLE) {
                    // Inside a double quoted string already, so this ends it.
                    // Do not keep this character, and set state to looking at
                    // plain text.
                    state = PARSE_PLAIN;
                } else if (state == PARSE_SINGLE) {
                    // Keep this character.
                    *(write_pos++) = '"';
                } else {
                    // Either searching for the start of an argument, or already in
                    // an argument.  Either case, enter a quoted context.
                    _arg_value.state = ARG_STATE_ESCAPED;
                    state = PARSE_DOUBLE;
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                if (state == PARSE_PLAIN) {
                    // Inside an argument.  This ends it.
                    // No need to increment the write position.
                    *write_pos = 0;
                    LOG(":: Parsed argument '");
                    LOG(_arg_value.arg);
                    LOG("'\n");
                    return &_arg_value;
                } else if (state != PARSE_SEARCH) {
                    // Inside a quoted part.  Make it part of the argument.
                    *(write_pos++) = to_parse;
                }
                // else still searching for the argument to start.
                break;
            case '\\':
                // Standard string escaping.
                // Turn on the escape mode for the next time around.
                state = state + PARSE_ESCAPE_PLAIN - PARSE_PLAIN;
                break;
            default:
                if (state == PARSE_SEARCH) {
                    // looking for an argument start, and we found it.
                    state = PARSE_PLAIN;
                }
                // Always keep the character.
                *(write_pos++) = to_parse;
                break;
        }
    }
}



// ----------------------------------------------
// Creator

int args_setup_tokenizer(const int src_argc, char *src_argv[]) {
    int i;
    for (i = 0; i < PARSED_ARG_SAVE_COUNT; i++) {
        _parse_data.preserve_args[i] = malloc(sizeof(char) * PARSED_ARG_SIZE);
        if (_parse_data.preserve_args[i] == NULL) {
            stderrP(helper_str__malloc_failed);
            return 1;
        }
    }
    _parse_data.preserve_index = 0;
    _parse_data.put_back = PUT_BACK_NOPE;

    if (src_argc < 2) {
        // no argument worth noting.
        _parse_data.source_type = PARSE_SRC_NONE;
        // also, tell the argument parser to end immediately.
        _parse_data.put_back = PUT_BACK_EOF;
#ifdef USE_STREAMING_INPUT
    } else if (src_argc == 2 && strcmp("-", src_argv[1]) == 0) {
        // Read from stdin
        _parse_data.source_type = PARSE_SRC_EXTERN_FD;
        _parse_data.source.fd.fd = STDIN_FILENO;
    } else if (src_argc == 3 && strcmp("-f", src_argv[1]) == 0) {
        // Read from a file.
        LOG(":: Running script ");
        LOGLN(src_argv[2]);
        _parse_data.source_type = PARSE_SRC_CLOSABLE_FD;
        _parse_data.source.fd.fd = open(src_argv[2], O_RDONLY);
        if (_parse_data.source.fd.fd == -1) {
            stderrP("ERROR opening file ");
            stderrPLn(src_argv[2]);
            _parse_data.source_type = PARSE_SRC_NONE;
            return 1;
        }
#endif /* USE_STREAMING_INPUT */
    } else if (src_argc == 3 && strcmp("-c", src_argv[1]) == 0) {
        // Use the src_argv[2] as a full script.
        _parse_data.source_type = PARSE_SRC_STATIC_STRING;
        _parse_data.source.string.source_chars = (char *) src_argv[2];
        _parse_data.source.string.str_pos = 0;
    } else {
        // Process the args as though there's a single space between
        // each argument.
        // argv must start in a parsable way.  We know it's parsable, because src_arg >= 2.
        _parse_data.source_type = PARSE_SRC_ARGV;
        _parse_data.source.argv.argv = src_argv;
        _parse_data.source.argv.argc = src_argc;

        // argv index 1; skip over command name.
        _parse_data.source.argv.argv_index = 1;

        _parse_data.source.argv.str_pos = 0;
    }
#ifdef USE_STREAMING_INPUT
    if (_parse_data.source_type == PARSE_SRC_EXTERN_FD || _parse_data.source_type == PARSE_SRC_CLOSABLE_FD) {
        _parse_data.source.fd.buffer = malloc(sizeof(char) * INPUT_BUFFER_SIZE);
        if (_parse_data.source.fd.buffer == NULL) {
            stderrP(helper_str__malloc_failed);
            return 1;
        }
        _parse_data.source.fd.str_pos = 0;
        _parse_data.source.fd.length = 0;
    }
#endif /* USE_STREAMING_INPUT */

    return 0;
}
