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
#include <sys/sysmacros.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
// #include <unistd.h>
#include <fcntl.h>
#include "output.h"
#include "general.h"
#include "args.h"
#include "version.h"
#include "commands.h"

// Note: the "*1" to "*2" and so on
// are importantly numbered to allow for +1

#define CMD_ERR     0
#define CMD_NOOP    1
#define CMD_ECHO    2
#define CMD_VERSION 3
#define CMD_RM      3
#define CMD_RMDIR   4
#define CMD_MKDIR1  5
#define CMD_MKDIR2  6
#define CMD_CHMOD1  7
#define CMD_CHMOD2  8
#define CMD_CHOWN1  9
#define CMD_CHOWN2  10
#define CMD_CHOWN3  11
#define CMD_HLINK1  12
#define CMD_HLINK2  13
#define CMD_SLINK1  14
#define CMD_SLINK2  15
#define CMD_MV1     16
#define CMD_MV2     17

// TODO FUTURE sleep should be under signals, but versioning
//   prevents this from being moved until a major version change
//   due to breaking backwards-compatibility with cmd to build
//   relationships.
#define CMD_SLEEP   18

#ifdef USES_INPUT
#define CMD_TOUCH   19
#define CMD_DUP1    20
#define CMD_DUP2    21
#endif
#ifdef USES_DEVICE
#define CMD_MKDNOD1 22
#define CMD_MKDNOD2 23
#define CMD_MKDNOD3 24
#define CMD_MKSNOD  25
#define CMD_MKXNOD  26
#endif

#ifdef USE_SIGNALS
#define CMD_PAUSE   27
#endif

// CMD_EXEC is handled ... differently.

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
    if (
            // nothing was parsed.  This can happen while endPtr[0] == 0 if the argument
            //   has length 0.
            value == endPtr

            // the argument had non-numeric stuff after the first numbers.
            || endPtr[0] != 0

            // Bad number parsing, usually a value of of range problem.
            || errno != 0

            // Easy max value checking, which is a requirement for all our stuff.
            //   It also helps with unsigned long -> int conversion restrictions.
            || ret > maxValue

            // || ret < 0, but we're reading unsigned, so it can't ever happen.
    ) {
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
 *        noop [file1 (file2 ...]]
 *          does nothing.
 *        echo [str1 [str2 ...]]
 *          prints each argument to STDOUT with a newline between them
 *        rm [file1 [file2 ...]]
 *          unlinks (removes) each file, hardlink, or symlink argument
 *        rmdir [file1 [file2 ...]]
 *          removes each directory.  They must be empty first
 *        mkdir (octal mode) [file1 [file2 ...]]
 *          creates the directories with the file mode
 *        mknod (s or p) [file1 [file2 ...]]
 *          creates either a socket (s) or fifo (p) node
 *        mkdev (major) (minor) (c | u | b) [file1 (file2 ...]]
 *          creates a device with the major/minor numbers and either a
 *          character device (c or u) or block device (b).
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
 *        touch (file1 (file2 ...))
 *          Create the file if it doesn't exist; fails on directories; does not change modified.
 *        trunc (file1 (file2 ...))
 *          If the file exists, sets the length to 0 and changes the modified time, if it
 *          doesn't exist, it creates the file; fails on directories.
 *        dup-r (dest fd) (file)
 *          changes the destination file descriptor number to read from the file.
 *          Use "/dev/tty" to change to the user TTY.
 *        dup-a (dest fd) (file)
 *          changes the destination file descriptor number to append to the file.
 *          Use "/dev/tty" to change to the user TTY.
 *        dup-w (dest fd) (file)
 *          changes the destination file descriptor number to truncate and write to the file.
 *          Use "/dev/tty" to change to the user TTY.
 *        mknod 
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
    int val1 = 0;
    int val2 = 0;
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
#ifdef USES_INPUT
            } else if (strequal("touch", cmdName)) {
                // "touch" and "trunc" use the same logic,
                // just different flags on open.  Flags are val1.
                cmd = CMD_TOUCH;
                val1 = O_WRONLY | O_CREAT;
            } else if (strequal("trunc", cmdName)) {
                cmd = CMD_TOUCH;
                val1 = O_WRONLY | O_CREAT | O_TRUNC;
            } else if (strequal("dup-r", cmdName) || strequal("dup-w", cmdName) || strequal("dup-a", cmdName)) {
                cmd = CMD_DUP1;
#endif
#ifdef USES_DEVICE
            } else if (strequal("mknod", cmdName)) {
                cmd = CMD_MKSNOD;
                // Takes an extra argument to indicate the kind of
                // special node to make.
            } else if (strequal("mkdev", cmdName)) {
                cmd = CMD_MKDNOD1;
                // Takes an extra 2 arguments, parsed below.
                // There is a third argument, too, which is the device type,
                // before listing the files.  Once that argument is consumed,
                // this is turned into a CMD_MKXNOD command.
#endif
            } else if (strequal("mkdir", cmdName)) {
                cmd = CMD_MKDIR1;
                // takes an extra argument, parsed below.
            } else if (strequal("chmod", cmdName)) {
                cmd = CMD_CHMOD1;
            } else if (strequal("chown", cmdName)) {
                cmd = CMD_CHOWN1;
            } else if (strequal("ln-s", cmdName)) {
                // symbolic link.  Special case where next arg is source and
                //   the following arg is the destination.
                // Due to the way error handling is done, the two arguments
                //   are parsed when the first one is encountered.  That means
                //   "ln-s && noop" is the same as "noop && noop"
                cmd = CMD_SLINK1;
            } else if (strequal("ln-h", cmdName)) {
                // hard link.  Special case where next arg is source and
                //   the following arg is the destination.
                // Due to the way error handling is done, the two arguments
                //   are parsed when the first one is encountered.  That means
                //   "ln-h && noop" is the same as "noop && noop"
                cmd = CMD_HLINK1;
            } else if (strequal("mv", cmdName)) {
                // Move file.  Similar to ln-s and ln-h.
                cmd = CMD_MV1;
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
                    LOG(":: error\n");
                    err = 1;
                    break;

                // Extra argument processing case statements.
                // These must be designed such that the follow-up command parsing
                // is (current command + 1).
                // If these encounter an error, the command case will switch
                // to CMD_ERR to prevent the real command from running.
                

                // Commands that take a file mode value in val1
                case CMD_MKDIR1:
                case CMD_CHMOD1:
                    LOG(":: Parsing arg as base 8 in val1: ");
                    LOGLN(arg);

                    // Next argument is command + 1
                    cmd++;
                    // file mode, which is octal in range [0, 07777].
                    val1 = toUint(arg, 8, 07777);
                    if (val1 < 0) {
                        LOG("::  - Bad base 8 number, or out of range\n");
                        err = 1;
                        cmd = CMD_ERR;
                        // and do not advance arg, because we want to report
                        // this error.
                    }
                    break;

                // Commands that take a decimal value in val1
                case CMD_CHOWN1:
#ifdef USES_INPUT
                case CMD_DUP1:
#endif
#ifdef USES_DEVICE
                case CMD_MKDNOD1:
#endif
                    LOG(":: Parsing arg as base 10 in val1: ");
                    LOGLN(arg);
                    cmd++;
                    val1 = toUint(arg, 10, 0xffff);
                    if (val1 < 0) {
                        LOG("::  - Bad base 10 number, or out of range\n");
                        err = 1;
                        cmd = CMD_ERR;
                    }
                    break;

                // Commands that take a decimal value in val2
                case CMD_CHOWN2:
#ifdef USES_DEVICE
                case CMD_MKDNOD2:
#endif
                    LOG(":: Parsing arg as base 10 in val2: ");
                    LOGLN(arg);
                    cmd++;
                    val2 = toUint(arg, 10, 0xffff);
                    if (val2 < 0) {
                        LOG("::  - Bad base 10 number, or out of range\n");
                        err = 1;
                        cmd = CMD_ERR;
                    }
                    break;

                // Commands that take a required string in arg2
                case CMD_SLINK1:
                case CMD_HLINK1:
                case CMD_MV1:
                    LOG(":: Loading arg into arg2: ");
                    LOGLN(arg);
                    cmd++;
                    arg2 = arg;
                    break;

                // Run commands
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

                case CMD_MKDIR2:
                    LOG(":: mkdir ");
                    LOGLN(arg);
                    err = mkdir(arg, val1);
                    break;

                case CMD_CHOWN3:
                    LOG(":: chown ");
                    LOGLN(arg);
                    err = chown(arg, val1, val2);
                    break;

                case CMD_CHMOD2:
                    LOG(":: chmod ");
                    LOGLN(arg);
                    err = chmod(arg, val1);
                    break;

                case CMD_SLINK2:
                    // First argument loaded into arg2
                    LOG(":: ln-s ");
                    LOG(arg2);
                    LOG(" ");
                    LOGLN(arg);
                    err = symlink(arg2, arg);
                    // Because this command only takes 2 arguments, set the
                    // current command as err.  Because we keep the real command name
                    // in cmdName, this change doesn't affect error reporting.
                    cmd = CMD_ERR;
                    break;

                case CMD_HLINK2:
                    // First argument loaded into arg2
                    LOG(":: ln-h ");
                    LOG(arg2);
                    LOG(" ");
                    LOGLN(arg);
                    err = link(arg2, arg);
                    // Because this command only takes 2 arguments, set the
                    // current command as err.  Because we keep the real command name
                    // in cmdName, this change doesn't affect error reporting.
                    cmd = CMD_ERR;
                    break;

                case CMD_MV2:
                    // First argument loaded into arg2
                    LOG(":: mv ");
                    LOG(arg);
                    LOG(" ");
                    LOGLN(arg2);
                    err = rename(arg2, arg);
                    // Because this command only takes 2 arguments, set the
                    // current command as err.  Because we keep the real command name
                    // in cmdName, this change doesn't affect error reporting.
                    cmd = CMD_ERR;
                    break;

                case CMD_SLEEP:
                    LOG(":: sleep ");
                    LOGLN(arg)
                    val1 = toUint(arg, 10, MAX_SLEEP_SECONDS);
                    if (val1 > 0) {
                        sleep(val1);
                    } else if (val1 < 0) {
                        err = 1;
                    }
                    break;
#ifdef USES_INPUT
                case CMD_TOUCH:
                    // touch/trunc, with the difference being the flags used.
                    // flags are set in val1
                    LOG(":: touch/trunc ");
                    LOGLN(arg);
                    // Currently a fixed file permission mode
                    val2 = open(
                        arg, val1,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH // | S_IWGRP | S_IWOTH
                    );
                    if (val2 == -1) {
                        err = 1;
                    } else {
                        close(val2);
                    }
                    break;

                case CMD_DUP2:
                    // This command takes 2 arguments, so any more is wrong.
                    cmd = CMD_ERR;
                    // val1 == fd to take on the role.
                    // Interesting hack.  The command is either "dup-?",
                    // so check the cmdName[4] to see the mode.
                    val2 = cmdName[4];
                    if (val2 == 'w') {
                        LOG(":: dup-w");
                        err = O_WRONLY | O_CREAT | O_TRUNC;
                    } else if (val2 == 'a') {
                        LOG(":: dup-a");
                        err = O_WRONLY | O_CREAT | O_APPEND;
                    } else {
                        LOG(":: dup-r");
                        err = O_RDONLY;
                    }
                    if (strequal("&2", arg)) {
                        LOG(" stderr\n");
                        val2 = STDERR_FILENO;
                    } else if (strequal("&1", arg)) {
                        LOG(" stdout\n");
                        val2 = STDOUT_FILENO;
                    } else {
                        LOGLN(arg);
                        val2 = open(
                            arg, err, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH // | S_IWGRP | S_IWOTH
                        );
                    }
                    if (val2 == -1) {
                        err = -1;
                    } else {
                        err = 0;
                        if (dup2(val2, val1) == -1) {
                            err = 1;
                        }
                    }

                    break;
#endif
#ifdef USES_DEVICE
                case CMD_MKDNOD3:
                    // mkdnod requires a follow-up mknod using the generated
                    // device number and mode.

                    // This particular part sets up the variables used to
                    // invoke mknod with the file parameter.
                    cmd = CMD_MKXNOD;

                    LOG(":: mkdev ");
                    LOGLN(arg);

                    // val1 is the major
                    // val2 is the minor
                    // arg is the device type, either 'c' (S_IFCHR), 'u' (S_IFCHR), or 'b' (S_IFBLK).

                    val1 = makedev(val1, val2);
                    if (val1 == -1) {
                        err = 1;
                        break;
                    }
                    val2 = 0644;
                    if (arg[0] == 'b') {
                        val2 |= S_IFBLK;
                    } else if (arg[0] == 'c' || arg[1] == 'u') {
                        val2 |= S_IFCHR;
                    } else {
                        err = 1;
                        cmd = CMD_ERR;
                    }

                    break;

                case CMD_MKSNOD:
                    // Creates a special node

                    // This particular part sets up the variables used to
                    // invoke mknod with the file parameter.
                    cmd = CMD_MKXNOD;

                    LOG(":: mknod type ");
                    LOGLN(arg);

                    // arg is the node type, either 'p' (fifo) or 's' (unix domain socket)
                    val1 = 0;
                    val2 = 0644;
                    if (arg[0] == 'p') {
                        val2 |= S_IFIFO;
                    } else if (arg[0] == 's') {
                        val2 |= S_IFSOCK;
                    } else {
                        err = 1;
                        cmd = CMD_ERR;
                    }

                    break;

                case CMD_MKXNOD:
                    // Virtual command, not directly invoked.
                    // Creates a node using special parameters setup elsewhere.

                    // val1 is the device number
                    // val2 is the mode.
                    // arg is the file to create.

                    LOG(":: mknod ");
                    LOGLN(arg);
                    err = mknod(arg, val2, val1);
                    break;
#endif
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
