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


// FD marker for non-free-able buffer.
#ifdef USE_STREAMING_INPUT
#define INPUT_FD_ARGV -2
#define INPUT_FD_FREED -1
#endif


// Configurable argument and buffer sizes.
//   By making these configurable, it means the tool is better
//   able to be sized into memory constrained environments.
#ifndef PARSED_ARG_SIZE
#define PARSED_ARG_SIZE    1000
#endif
#ifndef INPUT_BUFFER_SIZE
#define INPUT_BUFFER_SIZE  4096
#endif

// Parse states
enum ParseState {
    PARSE_SEARCH,
    PARSE_PLAIN,
    PARSE_DOUBLE,
    PARSE_SINGLE,
    PARSE_END
};


// When a newline character is encountered, the
// parsed arg points to this string instead of where
// the newline happens.  In Bash, the '-e' flag will
// turn this into '&&'.
static const char *_ReplaceNewline = ";";

// Buffers:
//    size = allocated size (cannot exceed this length).
//    len = number of usable values.

static const char **_parsed_arg_list = NULL;
static int _parsed_arg_pos = 0;
static int _parsed_arg_len = 0;
static int _parsed_arg_size = 0;
static int _parsed_arg_allocated = 0;

#ifdef USE_STREAMING_INPUT
static int _input_fd = INPUT_FD_FREED;
#endif
static char *_input_chars = NULL;
static int _input_len = 0;
static int _input_size = 0;
static int _input_allocated = 0;

static int _input_last_arg_start_pos = 0;
static int _input_source_pos = 0;
static int _input_target_pos = 0;
static enum ParseState _parse_state = PARSE_SEARCH;


void args_tokenize_buffer(int);

// ---------------------------------------------
// Close function

int args_close_tokenizer() {
    int ret = 0;
    if (_input_chars != NULL && _input_allocated) {
        free(_input_chars);
        _input_chars = NULL;
        _input_len = _input_size = 0;
    }
    if (_parsed_arg_list != NULL && _parsed_arg_allocated) {
        free(_parsed_arg_list);
        _parsed_arg_list = NULL;
        _parsed_arg_size = _parsed_arg_len = 0;
    }
#ifdef USE_STREAMING_INPUT
    if (_input_fd != INPUT_FD_ARGV && _input_fd != INPUT_FD_FREED) {
        ret = close(_input_fd);
        _input_fd = INPUT_FD_FREED;
    }
#endif
    return ret;
}

// ----------------------------------------------
// Tokenize functions

/** Everything is already parsed. */
const char *args_advance_pre_parsed() {
    if (_parsed_arg_pos < _parsed_arg_len) {
        return _parsed_arg_list[_parsed_arg_pos++];
    }
    return NULL;
}

const char *args_advance_token() {
    int i, j;
    int closed = 0;

    // Read from the FD -> the buffer -> the parser args.
    const char *ret = args_advance_pre_parsed();
    if (ret == NULL
#ifdef USE_STREAMING_INPUT
        && _input_fd != INPUT_FD_FREED
#endif
    ) {

#ifdef USE_STREAMING_INPUT
        // Read the buffer, tokenize the buffer, then advance parser args.
        
        // The input buffer can contain the left-over cruft from the last
        //   read, so we need to keep that.
        //   This shuffles over the buffer, so that last arg start is now 0,
        //   the source is now based on 0, and reading continues after the length.
        // Also, after this operation, the parsedArg stuff could be pointing at
        //   invalid locations, so those are reset.
        _parsed_arg_pos = _parsed_arg_len = 0;
        for (j = 0, i = _input_last_arg_start_pos; i < _input_len; i++, j++) {
            _input_chars[j] = _input_chars[i];
        }
        // j <- where the reading of new data can continue.
        j = _input_len - _input_last_arg_start_pos;
        if (j >= _input_size) {
            // The last argument is bigger than the buffer size.
            //   That's a problem, it means we don't have enough memory allocated
            //   to handle the user request.
            // TODO add some error messages?
            return NULL;
        }
        _input_source_pos -= _input_last_arg_start_pos;
        _input_target_pos -= _input_last_arg_start_pos;
        _input_last_arg_start_pos = 0;
        i = read(_input_fd, &(_input_chars[j]), _input_size - j);
        if (i <= 0) {
            i = close(_input_fd);
            // TODO how to handle errors?
            _input_fd = INPUT_FD_FREED;
            i = 0;
            closed = 1;
        }
        _input_len = j + i;
#endif /* USE_STREAMING_INPUT */

        args_tokenize_buffer(closed);
        ret = args_advance_pre_parsed();
    }
    return ret;
}


/**
 * @brief tokenize the entire input buffer
 * 
 * This modifies the input buffer based on the parsed values, and for each
 * token discovered, it is added to the parsed arg list.  The modifications
 * made to the input buffer are guaranteed to never grow the size of it.
 * 
 * Because the input buffer can include a partial token at the end, the
 * _input_last_arg_start_pos variable manages where it last left off
 * scanning for tokens.
 * 
 * _parsed_arg_pos and _parsed_arg_len are expected to be 0 when this starts, but it
 * will still work even if they aren't.
 */
void args_tokenize_buffer(int isComplete) {
    while (
            _parsed_arg_len < _parsed_arg_size
            && _input_source_pos < _input_len
            && _parse_state != PARSE_END
    ) {
        switch (_input_chars[_input_source_pos]) {
            case 0:
                // null - end of input string
                //   This might be before the full input buffer is read,
                //   but we'll still recognize it as a termination of the
                //   input.
                //   Note that there's no need to increment the target here.
                _input_chars[_input_target_pos] = 0;
                // Terminate the loop
                _parse_state = PARSE_END;
                break;
            case '\n':
                if (_parse_state == PARSE_PLAIN || _parse_state == PARSE_SEARCH) {
                    // Newline - this is like a ';' or '&&' depending on the
                    //   setup.  In order to avoid expanding the one character
                    //   into 2 or 3 characters, we point the parsedArg at a
                    //   separate, static string.
                    _parsed_arg_list[_parsed_arg_len++] = _ReplaceNewline;

                    // In case this is part of a plain search, force this to
                    //   be the end of the value.
                    _input_chars[_input_target_pos++] = 0;

                    // The last arg start shouldn't advance yet, because the
                    // state is search.  However, for safety, set it here.
                    _input_last_arg_start_pos = _input_target_pos;
                    _parse_state = PARSE_SEARCH;

                    LOG(":: Parsed newline, which created argument '");
                    LOG(_parsed_arg_list[_parsed_arg_len-2]);
                    LOG("' and added '");
                    LOG(_parsed_arg_list[_parsed_arg_len-1]);
                    LOG("'\n");
                } else {
                    // In a string, so keep it in the buffer.
                    _input_chars[_input_target_pos++] = '\n';
                }
                break;
            case '\'':
                if (_parse_state == PARSE_SEARCH) {
                    // enter a string.
                    // Point the argument start to the next character,
                    //   which will be where targetPos is actively pointing.
                    //   The target position does not move.
                    _parsed_arg_list[_parsed_arg_len++] = &(_input_chars[_input_target_pos]);
                    _input_last_arg_start_pos = _input_target_pos;
                    _parse_state = PARSE_SINGLE;
                } else if (_parse_state == PARSE_SINGLE) {
                    // Exit a string.
                    // This will always be considered as though there is
                    //   more text in this argument, so ignore the
                    //   character by stepping over it.
                    _parse_state = PARSE_PLAIN;
                } else if (_parse_state == PARSE_PLAIN) {
                    // Encountered a quote in the middle of text.
                    // Step over it, but put us in string mode.
                    _parse_state = PARSE_SINGLE;
                } else {
                    // Inside a parsing state where we want to keep this character.
                    _input_chars[_input_target_pos++] = '\'';
                }
                break;
            case '"':
                if (_parse_state == PARSE_SEARCH) {
                    // enter a string.
                    // Point the argument start to the next character,
                    //   which will be where targetPos is actively pointing.
                    _parsed_arg_list[_parsed_arg_len++] = &(_input_chars[_input_target_pos]);
                    _input_last_arg_start_pos = _input_target_pos;
                    _parse_state = PARSE_DOUBLE;
                } else if (_parse_state == PARSE_DOUBLE) {
                    // Exit a string.
                    // This will always be considered as though there is
                    //   more text in this argument, so ignore the
                    //   character by stepping over it.
                    _parse_state = PARSE_PLAIN;
                } else if (_parse_state == PARSE_PLAIN) {
                    // Encountered a quote in the middle of text.
                    // Step over it, but put us in string mode.
                    _parse_state = PARSE_DOUBLE;
                } else {
                    // Inside a parsing state where we want to keep this character.
                    _input_chars[_input_target_pos++] = '"';
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                if (_parse_state == PARSE_PLAIN) {
                    // inside an argument.  This ends it.
                    _input_chars[_input_target_pos++] = 0;
                    // Move the arg parsing to the character after the space.
                    _input_last_arg_start_pos = _input_target_pos;
                    _parse_state = PARSE_SEARCH;
                    LOG(":: Parsed argument '");
                    LOG(_parsed_arg_list[_parsed_arg_len-1]);
                    LOG("'\n");
                } else if (_parse_state != PARSE_SEARCH) {
                    // Inside a quoted part.  Make it part of the argument.
                    _input_chars[_input_target_pos++] = _input_chars[_input_source_pos];
                }
                break;
            case '\\':
                // standard string escaping.
                if (_parse_state == PARSE_SEARCH) {
                    // looking for an argument start, and we found it.
                    _parsed_arg_list[_parsed_arg_len++] = &(_input_chars[_input_target_pos]);
                    _input_last_arg_start_pos = _input_target_pos;
                    _parse_state = PARSE_PLAIN;
                }
                if (_input_source_pos < _input_len) {
                    // Ensure we don't look past the read length.
                    _input_source_pos++;
                    switch (_input_chars[_input_source_pos]) {
                        case 0:
                            // Whoops - ran over.  Formally, a problem
                            //   with the input, but we'll silently ignore it.
                            _input_source_pos--;
                            break;
                        case 'n':
                            _input_chars[_input_target_pos++] = '\n';
                            break;
                        case 'r':
                            _input_chars[_input_target_pos++] = '\r';
                            break;
                        case 't':
                            _input_chars[_input_target_pos++] = '\t';
                            break;
                        default:
                            _input_chars[_input_target_pos++] = _input_chars[_input_source_pos];
                            break;
                    }
                    LOG(":: Escaped ");
                    LOG1(&(_input_chars[_input_source_pos]));
                    LOG("\n");
                }
                break;
            default:
                if (_parse_state == PARSE_SEARCH) {
                    // looking for an argument start, and we found it.
                    _parsed_arg_list[_parsed_arg_len++] = &(_input_chars[_input_target_pos]);
                    _input_last_arg_start_pos = _input_target_pos;
                    _parse_state = PARSE_PLAIN;
                }
                // Always keep the character.
                _input_chars[_input_target_pos++] = _input_chars[_input_source_pos];
                break;
        }
        _input_source_pos++;
    }

    // Adjust some of the parsing values to accomodate that the last thing
    // read may be an incomplete argument.
    if (isComplete) {
        _input_chars[_input_target_pos] = 0;
        _input_last_arg_start_pos = _input_len;
        if (_parse_state != PARSE_SEARCH) {
            _parsed_arg_len++;
        }
#ifdef DEBUG
        if (_parsed_arg_len > 0) {
            LOG(":: Parsed last argument '");
            LOG(_parsed_arg_list[_parsed_arg_len-1]);
            LOG("'\n");
        } else {
            LOG(":: No last argument to parse\n");
        }
#endif
    } else if (_parse_state == PARSE_SEARCH) {
        _input_last_arg_start_pos = _input_len;
    } else if (_parsed_arg_len > 0) {
        _parsed_arg_len--;
    }
}



// ----------------------------------------------
// Creator

int args_setup_tokenizer(const int srcArgc, char *srcArgv[]) {
    if (srcArgc < 2) {
        // no argument worth noting.
        _parsed_arg_pos = 0;
        _parsed_arg_len = 0;
#ifdef USE_STREAMING_INPUT
    } else if (
        (srcArgc == 2 && strcmp("-", srcArgv[1]) == 0)
        || (srcArgc == 3 && strcmp("-f", srcArgv[1]) == 0)
    ) {
        // Read & parse from an FD.

        // Setup the common data.
        _parsed_arg_list = malloc(sizeof(char *) * PARSED_ARG_SIZE);
        _parsed_arg_allocated = 1;
        if (_parsed_arg_list == NULL) {
            stderrP("ERROR malloc failed\n");
            return 1;
        }
        _parsed_arg_size = PARSED_ARG_SIZE;

        _input_chars = malloc(sizeof(char) * INPUT_BUFFER_SIZE);
        _input_allocated = 1;
        if (_input_chars == NULL) {
            stderrP("ERROR malloc failed\n");
            return 1;
        }
        _input_size = INPUT_BUFFER_SIZE;

        // Per-argument specialization.
        if (srcArgc == 2 && strcmp("-", srcArgv[1]) == 0) {
            // Use stdin as commands.
            _input_fd = STDIN_FILENO;
        } else if (srcArgc == 3 && strcmp("-f", srcArgv[1]) == 0) {
            LOG(":: Running script ");
            LOGLN(srcArgv[2]);
            _input_fd = open(srcArgv[2], O_RDONLY);
            if (_input_fd == -1) {
                stderrP("ERROR opening file ");
                stderrPLn(srcArgv[2]);
                return 1;
            }
        } else {
            // ERROR STATE - invalid program state
            return 15;
        }
#endif /* USE_STREAMING_INPUT */
    } else if (srcArgc == 3 && strcmp("-c", srcArgv[1]) == 0) {
        // Use the srcArgv[2] as a full script.
        _input_chars = (char *) srcArgv[2];
        _input_size
            = _input_len
            = _input_last_arg_start_pos
            = strlen(_input_chars);

        // With an input of "a b", the arg count is 2 (len / 2 + 1)
        // With an input of "a b c", the arg count is 3 (len / 2 + 1)
        // These are maximum argument counts.
        _parsed_arg_size = (_input_size / 2) + 1;
        _parsed_arg_list = malloc(sizeof(char *) * _parsed_arg_size);
        _parsed_arg_allocated = 1;
        
#ifdef USE_STREAMING_INPUT
        _input_fd = INPUT_FD_FREED;
#endif

        // Just tokenize one time.
        args_tokenize_buffer(1);
    } else {
        // process the args as-is.
        _parsed_arg_pos = 1;
        _parsed_arg_len = srcArgc;
        _parsed_arg_list = (const char **) srcArgv;
    }
    return 0;
}
