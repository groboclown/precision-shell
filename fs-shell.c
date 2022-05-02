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
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

// This is turned in on the makefile for the debug build.
// #define DEBUG 1

#define CMD_ERR   0
#define CMD_NOOP  1
#define CMD_ECHO  2
#define CMD_RM    3
#define CMD_RMDIR 4
#define CMD_MKDIR 5
#define CMD_CHMOD 6
#define CMD_CHOWN 7
#define CMD_HLINK 8
#define CMD_SLINK 9
#define CMD_PAUSE 10

#define PARSE_SEARCH 0
#define PARSE_PLAIN  1
#define PARSE_DOUBLE 2
#define PARSE_SINGLE 3
#define PARSE_END 4

#define STDOUT 1
#define STDERR 2

static char *EMPTY_STR = "";

void emptySignalHandler(int signal) {
#ifdef DEBUG
    write(STDOUT, ":: handled signal\n", 18);
#endif
}


int main(const int srcArgc, char *srcArgv[]) {
    int argc = srcArgc;
    char **argv = srcArgv;
    int i = 1;
    long val1 = 0;
    long val2 = 0;
    int err = 0;
    int errCount = 0;
    int cmd = CMD_ERR;
    char *arg = EMPTY_STR;
    const char *cmdName = EMPTY_STR;
    sigset_t signalSet;

    int isCmdStart = 1;

    if (argc > 1 && strcmp("-c", argv[1]) == 0) {
        // Invoked like "/bin/sh -c" style execution.
        // Just ignore the "-c".
#ifdef DEBUG
        write(STDOUT, ":: ignoring initial -c\n", 24);
#endif

        if (argc == 3) {
            int maxArgs = strlen(argv[2]);
            int srcPos = 0;
            int targetPos = 0;
            argc = 2;
            arg = srcArgv[2];

            // This signal means we need to do our own argument
            // parsing.  The Docker RUN command does not perform
            // argument splitting.
            argv = malloc(sizeof(char *) * maxArgs);
            if (argv == NULL) {
                write(STDERR, "ERROR malloc failed\n", 20);
                return 1;
            }
            argv[0] = &(srcArgv[0][0]);
            argv[1] = &(srcArgv[1][0]);

            // i: parse state
            i = 0;
            while (argc < maxArgs && i != PARSE_END) {
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
                            argv[argc] = &(arg[targetPos]);
                            argc++;
                            i = PARSE_SINGLE;
                        } else if (i == PARSE_SINGLE) {
                            // Exit a string.
                            // This will always be considered as though there is
                            // more text in this argument, so ignore the
                            // character by stepping over it.
                            i = PARSE_PLAIN;
                        } else {
                            // Encountered a quote in the middle of text.
                            // Step over it.
                            i = PARSE_SINGLE;
                        }
                        break;
                    case '"':
                        if (i == PARSE_SEARCH) {
                            // enter a string.
                            // Point the argument start to the next character,
                            //   which will be where targetPos is actively pointing.
                            argv[argc] = &(arg[targetPos]);
                            argc++;
                            i = PARSE_DOUBLE;
                        } else if (i == PARSE_DOUBLE) {
                            // Exit a string.
                            // This will always be considered as though there is
                            // more text in this argument, so ignore the
                            // character by stepping over it.
                            i = PARSE_PLAIN;
                        } else {
                            // Encountered a quote in the middle of text.
                            // Step over it.
                            i = PARSE_DOUBLE;
                        }
                        break;
                    case ' ':
                        if (i == PARSE_PLAIN) {
                            // inside an argument.  This ends it.
                            arg[targetPos++] = 0;
                            i = PARSE_SEARCH;
#ifdef DEBUG
                            write(STDOUT, ":: Parsed argument '", 20);
                            write(STDOUT, argv[argc-1], strlen(argv[argc-1]));
                            write(STDOUT, "'\n", 2);
#endif
                        } else if (i != PARSE_SEARCH) {
                            // Inside a quoted part.  Make it part of the argument.
                            arg[targetPos++] = arg[srcPos];
                        }
                        break;
                    case '\\':
                        // standard string escaping.
                        if (i == PARSE_SEARCH) {
                            // looking for an argument start, and we found it.
                            argv[argc] = &(arg[targetPos]);
                            argc++;
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
#ifdef DEBUG
                        write(STDOUT, ":: Escaped ", 11);
                        write(STDOUT, &(arg[srcPos]), 1);
                        write(STDOUT, "\n", 1);
#endif
                        break;
                    default:
                        if (i == PARSE_SEARCH) {
                            // looking for an argument start, and we found it.
                            argv[argc] = &(arg[targetPos]);
                            argc++;
                            i = PARSE_PLAIN;
                        }
                        // Always keep the character.
                        arg[targetPos++] = arg[srcPos];
                        break;
                }
                srcPos++;
            }
        }
        i = 2;
    }

    // Inefficient loop in terms of performance, but condenses the code.

    while (i < argc) {
        arg = argv[i];

        // ----------------------------------------
        // Command start check.
        if (isCmdStart == 1) {
            errCount = 0;
            isCmdStart = 0;
            cmdName = arg;
#ifdef DEBUG
            write(STDOUT, ":: start command '", 18);
            write(STDOUT, arg, strlen(arg));
            write(STDOUT, "'\n", 2);
#endif

            // ends the loop prematurely, so need to explicitly
            // increment i.
            i++;

            // Command name check is done here.
            if (strcmp("noop", arg) == 0) {
                cmd = CMD_NOOP;
            } else if (strcmp("echo", arg) == 0) {
                cmd = CMD_ECHO;
            } else if (strcmp("rm", arg) == 0) {
                cmd = CMD_RM;
            } else if (strcmp("rmdir", arg) == 0) {
                cmd = CMD_RMDIR;
            } else if (strcmp("mkdir", arg) == 0) {
                cmd = CMD_MKDIR;
                // extra argument load for the permissions
                if (argc > i) {
                    // mkdir (octal mode)
                    val1 = strtoul(argv[i], (char **)NULL, 8);
                    // Note: octal max value.
                    if (errno != 0 || val1 < 0 || val1 > 07777) {
                        cmd = CMD_ERR;
                    } else {
                        i++;
                    }
                }
            } else if (strcmp("chmod", arg) == 0) {
                cmd = CMD_CHMOD;
                // extra argument load
                if (argc > i) {
                    // chmod (octal mode)
                    val1 = strtoul(argv[i], (char **)NULL, 8);
                    // Note: octal max value.
                    if (errno != 0 || val1 < 0 || val1 > 07777) {
                        cmd = CMD_ERR;
                    } else {
                        i++;
                    }
                }
            } else if (strcmp("chown", arg) == 0) {
                cmd = CMD_CHOWN;
                // extra argument load
                if (argc > i + 1) {
                    // chown (uid) (gid)
                    val1 = strtoul(argv[i], (char **)NULL, 10);
                    if (errno != 0 || val1 < 0 || val1 > 0xffff) {
                        cmd = CMD_ERR;
                    } else {
                        i++;
                        val2 = strtoul(argv[i], (char **)NULL, 10);
                        if (errno != 0 || val1 < 0 || val1 > 0xffff) {
                            cmd = CMD_ERR;
                        } else {
                            i++;
                        }
                    }
                }
            } else if (strcmp("ln-s", arg) == 0) {
                // symbolic link.  Special case where next arg is source and
                // the following arg is the destination.
                // Due to the way error handling is done, the two arguments
                // are parsed when the first one is encountered.  That means
                // "ln-s && noop" is the same as "noop && noop"
                cmd = CMD_SLINK;
            } else if (strcmp("ln-h", arg) == 0) {
                // hard link.  Special case where next arg is source and
                // the following arg is the destination.
                // Due to the way error handling is done, the two arguments
                // are parsed when the first one is encountered.  That means
                // "ln-h && noop" is the same as "noop && noop"
                cmd = CMD_HLINK;
            } else if (strcmp("signal", arg) == 0) {
                // Marks the start of a signal wait.
                sigemptyset(&signalSet);
                cmd = CMD_PAUSE;
            } else {
                // Unknown command.
                cmd = CMD_ERR;

                // This doesn't set the error code, which means that the
                // error will only be reported if there is an argument to
                // this command.  Proper error handling here means more
                // code to handle this edge case.  As it stands, this will
                // not report an error if there is no argument.
            }

            continue;
        }

        // ==================================================================
        // ==================================================================
        // Argument check if/else block.
        //   Each of these must set the err state.
        err = 0;

        // ==================================================================
        // Next command; this compares argument, not cmd
        // Note that cmd could be an error here.
        if (strcmp("&&", arg) == 0) {
            if (errCount > 0) {
                // && with errors stops the shell.
                write(STDERR, "FAIL &&\n", 8);
                break;
            }
#ifdef DEBUG
            write(STDOUT, ":: &&\n", 6);
#endif
            isCmdStart = 1;
        } else
        if (strcmp(";", arg) == 0) {
            // ";" ignores any errors, resetting the error count.
#ifdef DEBUG
            write(STDOUT, ":: ;\n", 5);
#endif
            errCount = 0;
            isCmdStart = 1;
        } else {
            // ==============================================================
            // Process the command argument.

#ifdef DEBUG
            write(STDOUT, ":: handing ", 11);
            write(STDOUT, cmdName, strlen(cmdName));
            write(STDOUT, " ", 1);
            write(STDOUT, arg, strlen(arg));
            write(STDOUT, "\n", 1);
#endif

            err = 0;
            switch (cmd) {
                case CMD_ERR:
                    err = 1;
                    break;
                case CMD_ECHO:
#ifdef DEBUG
                    write(STDOUT, ":: echo ", 8);
                    write(STDOUT, arg, strlen(arg));
                    write(STDOUT, "\n", 1);
#endif
                    write(STDOUT, arg, strlen(arg));
                    write(STDOUT, "\n", 1);
                    break;
                case CMD_RM:
#ifdef DEBUG
                    write(STDOUT, ":: rm ", 6);
                    write(STDOUT, arg, strlen(arg));
                    write(STDOUT, "\n", 1);
#endif
                    err = unlink(arg);
                    break;
                case CMD_RMDIR:
#ifdef DEBUG
                    write(STDOUT, ":: rmdir ", 9);
                    write(STDOUT, arg, strlen(arg));
                    write(STDOUT, "\n", 1);
#endif
                    err = rmdir(arg);
                    break;
                case CMD_MKDIR:
#ifdef DEBUG
                    write(STDOUT, ":: mkdir ", 9);
                    write(STDOUT, arg, strlen(arg));
                    write(STDOUT, "\n", 1);
#endif
                    err = mkdir(arg, val1);
                    break;
                case CMD_CHOWN:
#ifdef DEBUG
                    write(STDOUT, ":: chown ", 9);
                    write(STDOUT, arg, strlen(arg));
                    write(STDOUT, "\n", 1);
#endif
                    err = chown(arg, val1, val2);
                    break;
                case CMD_CHMOD:
#ifdef DEBUG
                    write(STDOUT, ":: chmod ", 9);
                    write(STDOUT, arg, strlen(arg));
                    write(STDOUT, "\n", 1);
#endif
                    err = chmod(arg, val1);
                    break;
                case CMD_SLINK:
                    // On first argument of symlink request.  Need to find next argument.
                    // increase i to point to second argument.
                    i++;
                    if (i >= argc) {
                        err = 1;
                    } else {
#ifdef DEBUG
                        write(STDOUT, ":: ln-s ", 8);
                        write(STDOUT, arg, strlen(arg));
                        write(STDOUT, " ", 1);
                        write(STDOUT, argv[i], strlen(argv[i]));
                        write(STDOUT, "\n", 1);
#endif
                        err = symlink(arg, argv[i]);
                        // Because this command only takes 2 arguments, set the
                        // current command as err.  Because we keep the real command name
                        // in cmdName, this change doesn't affect error reporting.
                        cmd = CMD_ERR;
                    }
                    break;
                case CMD_HLINK:
                    // On first argument of symlink request.  Need to find next argument.
                    // increase i to point to second argument.
                    i++;
                    if (i >= argc) {
                        err = 1;
                    } else {
#ifdef DEBUG
                        write(STDOUT, ":: ln-h ", 8);
                        write(STDOUT, arg, strlen(arg));
                        write(STDOUT, " ", 1);
                        write(STDOUT, argv[i], strlen(argv[i]));
                        write(STDOUT, "\n", 1);
#endif
                        err = link(arg, argv[i]);
                        // Because this command only takes 2 arguments, set the
                        // current command as err.  Because we keep the real command name
                        // in cmdName, this change doesn't affect error reporting.
                        cmd = CMD_ERR;
                    }
                    break;
                case CMD_PAUSE:
                    // The "wait" string indicates the end of the signals.
                    if (strcmp("wait", arg) == 0) {
#ifdef DEBUG
                        write(STDOUT, ":: start signal wait\n", 21);
#endif
                        err = sigwait(&signalSet, &cmd);
#ifdef DEBUG
                        write(STDOUT, ":: wait complete\n", 17);
#endif
                        cmd = CMD_ERR;
                    } else {
                        // "wait" hasn't been found yet, so each argument is a
                        // signal number.
                        val1 = strtoul(arg, (char **)NULL, 10);
                        if (errno != 0 || val1 < 0 || val1 > 32) {
                            err = 1;
                        } else {
#ifdef DEBUG
                            write(STDOUT, ":: signal ", 10);
                            write(STDOUT, arg, strlen(arg));
                            write(STDOUT, "\n", 1);
#endif
                            sigaddset(&signalSet, val1);
                            signal(val1, &emptySignalHandler);
                        }
                    }
                    break;
                case CMD_NOOP:
#ifdef DEBUG
                    write(STDOUT, ":: noop ", 8);
                    write(STDOUT, arg, strlen(arg));
                    write(STDOUT, "\n", 1);
#endif
                    break;
#ifdef DEBUG
                default:
                    write(STDOUT, ":: bad cmd index", 16);
                    break;
#endif
            }
        }

        // ------------------------------------------------------------------
        // End if/else block.
        if (err != 0) {
            // Could use fprintf, but let's get smaller.
            write(STDERR, "ERROR ", 6);
            write(STDERR, cmdName, strlen(cmdName));
            write(STDERR, ": ", 2);
            write(STDERR, arg, strlen(arg));
            write(STDERR, "\n", 1);
            errCount++;
        }
        i++;
    }

    // not needed, but we're nice.
    if (argv != srcArgv) {
        free(argv);
    }

    return errCount;
}
