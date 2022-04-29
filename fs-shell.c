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
 *          prints each argument to stdout with a newline between them.
 *        rm (file1 (file2 ...))
 *          unlinks (removes) each file, hardlink, or symlink argument
 *        rmdir (file1 (file2 ...))
 *          removes each directory.  They must be empty first.
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

// #define DEBUG 1

#define CMD_ERR   0
#define CMD_NOOP  1
#define CMD_ECHO  2
#define CMD_RM    3
#define CMD_RMDIR 4
#define CMD_CHMOD 5
#define CMD_CHOWN 6
#define CMD_HLINK 7
#define CMD_SLINK 8


int main(const int argc, const char *argv[]) {
    int i = 1;
    long val1 = 0;
    long val2 = 0;
    int err = 0;
    int errCount = 0;
    int cmd = CMD_ERR;
    const char *arg = "";
    const char *cmdName = "";
    int isCmdStart = 1;

    if (argc > i && strcmp("-c", argv[i]) == 0) {
        // Invoked like "/bin/sh -c" style execution.
        // Just ignore the "-c".
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
            fwrite(":: start command ", 17, 1, stdout);
            fwrite(arg, strlen(arg), 1, stdout);
            fwrite("\n", 1, 1, stdout);
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
            } else if (strcmp("chmod", arg) == 0) {
                cmd = CMD_CHMOD;
                // extra argument load
                if (argc > i) {
                    // chmod (octal mode)
                    val1 = strtoul(argv[i], (char **)NULL, 8);
                    i++;
                }
            } else if (strcmp("chown", arg) == 0) {
                cmd = CMD_CHOWN;
                // extra argument load
                if (argc > i + 1) {
                    // chown (uid) (gid)
                    val1 = strtoul(argv[i], (char **)NULL, 10);
                    val2 = strtoul(argv[i + 1], (char **)NULL, 10);
                    i += 2;
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
            } else {
                cmd = CMD_ERR;
            }

            continue;
        }

        // ==================================================================
        // ==================================================================
        // Argument check if/else block.
        //   Each of these must set the err state.
        err = 0;

#ifdef DEBUG
        fwrite(":: handing ", 11, 1, stdout);
        fwrite(cmdName, strlen(cmdName), 1, stdout);
        fwrite(" ", 1, 1, stdout);
        fwrite(arg, strlen(arg), 1, stdout);
        fwrite("\n", 1, 1, stdout);
#endif

        // ==================================================================
        // Next command; this compares argument, not cmd
        // Note that cmd could be an error here.
        if (strcmp("&&", arg) == 0) {
            if (errCount > 0) {
                // && with errors stops the shell.
                fwrite("FAIL &&\n", 8, 1, stderr);
                break;
            }
#ifdef DEBUG
            fwrite(":: &&\n", 6, 1, stdout);
#endif
            isCmdStart = 1;
        } else
        if (strcmp(";", arg) == 0) {
            // ";" ignores any errors, resetting the error count.
#ifdef DEBUG
            fwrite(":: ;\n", 5, 1, stdout);
#endif
            errCount = 0;
            isCmdStart = 1;
        } else {
            // ==============================================================
            // Process the command argument.
            err = 0;
            switch (cmd) {
                case CMD_ERR:
                    err = 1;
                    break;
                case CMD_ECHO:
#ifdef DEBUG
                    fwrite(":: echo ", 8, 1, stdout);
                    fwrite(arg, strlen(arg), 1, stdout);
                    fwrite("\n", 1, 1, stdout);
#endif
                    fwrite(arg, strlen(arg), 1, stdout);
                    fwrite("\n", 1, 1, stdout);
                    break;
                case CMD_RM:
#ifdef DEBUG
                    fwrite(":: rm ", 6, 1, stdout);
                    fwrite(arg, strlen(arg), 1, stdout);
                    fwrite("\n", 1, 1, stdout);
#endif
                    err = unlink(arg);
                    break;
                case CMD_RMDIR:
#ifdef DEBUG
                    fwrite(":: rmdir ", 6, 1, stdout);
                    fwrite(arg, strlen(arg), 1, stdout);
                    fwrite("\n", 1, 1, stdout);
#endif
                    err = rmdir(arg);
                    break;
                case CMD_CHOWN:
#ifdef DEBUG
                    fwrite(":: chown ", 9, 1, stdout);
                    fwrite(arg, strlen(arg), 1, stdout);
                    fwrite("\n", 1, 1, stdout);
#endif
                    err = chown(arg, val1, val2);
                    break;
                case CMD_CHMOD:
#ifdef DEBUG
                    fwrite(":: chmod ", 9, 1, stdout);
                    fwrite(arg, strlen(arg), 1, stdout);
                    fwrite("\n", 1, 1, stdout);
#endif
                    err = chmod(arg, val1);
                    break;
#ifdef DEBUG
                case CMD_SLINK:
                    // On first argument of symlink request.  Need to find next argument.
                    // increase i to point to second argument.
                    i++;
                    if (i >= argc) {
                        err = 1;
                    } else {
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
                        err = link(arg, argv[i]);
                        // Because this command only takes 2 arguments, set the
                        // current command as err.  Because we keep the real command name
                        // in cmdName, this change doesn't affect error reporting.
                        cmd = CMD_ERR;
                    }
                    break;
                case CMD_NOOP:
                    fwrite(":: noop ", 8, 1, stdout);
                    fwrite(arg, strlen(arg), 1, stdout);
                    fwrite("\n", 1, 1, stdout);
                    break;
                default:
                    fwrite(":: bad cmd index", 16, 1, stdout);
                    break;
#endif
            }
        }

        // ------------------------------------------------------------------
        // End if/else block.
        if (err != 0) {
            // Could use fprintf, but let's get smaller.
            fwrite("ERROR ", 6, 1, stderr);
            fwrite(cmdName, strlen(cmdName), 1, stderr);
            fwrite(": ", 2, 1, stderr);
            fwrite(arg, strlen(arg), 1, stderr);
            fwrite("\n", 1, 1, stderr);
            errCount++;
        }
        i++;
    }

    return errCount;
}
