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
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "output.h"
#include "args.h"

// FD marker for non-free-able buffer.
#define ARGV_INPUT -2
#define INPUT_FD_FREED -1

// Some standard things.
#define DEFAULT_PARSED_ARG_SIZE    1000
#define DEFAULT_INPUT_BUFFER_SIZE  4096

// Parse states
#define PARSE_SEARCH 0
#define PARSE_PLAIN  1
#define PARSE_DOUBLE 2
#define PARSE_SINGLE 3
#define PARSE_END    4


// When a newline character is encountered, the
// parsed arg points to this string instead of where
// the newline happens.
static const char *_ReplaceNewline = ";";

// Buffers:
//    size = allocated size (cannot exceed this length).
//    len = number of usable values.

static const char **_parsedArgs = NULL;
static int _parsedArgPos = 0;
static int _parsedArgLen = 0;
static int _parsedArgSize = 0;
static int _parsedArgAllocated = 0;

static int _inputFD = INPUT_FD_FREED;
static char *_inputBuffer = NULL;
static int _inputBufferLen = 0;
static int _inputBufferSize = 0;
static int _inputBufferAllocated = 0;

static int _inputBufferLastArgStart = 0;
static int _inputBufferSource = 0;
static int _inputBufferTarget = 0;
static int _parseState = PARSE_SEARCH;


void tokenizeBuffer(int);

// ---------------------------------------------
// Close function

int closeTokenizer() {
    int ret = 0;
    if (_inputBuffer != NULL && _inputBufferAllocated) {
        free(_inputBuffer);
        _inputBuffer = NULL;
        _inputBufferLen = _inputBufferSize = 0;
    }
    if (_parsedArgs != NULL && _parsedArgAllocated) {
        free(_parsedArgs);
        _parsedArgs = NULL;
        _parsedArgSize = _parsedArgLen = 0;
    }
    if (_inputFD != ARGV_INPUT && _inputFD != INPUT_FD_FREED) {
        ret = close(_inputFD);
        _inputFD = INPUT_FD_FREED;
    }
    return ret;
}

// ----------------------------------------------
// Tokenize functions

/** Everything is already parsed. */
const char *advancePreParsed() {
    if (_parsedArgPos < _parsedArgLen) {
        return _parsedArgs[_parsedArgPos++];
    }
    return NULL;
}

const char *advanceToken() {
    int i, j;
    int closed = 0;

    // Read from the FD -> the buffer -> the parser args.
    const char *ret = advancePreParsed();
    if (ret == NULL && _inputFD != INPUT_FD_FREED) {
        // Read the buffer, tokenize the buffer, then advance parser args.
        
        // The input buffer can contain the left-over cruft from the last
        //   read, so we need to keep that.
        //   This shuffles over the buffer, so that last arg start is now 0,
        //   the source is now based on 0, and reading continues after the length.
        // Also, after this operation, the parsedArg stuff could be pointing at
        //   invalid locations, so those are reset.
        _parsedArgPos = _parsedArgLen = 0;
        for (j = 0, i = _inputBufferLastArgStart; i < _inputBufferLen; i++, j++) {
            _inputBuffer[j] = _inputBuffer[i];
        }
        // j <- where the reading of new data can continue.
        j = _inputBufferLen - _inputBufferLastArgStart;
        if (j >= _inputBufferSize) {
            // The last argument is bigger than the buffer size.
            //   That's a problem, it means we don't have enough memory allocated
            //   to handle the user request.
            // TODO add some error messages?
            return NULL;
        }
        _inputBufferSource -= _inputBufferLastArgStart;
        _inputBufferTarget -= _inputBufferLastArgStart;
        _inputBufferLastArgStart = 0;
        i = read(_inputFD, &(_inputBuffer[j]), _inputBufferSize - j);
        if (i <= 0) {
            i = close(_inputFD);
            // TODO how to handle errors?
            _inputFD = INPUT_FD_FREED;
            i = 0;
            closed = 1;
        }
        _inputBufferLen = j + i;

        tokenizeBuffer(closed);
        ret = advancePreParsed();
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
 * _inputBufferLastArgStart variable manages where it last left off
 * scanning for tokens.
 * 
 * _parsedArgPos and _parsedArgLen are expected to be 0 when this starts, but it
 * will still work even if they aren't.
 */
void tokenizeBuffer(int isComplete) {
    while (
            _parsedArgLen < _parsedArgSize
            && _inputBufferSource < _inputBufferLen
            && _parseState != PARSE_END
    ) {
        switch (_inputBuffer[_inputBufferSource]) {
            case 0:
                // null - end of input string
                //   This might be before the full input buffer is read,
                //   but we'll still recognize it as a termination of the
                //   input.
                //   Note that there's no need to increment the target here.
                _inputBuffer[_inputBufferTarget] = 0;
                // Terminate the loop
                _parseState = PARSE_END;
                break;
            case '\n':
                if (_parseState == PARSE_PLAIN || _parseState == PARSE_SEARCH) {
                    // Newline - this is like a ';' or '&&' depending on the
                    //   setup.  In order to avoid expanding the one character
                    //   into 2 or 3 characters, we point the parsedArg at a
                    //   separate, static string.
                    _parsedArgs[_parsedArgLen++] = _ReplaceNewline;

                    // In case this is part of a plain search, force this to
                    //   be the end of the value.
                    _inputBuffer[_inputBufferTarget++] = 0;

                    // The last arg start shouldn't advance yet, because the
                    // state is search.  However, for safety, set it here.
                    _inputBufferLastArgStart = _inputBufferTarget;
                    _parseState = PARSE_SEARCH;
                } else {
                    // In a string, so keep it in the buffer.
                    _inputBuffer[_inputBufferTarget++] = '\n';
                }
                break;
            case '\'':
                if (_parseState == PARSE_SEARCH) {
                    // enter a string.
                    // Point the argument start to the next character,
                    //   which will be where targetPos is actively pointing.
                    //   The target position does not move.
                    _parsedArgs[_parsedArgLen++] = &(_inputBuffer[_inputBufferTarget]);
                    _inputBufferLastArgStart = _inputBufferTarget;
                    _parseState = PARSE_SINGLE;
                } else if (_parseState == PARSE_SINGLE) {
                    // Exit a string.
                    // This will always be considered as though there is
                    //   more text in this argument, so ignore the
                    //   character by stepping over it.
                    _parseState = PARSE_PLAIN;
                } else if (_parseState == PARSE_PLAIN) {
                    // Encountered a quote in the middle of text.
                    // Step over it, but put us in string mode.
                    _parseState = PARSE_SINGLE;
                } else {
                    // Inside a parsing state where we want to keep this character.
                    _inputBuffer[_inputBufferTarget++] = '\'';
                }
                break;
            case '"':
                if (_parseState == PARSE_SEARCH) {
                    // enter a string.
                    // Point the argument start to the next character,
                    //   which will be where targetPos is actively pointing.
                    _parsedArgs[_parsedArgLen++] = &(_inputBuffer[_inputBufferTarget]);
                    _inputBufferLastArgStart = _inputBufferTarget;
                    _parseState = PARSE_DOUBLE;
                } else if (_parseState == PARSE_DOUBLE) {
                    // Exit a string.
                    // This will always be considered as though there is
                    //   more text in this argument, so ignore the
                    //   character by stepping over it.
                    _parseState = PARSE_PLAIN;
                } else if (_parseState == PARSE_PLAIN) {
                    // Encountered a quote in the middle of text.
                    // Step over it, but put us in string mode.
                    _parseState = PARSE_DOUBLE;
                } else {
                    // Inside a parsing state where we want to keep this character.
                    _inputBuffer[_inputBufferTarget++] = '"';
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                if (_parseState == PARSE_PLAIN) {
                    // inside an argument.  This ends it.
                    _inputBuffer[_inputBufferTarget++] = 0;
                    // Move the arg parsing to the character after the space.
                    _inputBufferLastArgStart = _inputBufferTarget;
                    _parseState = PARSE_SEARCH;
                    LOG(":: Parsed argument '");
                    LOG(_parsedArgs[_parsedArgLen-1]);
                    LOG("'\n");
                } else if (_parseState != PARSE_SEARCH) {
                    // Inside a quoted part.  Make it part of the argument.
                    _inputBuffer[_inputBufferTarget++] = _inputBuffer[_inputBufferSource];
                }
                break;
            case '\\':
                // standard string escaping.
                if (_parseState == PARSE_SEARCH) {
                    // looking for an argument start, and we found it.
                    _parsedArgs[_parsedArgLen++] = &(_inputBuffer[_inputBufferTarget]);
                    _inputBufferLastArgStart = _inputBufferTarget;
                    _parseState = PARSE_PLAIN;
                }
                if (_inputBufferSource < _inputBufferLen) {
                    // Ensure we don't look past the read length.
                    _inputBufferSource++;
                    switch (_inputBuffer[_inputBufferSource]) {
                        case 0:
                            // Whoops - ran over.  Formally, a problem
                            //   with the input, but we'll silently ignore it.
                            _inputBufferSource--;
                            break;
                        case 'n':
                            _inputBuffer[_inputBufferTarget++] = '\n';
                            break;
                        case 'r':
                            _inputBuffer[_inputBufferTarget++] = '\r';
                            break;
                        case 't':
                            _inputBuffer[_inputBufferTarget++] = '\t';
                            break;
                        default:
                            _inputBuffer[_inputBufferTarget++] = _inputBuffer[_inputBufferSource];
                            break;
                    }
                    LOG(":: Escaped ");
                    LOG1(&(_inputBuffer[_inputBufferSource]));
                    LOG("\n");
                }
                break;
            default:
                if (_parseState == PARSE_SEARCH) {
                    // looking for an argument start, and we found it.
                    _parsedArgs[_parsedArgLen++] = &(_inputBuffer[_inputBufferTarget]);
                    _inputBufferLastArgStart = _inputBufferTarget;
                    _parseState = PARSE_PLAIN;
                }
                // Always keep the character.
                _inputBuffer[_inputBufferTarget++] = _inputBuffer[_inputBufferSource];
                break;
        }
        _inputBufferSource++;
    }

    // Adjust some of the parsing values to accomodate that the last thing
    // read may be an incomplete argument.
    if (isComplete) {
        _inputBuffer[_inputBufferTarget] = 0;
        _inputBufferLastArgStart = _inputBufferLen;
        if (_parseState != PARSE_SEARCH) {
            _parsedArgLen++;
        }
#ifdef DEBUG
        if (_parsedArgLen > 0) {
            LOG(":: Parsed last argument '");
            LOG(_parsedArgs[_parsedArgLen-1]);
            LOG("'\n");
        } else {
            LOG(":: No last argument to parse\n");
        }
#endif
    } else if (_parseState == PARSE_SEARCH) {
        _inputBufferLastArgStart = _inputBufferLen;
    } else if (_parsedArgLen > 0) {
        _parsedArgLen--;
    }
}



// ----------------------------------------------
// Creator

int setupTokenizer(const int srcArgc, char *srcArgv[]) {
    if (srcArgc < 2) {
        // no argument worth noting.
        _parsedArgPos = 0;
        _parsedArgLen = 0;
    } else if (
        (srcArgc == 2 && strcmp("-", srcArgv[1]) == 0)
        || (srcArgc == 3 && strcmp("-f", srcArgv[1]) == 0)
    ) {
        // Read & parse from an FD.

        // Setup the common data.
        _parsedArgs = malloc(sizeof(char *) * DEFAULT_PARSED_ARG_SIZE);
        _parsedArgAllocated = 1;
        if (_parsedArgs == NULL) {
            stderrP("ERROR malloc failed\n");
            return 1;
        }
        _parsedArgSize = DEFAULT_PARSED_ARG_SIZE;

        _inputBuffer = malloc(sizeof(char *) * DEFAULT_INPUT_BUFFER_SIZE);
        _inputBufferAllocated = 1;
        if (_inputBuffer == NULL) {
            stderrP("ERROR malloc failed\n");
            return 1;
        }
        _inputBufferSize = DEFAULT_INPUT_BUFFER_SIZE;

        // Per-argument specialization.
        if (srcArgc == 2 && strcmp("-", srcArgv[1]) == 0) {
            // Use stdin as commands.
            _inputFD = STDIN_FILENO;
        } else if (srcArgc == 3 && strcmp("-f", srcArgv[1]) == 0) {
            LOG(":: Running script ");
            LOGLN(srcArgv[2]);
            _inputFD = open(srcArgv[2], O_RDONLY);
            if (_inputFD == -1) {
                stderrP("ERROR opening file ");
                stderrPLn(srcArgv[2]);
                return 1;
            }
        } else {
            // ERROR STATE - invalid program state
            return 15;
        }
    } else if (srcArgc == 3 && strcmp("-c", srcArgv[1]) == 0) {
        // Use the srcArgv[2] as a full script.
        _inputBuffer = (char *) srcArgv[2];
        _inputBufferSize
            = _inputBufferLen
            = _inputBufferLastArgStart
            = strlen(_inputBuffer);

        // With an input of "a b", the arg count is 2 (len / 2 + 1)
        // With an input of "a b c", the arg count is 3 (len / 2 + 1)
        // These are maximum argument counts.
        _parsedArgSize = (_inputBufferSize / 2) + 1;
        _parsedArgs = malloc(sizeof(char **) * _parsedArgSize);
        _parsedArgAllocated = 1;
        
        _inputFD = INPUT_FD_FREED;

        // Just tokenize one time.
        tokenizeBuffer(1);
    } else {
        // process the args as-is.
        _parsedArgPos = 1;
        _parsedArgLen = srcArgc;
        _parsedArgs = (const char **) srcArgv;
    }
    return 0;
}
