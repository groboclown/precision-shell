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

/*
 * A simple "shell" that handles only simple file operations.
 * Intended to be used with the manipulation of file-only docker images.
 * At last count, this file compiled is huge, at 894 kilobytes.
 *
 * It is intended to be used as a non-interactive shell replacement.
 *
 * Usage:
 *   fshell (cmd)
 * Where:
 *   cmd: one of:
 *        cmd-part
 *        cmd-part "&&" cmd
 *        cmd-part ";" cmd
 *   cmd-part: one of:
 *        noop (file1 (file2 ...))
 *          does nothing.
 *        echo (str1 (str2 ...))
 *          prints each argument to STDOUT with a newline between them
 *        rm (file1 (file2 ...))
 *          unlinks (removes) each file, hardlink, or symlink argument
 *        rmdir (file1 (file2 ...))
 *          removes each directory.  They must be empty first
 *        mkdir (octal mode) (file1 (file2 ...))
 *          creates the directories with the file mode
 *        chmod (octal mode) (file1 (file2 ...))
 *          changes the file mode for each file, directory, or symlink argument
 *        chown (uid) (gid) (file1 (file2 ...))
 *          changes the owner and group ID for each file, directory, or symlink argument
 *        ln-s (src file) (dest file)
 *          creates a symbolic link named dest file, pointing to src file.
 *        ln-h (src file) (dest file)
 *          creates a hard link named dest file, pointing to src file.
 *
 * Commands like "cp" and "mkdir" are not supported, because those should be done through
 * Docker ADD and COPY instructions.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include "output.h"
#include "commands.h"

// This is turned in on the makefile for the debug build.
// #define DEBUG 1

#define PARSE_SEARCH 0
#define PARSE_PLAIN  1
#define PARSE_DOUBLE 2
#define PARSE_SINGLE 3
#define PARSE_END 4

static char *EMPTY_STR = "";

static char **_argv = NULL;
static int _argc = 0;
static int _argi = 0;

const char *advance() {
    if (_argi < _argc) {
        return _argv[_argi++];
    }
    return NULL;
}


int main(const int srcArgc, char *srcArgv[]) {
    int ret = 0;
    _argc = srcArgc;
    _argv = srcArgv;
    _argi = 1;

    if (srcArgc > 1 && strcmp("-c", srcArgv[1]) == 0) {
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

    ret = runCommands(&advance);

    // not needed, but we're nice.
    if (_argv != srcArgv) {
        free(_argv);
    }

    return ret;
}
