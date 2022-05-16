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
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "output.h"
#include "general.h"
#include "args.h"
#include "version.h"
#include "commands.h"

#define CMD_ERR     0
#define CMD_NOOP    1
#define CMD_ECHO    2
#define CMD_VERSION 3
#define CMD_RM      3
#define CMD_RMDIR   4
#define CMD_MKDIR   5
#define CMD_CHMOD   6
#define CMD_CHOWN   7
#define CMD_HLINK   8
#define CMD_SLINK   9
#define CMD_MV      10
#define CMD_SLEEP   11
#ifdef USE_SIGNALS
#define CMD_PAUSE   12
#endif

// USE_EXEC is handled ... differently.

#define MAX_SLEEP_SECONDS 60 * 60

// Number of arguments for exec
#define MAX_EXEC_ARGS 1000
// Number of characters, including trailing \0 for all the arguments concatenated together.
#define MAX_EXEC_ARG_LEN (MAX_EXEC_ARGS * 100)

static char *EMPTY_STR = "";

void emptySignalHandler(int signal) {
    LOG(":: handled signal\n");
}

int toUint(const char *value, int base, int maxValue) {
    char *endPtr;
    errno = 0;
    unsigned long ret = strtoul(value, &endPtr, base);
    if (value == endPtr || *endPtr != 0 || errno != 0 || ret > maxValue) {
        return -1;
    }
    return ret;
}

/*
 * Usage:
 *   fs-shell (cmd)
 * Where:
 *   cmd: one of:
 *        cmd-part
 *        cmd-part "&&" cmd
 *        cmd-part ";" cmd
 *   cmd-part: one of:
 *        version
 *          prints the current version, including compile flags that affect
 *          which commands are supported.
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
 *        mv (src file) (dest file)
 *          moves src file to a new file named dest file.
 *        sleep (time ...)
 *          Wait for time seconds.
 *        signal [sig1 [sig2 ...]] [wait]
 *          if "wait" is given, waits for any of the signals to be sent to the
 *          shell's process, or for a termination signal to be sent to the
 *          process.  Any signal numbers given will be ignored for the remainder
 *          of the shell execution, unless waited on.
 *        exec cmd (args ...)
 *          launches "cmd" with the arguments, replacing the current command.
 *          All arguments afterwards are consumed, including separators like ';' and '&&'.
 *
 * Commands like "cp" are not supported, because those should be done through
 * Docker ADD and COPY instructions.
 */
int runCommands() {
    long val1 = 0;
    long val2 = 0;
    int err = 0;
    int errCount = 0;
    int cmd = CMD_ERR;
    const char *arg2 = EMPTY_STR;
    const char *cmdName = EMPTY_STR;
#ifdef USE_SIGNALS
    sigset_t signalSet;
#endif
#ifdef USE_EXEC
    char *arg3 = NULL;
    const char **argv = NULL;
#endif

    int isCmdStart = 1;

    // Inefficient loop in terms of performance, but condenses the code.
    const char *arg = advanceToken();

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
            arg = advanceToken();

            // Command name check is done here.
            if (strequal("noop", cmdName)) {
                cmd = CMD_NOOP;
            } else if (strequal("echo", cmdName)) {
                cmd = CMD_ECHO;
            } else if (strequal("version", cmdName)) {
                // print the version and force the next values to be
                //   another command.
                stdoutP(VERSION_STR);
                cmd = CMD_ERR;
            } else if (strequal("rm", cmdName)) {
                cmd = CMD_RM;
            } else if (strequal("rmdir", cmdName)) {
                cmd = CMD_RMDIR;
            } else if (strequal("mkdir", cmdName)) {
                cmd = CMD_MKDIR;
                // extra argument load for the permissions
                if (arg != NULL) {
                    // mkdir (octal mode)
                    val1 = toUint(arg, 8, 07777);
                    if (val1 < 0) {
                        cmd = CMD_ERR;
                        // and do not advance arg, because we want to report
                        // this error.
                    } else {
                        arg = advanceToken();
                    }
                }
            } else if (strequal("chmod", cmdName)) {
                cmd = CMD_CHMOD;
                // extra argument load
                if (arg != NULL) {
                    // chmod (octal mode)
                    val1 = toUint(arg, 8, 07777);
                    // arg == arg3: no conversion (non-numeric)
                    // arg3 != NULL: extra text after.
                    // so, if arg3 != NULL, then error.
                    // Note: octal max value.
                    if (val1 < 0) {
                        cmd = CMD_ERR;
                        // and do not advance arg, because we want to report
                        // this error.
                    } else {
                        arg = advanceToken();
                    }
                }
            } else if (strequal("chown", cmdName)) {
                cmd = CMD_CHOWN;
                // extra argument load
                if (arg != NULL) {
                    // chown (uid) (gid)
                    val1 = toUint(arg, 10, 0xffff);
                    // arg == arg3: no conversion (non-numeric)
                    // arg3 != NULL: extra text after.
                    // so, if arg3 != NULL, then error.
                    if (val1 < 0) {
                        cmd = CMD_ERR;
                        // and do not advance arg, because we want to report
                        // this error.
                    } else {
                        arg = advanceToken();
                        if (arg != NULL) {
                            val2 = toUint(arg, 10, 0xffff);
                            if (val2 < 0) {
                                cmd = CMD_ERR;
                                // and do not advance arg, because we want to report
                                // this error.
                            } else {
                                arg = advanceToken();
                            }
                        }
                    }
                }
            } else if (strequal("ln-s", cmdName)) {
                // symbolic link.  Special case where next arg is source and
                //   the following arg is the destination.
                // Due to the way error handling is done, the two arguments
                //   are parsed when the first one is encountered.  That means
                //   "ln-s && noop" is the same as "noop && noop"
                cmd = CMD_SLINK;
            } else if (strequal("ln-h", cmdName)) {
                // hard link.  Special case where next arg is source and
                //   the following arg is the destination.
                // Due to the way error handling is done, the two arguments
                //   are parsed when the first one is encountered.  That means
                //   "ln-h && noop" is the same as "noop && noop"
                cmd = CMD_HLINK;
            } else if (strequal("mv", cmdName)) {
                // Move file.  Similar to ln-s and ln-h.
                cmd = CMD_MV;
            } else if (strequal("sleep", cmdName)) {
                // Sleep for a time.
                cmd = CMD_SLEEP;
#ifdef USE_SIGNALS
            } else if (strequal("signal", cmdName)) {
                // Marks the start of a signal wait.
                sigemptyset(&signalSet);
                cmd = CMD_PAUSE;
#endif
#ifdef USE_EXEC
            } else if (strequal("exec", cmdName)) {
                // Strange form of the command.  All other arguments must be pulled in and run as-is.
                // This ignores all command parsing after this.
                // Slurp up all remaining arguments up to command max.
                // arg: arguments
                // argv: pointers to arguments.
                // arg3: container for all arguments copied in.
                // val1: position in argv
                // val2: position in arg2
                // err: loop counter
                argv = malloc((sizeof(const char *) * MAX_EXEC_ARGS) + 1);
                if (argv == NULL) {
                    stderrP("ERROR malloc failed\n");
                    return 1;
                }
                //   add a trailing + 1 for the final 0, if necessary
                arg3 = malloc((sizeof(const char) * MAX_EXEC_ARG_LEN) + 1);
                if (arg3 == NULL) {
                    stderrP("ERROR malloc failed\n");
                    return 1;
                }
                val1 = 0;
                val2 = 0;
                // arg is already pointing at the next argument.
                LOG(":: generating arguments\n");
                while (arg != NULL) {
                    if (val1 >= MAX_EXEC_ARGS) {
                        stderrP("ERROR exec too many arguments\n");
                        return 1;
                    }
                    if (val2 >= MAX_EXEC_ARG_LEN) {
                        stderrP("ERROR exec argument total length exceeded\n");
                        return 1;
                    }
                    err = 0;
                    argv[val1++] = &(arg3[val2]);
                    while (arg[err] != 0 && val2 < MAX_EXEC_ARG_LEN) {
                        arg3[val2++] = arg[err++];
                    }
                    arg3[val2++] = 0;
                    LOG(":: arg: [");
                    LOG(argv[val1-1]);
                    LOG("]\n");
                    arg = advanceToken();
                }
                argv[val2++] = NULL;
                if (val2 <= 1) {
                    // No command to run
                    stderrP("ERROR no command\n");
                    return 1;
                }
#ifdef DEBUG
                LOG(":: Running command [");
                LOG(argv[0]);
                LOG("] with arguments [");
                for (err = 0; err < val1; err++) {
                    LOG("] [");
                    LOG(argv[err]);
                }
                LOG("]\n");
#endif  /* DEBUG */
                // This launches a new executable and terminates this one immediately.
                execvp(argv[0], (char * const*) argv);
#endif  /* USE_EXEC */
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
        if (strequal("&&", arg)) {
            if (errCount > 0) {
                // && with errors stops the shell.
                stderrP("FAIL &&\n");
                break;
            }
            LOG(":: &&\n");
            isCmdStart = 1;
        } else
        if (strequal(";", arg)) {
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
                case CMD_HLINK:
                case CMD_MV:
                    // On first argument of the request.  Need to find next argument.
                    // increase i to point to second argument.
                    arg2 = advanceToken();
                    if (arg2 == NULL) {
                        err = 1;
                    } else {
                        LOG(":: ");
                        LOG(cmdName);
                        LOG(" ");
                        LOG(arg);
                        LOG(" ");
                        LOGLN(arg2);
                        if (cmd == CMD_SLINK) {
                            err = symlink(arg, arg2);
                        } else if (cmd == CMD_HLINK) {
                            err = link(arg, arg2);
                        } else if (cmd == CMD_MV) {
                            err = rename(arg, arg2);
                        }
                        // Because this command only takes 2 arguments, set the
                        // current command as err.  Because we keep the real command name
                        // in cmdName, this change doesn't affect error reporting.
                        cmd = CMD_ERR;
                    }
                    break;
                case CMD_SLEEP:
                    val1 = toUint(arg, 10, MAX_SLEEP_SECONDS);
                    if (val1 > 0) {
                        sleep(val1);
                    } else if (val1 < 0) {
                        err = 1;
                    }
                    break;
#ifdef USE_SIGNALS
                case CMD_PAUSE:
                    // The "wait" string indicates the end of the signals.
                    if (strequal("wait", arg)) {
                        LOG(":: start signal wait\n");
                        // Early POSIX Draft 6 has sigwait() return the signal number.
                        // New standard has sigwait() return 0 for no error, or the error number.
                        // Dietlibc uses the old return code.
                        // There's a small chance that an error occurs and that error code
                        //   matches the signal.  To work around this, we carefully setup cmd
                        //   before calling.
                        // CMD_ERR == 0
                        cmd = CMD_ERR;
                        err = sigwait(&signalSet, &cmd);
//#ifdef DEBUG
//printf(":: sigwait() returned %i, signal %i\n", err, cmd);
//#endif
                        if (err != 0 && err != cmd) {
                            // Due to the cmd initialization and the old way of working,
                            // this situation means that this is a real error situation.
                            err = 1;
                        } else {
                            // Looks like a success.
                            err = 0;
                        }
                        LOG(":: wait complete\n");
                    } else {
                        // "wait" hasn't been found yet, so each argument is a
                        // signal number.
                        val1 = toUint(arg, 10, 32);
                        // arg == arg3: no conversion (non-numeric)
                        // arg3 != NULL: extra text after.
                        // so, if arg3 != NULL, then error.
                        if (val1 < 0) {
                            err = 1;
                            // Do not allow the signal to wait.  This can lead to
                            // invalid operations.
                            cmd = CMD_ERR;
                        } else {
                            LOG(":: signal ");
                            LOGLN(arg);
                            sigaddset(&signalSet, val1);
                            signal(val1, &emptySignalHandler);
                        }
                    }
                    break;
#endif /* USE_SIGNALS */
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
        arg = advanceToken();
    }

    return errCount;
}
