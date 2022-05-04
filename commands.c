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
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include "output.h"

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


static char *EMPTY_STR = "";

void emptySignalHandler(int signal) {
    LOG(":: handled signal\n");
}


int runCommands(const char *(*advance)()) {
    long val1 = 0;
    long val2 = 0;
    int err = 0;
    int errCount = 0;
    int cmd = CMD_ERR;
    const char *arg2 = EMPTY_STR;
    const char *cmdName = EMPTY_STR;
    sigset_t signalSet;

    int isCmdStart = 1;

    // Inefficient loop in terms of performance, but condenses the code.
    const char *arg = (*advance)();

    while (arg != NULL) {
        // ----------------------------------------
        // Command start check.
        if (isCmdStart == 1) {
            errCount = 0;
            isCmdStart = 0;
            cmdName = arg;

            LOG(":: start command '");
            LOG(arg);
            LOG("'\n");

            // ends the loop prematurely, so need to explicitly advance the argument.
            arg = (*advance)();

            // Command name check is done here.
            if (strcmp("noop", cmdName) == 0) {
                cmd = CMD_NOOP;
            } else if (strcmp("echo", cmdName) == 0) {
                cmd = CMD_ECHO;
            } else if (strcmp("rm", cmdName) == 0) {
                cmd = CMD_RM;
            } else if (strcmp("rmdir", cmdName) == 0) {
                cmd = CMD_RMDIR;
            } else if (strcmp("mkdir", cmdName) == 0) {
                cmd = CMD_MKDIR;
                // extra argument load for the permissions
                if (arg != NULL) {
                    // mkdir (octal mode)
                    val1 = strtoul(arg, (char **)NULL, 8);
                    // Note: octal max value.
                    if (errno != 0 || val1 < 0 || val1 > 07777) {
                        cmd = CMD_ERR;
                        // and do not advance arg, because we want to report
                        // this error.
                    } else {
                        arg = (*advance)();
                    }
                }
            } else if (strcmp("chmod", cmdName) == 0) {
                cmd = CMD_CHMOD;
                // extra argument load
                if (arg != NULL) {
                    // chmod (octal mode)
                    val1 = strtoul(arg, (char **)NULL, 8);
                    // Note: octal max value.
                    if (errno != 0 || val1 < 0 || val1 > 07777) {
                        cmd = CMD_ERR;
                        // and do not advance arg, because we want to report
                        // this error.
                    } else {
                        arg = (*advance)();
                    }
                }
            } else if (strcmp("chown", cmdName) == 0) {
                cmd = CMD_CHOWN;
                // extra argument load
                if (arg != NULL) {
                    // chown (uid) (gid)
                    val1 = strtoul(arg, (char **)NULL, 10);
                    if (errno != 0 || val1 < 0 || val1 > 0xffff) {
                        cmd = CMD_ERR;
                        // and do not advance arg, because we want to report
                        // this error.
                    } else {
                        arg = (*advance)();
                        if (arg != NULL) {
                            val2 = strtoul(arg, (char **)NULL, 10);
                            if (errno != 0 || val1 < 0 || val1 > 0xffff) {
                                cmd = CMD_ERR;
                                // and do not advance arg, because we want to report
                                // this error.
                            } else {
                                arg = (*advance)();
                            }
                        }
                    }
                }
            } else if (strcmp("ln-s", cmdName) == 0) {
                // symbolic link.  Special case where next arg is source and
                // the following arg is the destination.
                // Due to the way error handling is done, the two arguments
                // are parsed when the first one is encountered.  That means
                // "ln-s && noop" is the same as "noop && noop"
                cmd = CMD_SLINK;
            } else if (strcmp("ln-h", cmdName) == 0) {
                // hard link.  Special case where next arg is source and
                // the following arg is the destination.
                // Due to the way error handling is done, the two arguments
                // are parsed when the first one is encountered.  That means
                // "ln-h && noop" is the same as "noop && noop"
                cmd = CMD_HLINK;
            } else if (strcmp("signal", cmdName) == 0) {
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
                stderrP("FAIL &&\n");
                break;
            }
            LOG(":: &&\n");
            isCmdStart = 1;
        } else
        if (strcmp(";", arg) == 0) {
            // ";" ignores any errors, resetting the error count.
            LOG(":: ;\n");
            errCount = 0;
            isCmdStart = 1;
        } else {
            // ==============================================================
            // Process the command argument.

            LOG(":: handing ");
            LOG(cmdName);
            LOG(" ");
            LOGLN(arg);

            err = 0;
            switch (cmd) {
                case CMD_ERR:
                    err = 1;
                    break;
                case CMD_ECHO:
                    LOG(":: echo ");
                    LOGLN(arg);

                    stdoutPLn(arg);
                    break;
                case CMD_RM:
                    LOG(":: rm ");
                    LOGLN(arg);
                    err = unlink(arg);
                    break;
                case CMD_RMDIR:
                    LOG(":: rmdir ");
                    LOGLN(arg);
                    err = rmdir(arg);
                    break;
                case CMD_MKDIR:
                    LOG(":: mkdir ");
                    LOGLN(arg);
                    err = mkdir(arg, val1);
                    break;
                case CMD_CHOWN:
                    LOG(":: chown ");
                    LOGLN(arg);
                    err = chown(arg, val1, val2);
                    break;
                case CMD_CHMOD:
                    LOG(":: chmod ");
                    LOGLN(arg);
                    err = chmod(arg, val1);
                    break;
                case CMD_SLINK:
                    // On first argument of symlink request.  Need to find next argument.
                    // increase i to point to second argument.
                    arg2 = (*advance)();
                    if (arg2 == NULL) {
                        err = 1;
                    } else {
                        LOG(":: ln-s ");
                        LOG(arg);
                        LOG(" ");
                        LOGLN(arg2);
                        err = symlink(arg, arg2);
                        // Because this command only takes 2 arguments, set the
                        // current command as err.  Because we keep the real command name
                        // in cmdName, this change doesn't affect error reporting.
                        cmd = CMD_ERR;
                    }
                    break;
                case CMD_HLINK:
                    // On first argument of symlink request.  Need to find next argument.
                    // increase i to point to second argument.
                    arg2 = (*advance)();
                    if (arg2 == NULL) {
                        err = 1;
                    } else {
                        LOG(":: ln-h ");
                        LOG(arg);
                        LOG(" ");
                        LOGLN(arg2);
                        err = link(arg, arg2);
                        // Because this command only takes 2 arguments, set the
                        // current command as err.  Because we keep the real command name
                        // in cmdName, this change doesn't affect error reporting.
                        cmd = CMD_ERR;
                    }
                    break;
                case CMD_PAUSE:
                    // The "wait" string indicates the end of the signals.
                    if (strcmp("wait", arg) == 0) {
                        LOG(":: start signal wait\n");
                        err = sigwait(&signalSet, &cmd);
                        LOG(":: wait complete\n");
                        cmd = CMD_ERR;
                    } else {
                        // "wait" hasn't been found yet, so each argument is a
                        // signal number.
                        val1 = strtoul(arg, (char **)NULL, 10);
                        if (errno != 0 || val1 < 0 || val1 > 32) {
                            err = 1;
                        } else {
                            LOG(":: signal ");
                            LOGLN(arg);
                            sigaddset(&signalSet, val1);
                            signal(val1, &emptySignalHandler);
                        }
                    }
                    break;
#ifdef DEBUG
                case CMD_NOOP:
                    LOG(":: noop ");
                    LOGLN(arg);
                    break;
                default:
                    LOG(":: bad cmd index\n");
                    break;
#endif
            }
        }

        // ------------------------------------------------------------------
        // End if/else block.
        if (err != 0) {
            // Could use fprintf, but let's get smaller.
            stderrP("ERROR ");
            stderrP(cmdName);
            stderrP(": ");
            stderrPLn(arg);
            errCount++;
        }
        arg = (*advance)();
    }

    return errCount;
}
