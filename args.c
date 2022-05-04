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

#include <stdlib.h>
#include <string.h>
#include "output.h"
#include "general.h"
#include "args.h"

#define PARSE_SEARCH 0
#define PARSE_PLAIN  1
#define PARSE_DOUBLE 2
#define PARSE_SINGLE 3
#define PARSE_END 4

static char **_argv = NULL;
static int _argc = 0;
static int _argi = 0;
static int _allocated = 0;


const char *advanceToken() {
    if (_argi < _argc) {
        return _argv[_argi++];
    }
    return NULL;
}


int setupTokenizer(const int srcArgc, char *srcArgv[]) {
    _argc = srcArgc;
    _argv = srcArgv;
    _argi = 1;
    _allocated = 0;

    if (srcArgc > 1 && strequal("-c", srcArgv[1])) {
        // Invoked like "/bin/sh -c" style execution.
        // Just ignore the "-c".
        LOG(":: ignoring initial -c\n");

        if (srcArgc == 3) {
            // minimum argument size is 1 character.
            // "a b" has 2 arguments
            // "a b c" has 3 arguments.
            int maxArgs = (strlen(srcArgv[2]) / 2) + 1;
            int srcPos = 0;
            int targetPos = 0;
            char *arg = srcArgv[2];
            int i;
            _argc = 2;
            _argi = 2;

            // This signal means we need to do our own argument
            // parsing.  The Docker RUN command does not perform
            // argument splitting.
            _argv = malloc(sizeof(char *) * maxArgs);
            if (_argv == NULL) {
                stderrP("ERROR malloc failed\n");
                return 1;
            }
            _allocated = 1;
            _argv[0] = &(srcArgv[0][0]);
            _argv[1] = &(srcArgv[1][0]);

            // i: parse state
            i = 0;
            while (_argc < maxArgs && i != PARSE_END) {
                switch (arg[srcPos]) {
                    case 0:
                        // null - end of input string
                        arg[targetPos] = 0;
                        // Terminate the loop
                        i = PARSE_END;
                        break;
                    case '\'':
                        if (i == PARSE_SEARCH) {
                            // enter a string.
                            // Point the argument start to the next character,
                            //   which will be where targetPos is actively pointing.
                            _argv[_argc++] = &(arg[targetPos]);
                            i = PARSE_SINGLE;
                        } else if (i == PARSE_SINGLE) {
                            // Exit a string.
                            // This will always be considered as though there is
                            // more text in this argument, so ignore the
                            // character by stepping over it.
                            i = PARSE_PLAIN;
                        } else if (i == PARSE_PLAIN) {
                            // Encountered a quote in the middle of text.
                            // Step over it.
                            i = PARSE_SINGLE;
                        } else {
                            // In another quoting thing that requires this character
                            // to be kept.
                            arg[targetPos++] = arg[srcPos];
                        }
                        break;
                    case '"':
                        if (i == PARSE_SEARCH) {
                            // enter a string.
                            // Point the argument start to the next character,
                            //   which will be where targetPos is actively pointing.
                            _argv[_argc++] = &(arg[targetPos]);
                            i = PARSE_DOUBLE;
                        } else if (i == PARSE_DOUBLE) {
                            // Exit a string.
                            // This will always be considered as though there is
                            // more text in this argument, so ignore the
                            // character by stepping over it.
                            i = PARSE_PLAIN;
                        } else if (i == PARSE_PLAIN) {
                            // Encountered a quote in the middle of text.
                            // Step over it.
                            i = PARSE_DOUBLE;
                        } else {
                            // In another quoting thing that requires this character
                            // to be kept.
                            arg[targetPos++] = arg[srcPos];
                        }
                        break;
                    // Note: For the moment, '\n' is just whitespace.  If we are
                    //   not in a quote, then this should be considered a separator.
                    //   In that case, argv[argc++] would need to point to a constant
                    //   separator string.
                    case '\n':
                    case '\r':
                    case ' ':
                    case '\t':
                        if (i == PARSE_PLAIN) {
                            // inside an argument.  This ends it.
                            arg[targetPos++] = 0;
                            i = PARSE_SEARCH;
                            LOG(":: Parsed argument '");
                            LOG(_argv[_argc-1]);
                            LOG("'\n");
                        } else if (i != PARSE_SEARCH) {
                            // Inside a quoted part.  Make it part of the argument.
                            arg[targetPos++] = arg[srcPos];
                        }
                        break;
                    case '\\':
                        // standard string escaping.
                        if (i == PARSE_SEARCH) {
                            // looking for an argument start, and we found it.
                            _argv[_argc++] = &(arg[targetPos]);
                            i = PARSE_PLAIN;
                        }
                        srcPos++;
                        switch (arg[srcPos]) {
                            case 0:
                                // Whoops - ran over.  Formally, a problem
                                //   with the input, but we'll silently ignore it.
                                srcPos--;
                                break;
                            case 'n':
                                arg[targetPos++] = '\n';
                                break;
                            case 'r':
                                arg[targetPos++] = '\r';
                                break;
                            case 't':
                                arg[targetPos++] = '\t';
                                break;
                            default:
                                arg[targetPos++] = arg[srcPos];
                                break;
                        }
                        LOG(":: Escaped ");
                        LOG1(&(arg[srcPos]));
                        LOG("\n");
                        break;
                    default:
                        if (i == PARSE_SEARCH) {
                            // looking for an argument start, and we found it.
                            _argv[_argc++] = &(arg[targetPos]);
                            i = PARSE_PLAIN;
                        }
                        // Always keep the character.
                        arg[targetPos++] = arg[srcPos];
                        break;
                }
                srcPos++;
            }
        }
    }
    return 0;
}

int closeTokenizer() {
    // not needed, but we're nice.
    if (_allocated == 1) {
        free(_argv);
        _argv = NULL;
    }
    return 0;
}
