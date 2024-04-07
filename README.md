# Precision Shell (presh)

[![Build](https://github.com/groboclown/precision-shell/actions/workflows/build.yml/badge.svg)](https://github.com/groboclown/precision-shell/actions/workflows/build.yml)


## Custom Built Shell With Only What You Need

Sometimes, you don't need or want everyting that a shell or Linux environment can do.  You just have a few things that you need to do.  Adding more adds bloat to your Linux environment and broadens the attack surface.  **Precision Shell** lets you compile the shell to contain only what you need.  It works great as a default shell environment for a Docker or Podman container for when you need just that extra bit of runtime setup, especially when running with a [distro-less container](https://github.com/GoogleContainerTools/distroless/).

`presh` offers a [few commands and shell syntax](#what-it-does), and gives you the flexibility to select which ones to compile, which can make the executable smaller and provide extra security by not enabling commands that don't need to be run.  It's not POSIX conformant, and doesn't try to be.  Its syntax is [eclectic](#command-parsing) and doesn't hold your hand.

The tool has two goals - provide just enough commands for what you need to do, and [make it small](#compiled-size).


## What It Does

The shell supports these commands:

* Output
  * [echo](#echo) - send text to `stdout`.
  * [dup-r, dup-w, dup-a](#dup-r-dup-w-dup-a) - duplicates a file (or file descriptor) to a specific file descriptor for the remaining commands in this execution.
  * [cat-fd](#cat-fd) - write the contents of a file to a file descriptor.
  * [env-cat-fd](#env-cat-fd) - write the contents of a file to a file descriptor, performing environment variable parsing on the source file.
  * [write-fd](#write-fd) - write the arguments to a file descriptor, either stdout or stdin, or those opened through the `dup-*` commands.
  * [ls](#ls) - list contents of a directory.
  * [ls-t](#ls-t) - list contents and content type of a directory
  * [file-stat](#file-stat) - describe a file
* Files
  * [fmode](#fmode) - set the octal file mode for other commands that work with files.
  * [rm](#rm) - remove files.
  * [rmdir](#rmdir) - remove empty directories.
  * [mv](#mv) - move a file from one name to another.
  * [mkdir](#mkdir) - create an empty directory.
  * [chmod](#chmod) - change file permissions.
  * [chown](#chown) - change user and group owner for files.
  * [ln-s](#ln-s) - create a symbolic link.
  * [ln-h](#ln-h) - create a hard link.
  * [touch](#touch) - <strike>Update the access and modification times of each file to the current time, or,</strike> if a file does not exist, it is created empty.
  * [trunc](#trunc) - Sets the file length to 0, and if the file does not exist, creates it.
  * [mknod](#mknod) - create a FIFO or UNIX socket node.
  * [mkdev](#mkdev) - create a device OS node.
* Command Execution
  * [cd](#cd) - change current working directory, for relative file locations and executed command working directory.
  * [export](#export) - export an environment variable + value into the running process and to-be-run child processes.
  * [exec](#exec) - switch execution to a new process.
  * [su-exec](#su-exec) - switch execution to a new process as another user and group ID.
  * [spawn](#spawn) - launch a new process in the background.
  * [su-spawn](#su-spawn) - launch a new process in the background as another user and group ID.
  * [wait-pid](#wait-pid) - wait for a process started by `spawn` to end.
  * [kill-pid](#kill-pid) - send a signal to a process.
  * [start-timer](#start-timer) - start the global timer.  Queried with other commands.
  * [elapsed-time](#elapsed-time) - prints number of seconds since the [start-timer](#start-timer) command was called.
  * [export-elapsed-time](#export-elapsed-time) - records the number of seconds since the [start-timer](#start-timer) command was called into an environment variable.
  * [elapsed-time-under](#elapsed-time-under) - passes with an error code of 0 if the time since the [start-timer](#start-timer) command was called is under the given number of seconds.
  * [signal .. wait](#signal-wait) - wait for an OS signal before continuing.
* Control Flow
  * [if-else](#if-else-command) - run a command conditionally based on the error result of another.
  * [subcmd](#subcmd) - run an argument as a complete precision shell command.
  * [not](#not) - run an argument as a complete precision shell command, and invert the error code (non-zero becomes zero, zero becomes 1).
  * [exit](#exit) - exits the command (or sub-command) with an exit code.
  * [sleep](#sleep) - wait for a number of seconds.
  * [for-each](#for-each) - loop over sub-arguments, setting an environment variable with the value and running a sub-command.
  * [while-error](#while-error) - run a sub-command until it ends without error.
  * [while-no-error](#while-no-error) - run a sub-command until it ends with an error.
  * [is-eq](#is-eq) - compare 2 or more strings for equality.
* Network
  * [test-connect](#test-connect) - test whether a host is listening on a given port number.
  * [export-host-lookup](#export-host-lookup) - exports to an environment variable the IP address of a hostname.
  * [expect-http-get-response](#expect-http-get-response) - generates an error if an HTTP GET request to a host, port, path returns a status code that doesn't match an expected value.
* Usability
  * [pwd](#pwd) - display current working directory, or store it in an environment variable.
  * [version](#version) - prints the current version (cannot be disabled).
  * [noop](#noop) - do nothing.  The comment command.

It also supports:
* [Embedded Quoting](#command-parsing) by using the `[` and `]` symbols to allow easy deep quotes, like `exec [/usr/bin/echo [from native echo]]`
* [Chaining commands](#chaining-commands) together with `&&` and `;` (cannot be disabled).
* [Standard script argument flag](#standard-script-flag) - if passed with the arguments `-c "commands"`, then the shell will parse the commands argument into individual commands (cannot be disabled).
* [Script files](#script-files) - as an argument if used with `-f script-file-name`.
* [Commands from stdin](#passing-commands-from-stdin) - With the `-` argument, commands are parsed from stdin.  **Without the streaming input flag, the shell will not read from stdin.**
* [Environment variable parsing](#environment-variables) - the input values can be replaced with environment variables, when specified in the form `${VALUE}` (the standard shell form of `$VALUE` is not supported).


## What It Doesn't Do

* Report detailed error messages.
* Change file timestamps.
* Provide splat pattern replacements.
* Tell you how to use it.  That's what this document is for.


## Why Would I Need It?

The tool was built with container (Docker and Podman) images that use minimal OS resources.  Golang projects commonly build a single file, the executable, placed into a `FROM scratch` image.  In these environments, `presh` can be easily added to provide some minimal file manipulation.

Additionally, you can build the tool with exactly the commands you need to run.  This limits the attack surface, making your install just that much safer.


## How Do I Get It?

Because of the goals for the shell, no compiled version is distributed.  You're expected to build it from source yourself.

You can build it directly and set the included commands:

```bash
make "COMMAND_FLAGS=-DUSE_CMD_CHMOD -DUSE_STREAMING_INPUT"
```

Flags are split into command flags and usage flags.  Each [command](#help) in this document also includes the flag to enable it in the compiled executable.  A complete list of flags is in the [`Makefile.command-flags`](Makefile.command-flags) file.

You can use some built-in command flag groups:

* `make INCLUDE_ALL_COMMANDS=1` - build with all supported commands.
* `make INCLUDE_STANDARD_COMMANDS=1` - include some simple file operations, streaming input, sleep, and signal handling.
* `make INCLUDE_MINIMAL_COMMANDS=1` - include symbolic links, mkdir, echo, chown, and chmod.

The commands that cannot be directly controlled for inclusion are:

* `version` - this is always included.
* `fmode` - this is automatically included for `mkdir`, `touch`, `trunc`, and the `dup` commands.

To run the build, you'll need basic C compiler, linker, and make.  If you have Python 3 installed, you can generate the command source files (they are bundled if you don't have it).

The most common setup is to build it inside a Docker container for use in another container.  See [`Dockerfile`](Dockerfile) for how this is done.


## Using It With Docker

You can build it outside the container and bundle it in your container through a simple copy:

```Dockerfile
FROM super-skinny-image:11.12

COPY presh /bin/sh

# Because /bin/sh is now the shell, run commands will
#   run through it by executing "/bin/sh -c (arguments)"
RUN echo Startup \
    && rm /www/404.html /www/501.html \
    && rmdir /tmp \
    echo Complete
```

You can also build it in a multi-stage Dockerfile.  See [sample.Dockerfile](sample.Dockerfile) for an example of using it with Docker and a `FROM scratch`, to show that no OS setup is necessary to run the shell.


## Help

All commands and special build modes are described here.  To add in a compile flag, run the build like: `make COMMAND_FLAGS="-DUSE_CMD_ECHO -DUSE_CMD_RM"`

### cat-fd

**Compile flag**: `-DUSE_CMD_CAT_FD`

**Usage**: `cat-fd (fd) [file 1 [file 2...]]`

The first argument is the file descriptor to send the output to (1 == stdout, 2 == stderr, and others can be used by use of the dup commands).

For each additional argument, in order, the command reads its contents and sends it, as-is, to the file descriptor.  If the command encounters a problem reading or accessing the file, the command will generate an error for that argument, but will keep going.

This can be used with the [`dup-w`](#dup-r-dup-w-dup-a) and [`dup-a`](#dup-r-dup-w-dup-a) commands to perform a file copy operation.

**Example 1:**

Copy a file, and report that it succeeded.

```bash
$ echo "file contents" > a.txt
$ presh -c "\
  echo [Copying a.txt to b.txt] \
  && dup-w 2 b.txt \
  && cat-fd 2 a.txt \
  && dup-w 2 &2 \
  && echo [Completed copy]"
```

### cd

**Compile flag**: `-DUSE_CMD_CD`

**Usage**: `cd (directory)`

Changes the current directory.  Useful for relative paths when running file commands, or when launching an executable to run inside a specific directory.

**Example 1:**

```bash
$ presh -c "\
  cd /usr ; spawn [/usr/bin/pwd] ; sleep 1 ;
  cd /lib ; spawn [/usr/bin/pwd]"
/usr
/lib
```

### chmod

**Compile flag**: `-DUSE_CMD_CHMOD`

**Usage**: `chmod (octal mode) [file1 [file2 ...]]`

Changes the file permissions for each file or directory.  The mode must be passed as an octal number.

Example:

```bash
$ touch a.txt
$ chmod 600 a.txt
$ ls -l a.txt
-rw------- 1 user user   0 Jan 19 09:51 a.txt
$ presh -c "chmod 770 a.txt"
$ ls -l a.txt
-rwxrwx--- 1 user user   0 Jan 19 09:51 a.txt
```

This command will fail if the mode value is out of range or not a number.

### chown

**Compile flag**: `-DUSE_CMD_MKDIR`

**Usage**: `chown (uid) (gid) [file1 [file2 ...]]`

Changes the owner and group ID for each file, directory, or symlink argument.

### dup-r, dup-w, dup-a

**Compile flag**: `-DUSE_CMD_DUP_W` and `-DUSE_CMD_DUP_R` and `-DUSE_CMD_DUP_A`

**Usage**: `dup-w (fd) (file)`

**Usage**: `dup-r (fd) (file)`

**Usage**: `dup-a (fd) (file)`

Opens the given file in either write + truncate mode (`dup-w`), write + append mode (`dup-a`), or read mode (`dup-r`), then duplicates that file descriptor to the "fd" argument.  This duplication remains in effect for the remainder of the execution, or until another `dup` command runs for the same file descriptor.

This is the equivalent of running a command like `echo > out.txt` to redirect stdout, stderr, or stdin to a file.

You can also use the special files "&1" and "&2" to redirect stdout or stderr, respectively, to the file descriptor.  See examples for interesting usages.

If this creates a file, the file will have the file permissions set in [`fmode`](#fmode).

In order to redirect output from one command to input of another command, you must use a combination of [`mknod`](#mknod) to create a FIFO queue and [`spawn`](#spawn) to launch the first command in the background.

**Example 1:**

Run `echo` with output sent to the file `here.txt`, appending existing text.

```bash
echo -n "foo" > here.txt
presh -c "dup-a 1 here.txt && echo [ bar text] && echo more text && dup-w 1 &1 && echo [complete]"
```

And the file `here.txt` will contain:

```
foo bar text
more
text
```

The final `dup-w 1 &1` restores stdout to the original stdout, allowing the `echo [complete]` to be seen on the console, and not written to `here.txt`.

**Example 2:**

Run `echo` with output sent to stderr

```bash
# These are equivalent
presh -c "dup-w 1 &2 && echo hello"
bash -c "echo hello 1>&2"
```

**Example 3:**

Run `find` command with stdout redirected to file `out.txt` and stderr redirected to file `err.txt`

```bash
# These are equivalent
presh -c "dup-w 1 out.txt && dup-w 2 err.txt && exec /usr/sbin/find /var -type f"
bash -c "find /var -type f >out.txt 2>err.txt"
```

**Example 4:**

Generate some text to a file named `contents.txt`, then and sort it into another file named `sorted.txt`.

```bash
presh -c "dup-w 1 contents.txt \
    && echo foo bar baz \
    && dup-w 1 sorted.txt \
    && dup-r 0 contents.txt \
    && exec /usr/sbin/sort"
```

**Example 5:**

Perform file descriptor redirect, then restore it when complete.

```bash
presh -c "dup-w 1 contents.txt \
    && echo foo bar baz \
    && dup-w 1 &1 \
    && echo [Wrote to contents.txt]"
```

### echo

**Compile flag**: `-DUSE_CMD_ECHO`

**Usage**: `echo [str1 [str2 ...]]`

Sends to `stdout` each argument, one per line.  To have a multi-word statement on a single line, it must be passed as a single argument; see (Command Parsing)[#command-parsing] for details.

### elapsed-time

**Compile flag**: `-DUSE_CMD_ELAPSED_TIME`

**Usage**: `elapsed-time`

Sends to stdout the number of seconds since the [`start-timer`](#start-timer) command was called, or, if `start-timer` was never called, then time since the epoch.

By including this command, it implies the inclusion of the `start-timer` command.

**Example:**

```bash
presh -c "start-timer ; sleep 2 ; elapsed-time ; sleep 2 ; elapsed-time"
2
4
```

### elapsed-time-under

**Compile flag**: `-DUSE_CMD_ELAPSED_TIME_UNDER`

**Usage**: `elapsed-time-under (seconds)`

If the number of seconds since the last call to the [start-timer](#start-timer) command is less than the argument's value, then this generates an error code of 0.  If not, then it generates an error code of 1.

**Example:**

```bash
#! /usr/bin/presh -f

start-timer
while-no-error [elapsed-time-under 30] [export-elapsed-time DURATION ; touch ${DURATION}.txt ; sleep 1]
```

This example creates 1 file every second for 30 seconds.

### env-cat-fd

**Compile flag**: `-DUSE_CMD_ENV_CAT_FD`

**Usage**: `env-cat-fd (fd) [file 1 [file 2...]]`

This is similar to the Unix `envsubst` command.  It works just like [`cat-fd`](#cat-fd), but performs environment variable replacement on each input file.

### exec

**Compile flag**: `-DUSE_CMD_TRUNC`

**Usage**: `exec (quoted command to run)`

Parses the first argument using the [presh quoting rules](#command-parsing) and transfers execution to the command.  The first value extracted is the full path to the executable to run, and the rest of the values are passed as arguments.  If the executable exists and is executable, then presh will not run any more commands.

Commands must be given in the full path; it doesn't look at any environment variable like `PATH`, even with the [environment variable parsing](#environment-variables) enabled.

If the command file does not exist or is not executable, then the next argument will be tried to run.  If none of them can be run, then the `exec` command generates an error.  Note that if the called command can run but generates an error, presh doesn't have control at that point and will not run anything else.

Because of how `exec` works, trailing the command with [`&&`](#chaining-commands) will never cause the following command to run.  This is because the only way `exec` will allow `presh` to run the next command is if it encountered an error attempting to launch the new process.

**Example 1:**

Report the date and time to a file, where the date time tool may be located in several locations, not known ahead of time.  This uses [`dup-w`](#dup-r-dup-w-dup-a) to cause the stdout from the executed command to be sent to the `/etc/time.txt` file, and if no date program is found, the message "unknown date" is written instead.

```bash
presh -c "dup-w 1 /etc/time.txt && exec [/bin/date] [/usr/bin/date] [/sbin/date] ; echo [unknown date]"
```

**Example 2:**

Runs the native copy command to copy files with spaces in their names.  This shows how the embedded quoting makes it much easier to read.

```bash
presh -c "exec [/usr/bin/cp [source file.txt] [target file.txt]]"
```

### exit

**Compile flag**: `-DUSE_CMD_EXIT`

**Usage**: `exit (exit code)`

Quits the execution of the current command context with an exit code, and no other commands are parsed.  If the exit is within a sub-command, then the sub command exits.

**Example:**

```bash
presh -c "\
  subcmd [exit 1 ; echo [should not run 1]] && \
  echo [should not run 2] ; \
  echo [should run]"
```

The "should not run 1" line will not be reported, because the `exit 1` in the sub-command will stop all further command processing inside that sub-command.  The "should not run 2" will not run either, because it runs after the "&&", which will prevent the next command from running because the sub-command exited with a non-zero return code (1, from the exit argument).  Finally, the "should run" line is printed because the ";" ignores the global error state.

### export

**Compile flag**: `-DUSE_CMD_EXPORT`

**Usage**: `export (ENV_NAME) (env value)`

Export an environment variable + value into the running process and to-be-run child processes.

### export-elapsed-time

**Compile flag**: `-DUSE_CMD_EXPORT_ELAPSED_TIME`

**Usage**: `export-elapsed-time [env-name [env-name ...]]`

Stores in environment variables named in the arguments the number of seconds since the [`start-timer`](#start-timer) command was called, or, if `start-timer` was never called, then time since the epoch.

By including this command, it implies the inclusion of the `start-timer` command.

**Example:**

```bash
presh -c "start-timer ; sleep 5 ; export-elapsed-time TIME ; echo \${TIME}"
5
```

### export-host-lookup

**Compile flag**: `-DUSE_EXPORT_HOST_LOOKUP`

**Usage**: `export-host-lookup (hostname) (ENV_NAME)`

Exports to an environment variable the IP address of a hostname.

### expect-http-get-response

**Compile flag**: `-DUSE_CMD_EXPECT_HTTP_GET_RESPONSE`

**Usage**: `expect-http-get-response (hostname) (port) (path) (response-code)`

Performs a simple HTTP GET request to the given hostname and port, in the form:

```
GET (path) HTTP/1.1
```

If the response code from the server does not match the expected response code, then the command generates an error.

### file-stat

**Compile flag**: `-DUSE_CMD_FILE_STAT`

**Usage**: `file-stat some/file.txt`

*NOTE: this is an experimental feature.  Output is expected to evolve.*

List out information about the given file.

### fmode

**Compile flag**: *included automatically if the dependent commands are included; see below*

**Usage**: `fmode (octal mode)`

Changes an internal default file mode value to the given octal mode.  If never set, the default file mode value is `0644`.

Example:

```bash
$ presh -c "fmode 765 ; touch a.txt"
$ ls -l a.txt
-rwxrw-r-x 1 user user   0 Jan 19 09:51 a.txt 
```

This can include the higher bits, too, so that some commands that support it (and if the executing user has permissions to run it) can set the sticky bits.

This command is added if commands that have an implicit file mode are added through compile flags ([`mkdir`](#mkdir), [`touch`](#touch), [`trunc`](#trunc), [`mknod`](#mknod), [`mkdev`](#mkdev), and any [`dup`](#dup-r-dup-w-dup-a) commands.

### for-each

**Compile flag**: `-DUSE_CMD_FOR_EACH`

**Usage**: `for-each (ENV_NAME) (value-list) (sub-command)`

For each sub-argument in the value-list, set the environment variable name in the first argument to that sub-argument, and run the sub-command.  For the sub-command to be able to use the sub-argument, it will need to escape the environment variable.

**Example 1:**

Here, the `$` is escaped for the original shell that runs presh, then the $ is escaped for presh by doubling it (`$$`).

```bash
$ presh -c "for-each TEXT [first second [sub text] 3 4] [echo \$\${TEXT}]"
first
second
sub text
3
4
```

### if-else

**Compile flag**: `-DUSE_CMD_IF_ELSE`

**Usage**: `if-else (conditional cmd) (if successful) [if failure]`

Runs the first argument as a full presh command, as though it was run through [`subcmd`](#subcmd).  If the exit code is zero, then the second argument is run as a full presh command).  If the first argument fails, then the third argument runs, or is skipped if it isn't given.

**Example 1:**

Test to make sure that [`chmod`](#chmod) correctly makes things not-writable.

```bash
$ presh -c "\
  touch /tmp/file && \
  chmod 000 /tmp/file && \
  if-else [touch /tmp/file] \
      [echo [chmod is dumb]] \
      [echo [chmod works]]"
ERROR touch: /tmp/file
chmod works
```

**Example 2:**

Because precision shell [does not support `||` chaining](#chaining-commands), this can be simulated by using the conditional operation with the [`noop`](#noop) command.

```bash
presh -c "\
  dup 2 /dev/null ;
  if-else [touch [/usr/bin/check-config my-config.rc]] \
      noop \
      [exec /usr/bin/generate-default-config my-config.rc]
  "
```

### is-eq

**Compile flag**: `-USE_CMD_IS_EQUAL`

**Usage**: `is-eq (arg1) (arg2) ...`

Compares the first argument against the remaining arguments for strict equality.  This compares *textual* equality, case sensitive.  If the any of the arguments do not equal, then the command generates an error exit code.

**Example 1:**

If all the arguments strictly equal each other, then this exits with a "0" (no error) exit code.

```bash
presh -c "is-eq [1] [1] [1] && echo [yes]"
```

**Example 2:**

If any of the arguments differ, even by case or whitespace, the command fails.  In the following examples, none of them match.

```bash
presh -c '
  is-eq [1] [ 1] ; echo [A: ${?}] ;
  is-eq [1] [1 ] ; echo [B: ${?}] ;
  is-eq [1] [01] ; echo [C: ${?}]
'
```

### kill-pid

**Compile flag**: `-DUSE_CMD_KILL_PID`

**Usage**: `kill-pid (signal) [pid 1 [pid 2 ...]]`

Sends the signal number in the first argument to the processes in the following arguments.

### ln-h

**Compile flag**: `-DUSE_CMD_LN_H`

**Usage**: `ln-h (src file) (dest file)`

Creates a hard link named dest file, pointing to src file.

### ln-s

**Compile flag**: `-DUSE_CMD_LN_S`

**Usage**: `ln-s (src file) (dest file)`

Creates a symbolic link named dest file, pointing to src file.

### ls

**Compile flag**: `-DUSE_CMD_LS`

**Usage**: `ls (directory1 (directory2 ...))`

Outputs, one per line, each entry within the given directory to stdout.  It does not use splat patterns (e.g. `*.txt`), and only accepts directory names.

The output does not include the directory name for the input.  This means if multiple directory arguments are passed, you won't be able to tell which directory the files belong to.

Limited use as a diagnostic tool when inspecting an image.  Most securely constructed images should never include this command.

**Example:**

```bash
$ presh -c "mkdir x ; mkdir y ; touch x/a.txt ; touch y/b.txt ; ls-l x y"
a.txt
b.txt
```

### ls-l

**Compile flag**: `-DUSE_CMD_LS_L`

**Usage**: `ls-l (directory1 (directory2 ...))`

Generates an output similar to the output for the standard Unix `ls -lA` command.  Output is sent to stdout.  Non-directory arguments generate errors.

Limited use as a diagnostic tool when inspecting an image.  Most securely constructed images should never include this command.

The output format has these columns:

1. File attributes.  Each character indicates a different attribute.
  1. File type.
    * `-` regular file
    * `d` directory
    * `l` symbolic link
    * `b` block-type device
    * `c` character-type device
    * `s` UNIX domain socket
    * `p` FIFO pipe
    * `?` other file type
  2. Sticky flag.  `t` means the userid is "sticky", `s` means the groupid is "sticky", and `-` means no sticky flag.
  3. Owning user read access.  `r` for allowed, `-` for not.
  4. Owning user write access. `w` for allowed, `-` for not.
  5. Owning user execute access.  `x` for allowed, `-` for not.
  6. Owning group read access.  `r` for allowed, `-` for not.
  7. Owning group write access. `w` for allowed, `-` for not.
  8. Owning group execute access.  `x` for allowed, `-` for not.
  6. Other read access.  `r` for allowed, `-` for not.
  7. Other write access. `w` for allowed, `-` for not.
  8. Other execute access.  `x` for allowed, `-` for not.
2. Number of hard links to this file.
3. Owning user ID (numeric, not name)
4. Owning group ID (numeric, not name)
5. Device major number
6. Device minor number
7. File size, in bytes
8. File name, including directory.

The command does not report modified or created times.

**Example 1:**

```bash
$ presh -c "mkdir x ; mkdir y ; touch x/a.txt ; mkdir x/o.d ; mknod p y/b.fifo ; ls-l x y"
d-rwxr-xr-x 2 1000 1000 0 0 4096 x/o.d
--rw-r--r-- 1 1000 1000 0 0 0 x/a.txt
p-rw-r--r-- 1 1000 1000 0 0 0 y/b.fifo
```

**Example 2:**

The `ls-l` command does not work on files; only directory paths.

```bash
$ presh -c "ls-l /dev/null"
ERROR ls-l: /dev/null
$ presh -c "ls-l /dev"
...
c-rw-rw-rw- 1 0 0 1 3 0 /dev/null
...
```

### ls-t

**Compile flag**: `-DUSE_CMD_LS_T`

**Usage**: `ls-t (directory1 (directory2 ...))`

Similar to [`ls`](#ls), but each line starts with a file-type letter:

* `b` - block-type device
* `c` - character-type device
* `f` - normal file
* `d` - directory
* `p` - FIFO pipe
* `l` - symbolic link
* `s` - UNIX domain socket
* `o` - other

Limited use as a diagnostic tool when inspecting an image.  Most securely constructed images should never include this command.

**Example:**

```bash
$ presh -c "mkdir x ; touch x/a.txt ; mkdir x/o.d ; mknod p x/b.fifo ; ls-t x"
f a.txt
d o.d
p b.fifo
```

### mkdev

**Compile flag**: `-DUSE_CMD_MKDEV`

**Usage**: `mkdev (major version) (minor version) (node type) [file1 [file2 ...]]`

The node type can be one of these:

* `b` - block device
* `c` and `u` - character device

The major and minor version reflect the OS kernel specific device number.

For example, to create the standard `/dev/null` device, you would run:

```bash
presh mkdev c 1 3 /dev/null
```

The created file will have the file permissions set in [`fmode`](#fmode).

The execution of this command requires running with root level privileges, or the tool will report an error.

### mkdir

**Compile flag**: `-DUSE_CMD_MKDIR`

**Usage**: `mkdir [file1 [file2 ...]]`

Creates the listed directories.  The parent directory must exist, or it will generate an error.  If any creation fails, then the command fails with the number of failed directories.

Each directory is created with the global file mode (see [`fmode`](#fmode)), with the user, group, and other executable flags also set.  To change this to something else, a [`chmod`](#chmod) must be run.

### mknod

**Compile flag**: `-DUSE_CMD_MKNOD`

**Usage**: `mknod (node type) [file1 [file2 ...]]`

Creates a special or ordinary file of the given type.  The node type is OS specific, but in general, the values supported are:

* `p` - pipe (FIFO queue)
* `s` - UNIX domain socket

The created file will have the file permissions set in [`fmode`](#fmode).

### mv

**Compile flag**: `-DUSE_CMD_MV`

**Usage**: `mv (src file) (target file)`

Renames the file referenced by the first argument to a new name referenced by the second argument.  If the operation fails, or if there is no second argument, then the command fails with an exit code of 1.

### noop

**Compile flag**: `-DUSE_CMD_NOOP`

**Usage**: `noop [arg1 [arg2 ...]]`

**Usage**: `# [Some comment text.  Put inside quoting to protect against && and ;]`

**Usage**: `#! /shebang/format/presh -f`

Does nothing and ignores all arguments after it.

**WARNING**  This does not work like you'd expect a normal comment to work.  This is a command, which means that ';' and '&&' will terminate it, so it's best to quote the arguments.  Also, it will be interpreted as a command for commands that take sub-commands as arguments.

The noop can also be used to mask a file start shebang (`#!`) marker.  To work with presh, the precise format will need a space after the shebang mark, and include the `-f` argument to have the script be interpreted as a file.  This mode requires the [stream input flag](#script-files).

**Example 1:**

A script file.

```bash
#! /usr/bin/presh -f

echo [This is a script file.]
```

**Example 2:**

Careful with characters; the no-op is a command.

```bash
$ presh -c " \
  echo [Text 1] ; \
  # echo [Text 2] ; echo [Text 3] ; \
  # [echo [Text 4] ; echo [Text 5]] ; \
  echo [Text 6]"
Text 1
Text 3
Text 6
```

**Example 3:**

Careful with location; the no-op is a command.

```bash
$ presh -c "\
  touch a-file.txt && \
  chmod 000 a-file.txt &&
  if-else [touch a-file.txt] \
    # [echo Worked] \
    [echo [chmod does not work]] \
    [echo [chmod works]]
chmod does not work
ERROR if-else: echo [chmod works]
```

In this situation, the `# [echo Worked]` line is interpreted as a no-op operation, so the failure line is `[echo [chmod does not work]]` and, because there's no termination for the `if-else` command, it generates an error for the extra parameter `[echo [chmod works]]`.

### not

**Compile flag**: `-DUSE_CMD_NOT`

**Usage**: `not [command]`

Similar to the [subcmd](#subcmd), this runs the argument as its own, complete precision shell command.  When the command exits, the `not` command will invert the exit code, such that an exit code of `0` becomes `1`, and any non-zero exit code becomes `0`.

### pwd

**Compile flag**: `-DUSE_CMD_PWD`

**Usage**: `pwd -`

**Usage**: `pwd (env variable name)`

When used with the argument `-`, the current working directory is written to stdout on its own line.  Otherwise, the environment variable name argument has the current working directory path stored in its value.

Multiple arguments can be given, where the same rules apply for each argument.

### rm

**Compile flag**: `-DUSE_CMD_RM`

**Usage**: `rm [file1 [file2 ...]]`

Removes each file passed as an argument.  The command will attempt to remove each file, and if any of them fail, then the whole command fails with an exit code equal to the sum of the error codes.

### rmdir

**Compile flag**: `-DUSE_CMD_RMDIR`

**Usage**: `rmdir [dir1 [dir2 ...]]`

Removes each empty directory passed as an argument.  If a directory is not empty, the command will fail.  The command will attempt to remove each directory, and if any of them fail, then the whole command fails with an exit code equal to the sum of the number of failed files.

### signal-wait

**Compile flag**: `-DUSE_CMD_SIGNAL`

**Usage**: `signal [*ENV] [signal1 [signal2]] [wait]`

Traps OS signal numbers passed as arguments.  If `wait` is given, then waits for any listed signal to occur before continuing.  If no signal is given (just `signal wait`), then it waits for a standard OS interruption, which will kill the whole process.  If the command includes a `*ENV` (where `ENV` is some environment name) argument, and Pres has [environment variable parsing](#environment-variables) enabled, then the shell stores trapped OS signal numbers in the environment variable.

Of note, once a signal is added to the list, it is registered for standard OS ignoring.  Only by adding the statement `wait` will the processing wait for the signal to be raised.  This can be used for interesting applications, such as:

```bash
presh -c "signal 2 ; signal 15 wait"
```

This will cause the shell to ignore SIGINT (2, usually sent by a ctrl-c input), and wait for SIGTERM (15).

*Note that `dietlibc` does not support ignoring signals not waited on, and will exit with an error if the to-be-ignored signals are received.*

**Example 1:**

Run a process that doesn't listen for OS signals, and instead have the shell take that ownership.

```bash
#! /usr/bin/presh -f

# [ Spawn the process ]
spawn [/usr/sbin/my-server] SERVER_PID && subcmd [
# [ Run this in a subcmd so that fail/pass of each of these instructions ]
# [   only runs when the spawn started successfully. ]

signal 1 2 9 15 17 wait

# [ Force the child to die, in case the signal wasn't a SIGCHLD (17). ]
kill-pid 15 ${SERVER_PID}

# [ Capture the exit code of the spawned server. ]
wait-pid ${SERVER_PID} *EXIT

# [ Exit the script with the spawned server's exit code. ]
exit ${EXIT}
]
```

**Example 2:**

Run a process that might generate an unexpected error.

```bash
#! /usr/bin/presh -f

FIXME

```

### sleep

**Compile flag**: `-DUSE_CMD_SLEEP`

**Usage**: `sleep [seconds [seconds ...]]`

Sleeps for the number of seconds in the argument.  If no arguments are given, or if an argument is not a positive integer, then it does nothing (no error).  If multiple, positive integers are given, then it sleeps for the sum of them.

### spawn

**Compile flag**: `-DUSE_CMD_SPAWN`

**Usage**: `spawn (quoted command) [env for pid]`

Launch a new process in the background.  If the second argument is given, then the launched PID is stored in that value and exported to the environment variables.

**Example 1:**

Launch one command in the background and switch to a different command in the foreground.

```bash
presh -c "\
  spawn [/usr/bin/httpd -c /etc/local.config] && \
  exec [/usr/bin/tail -F /var/log/httpd/error.log]"
```

**Example 2:**

With [environment variable parsing](#environment-variables) enabled, commands can spawn, wait for spawned processes to finish, and kill them.

```bash
presh -c "\
    spawn [/usr/bin/sleep 10000] FOREVER && \
    spawn [/usr/bin/sleep 10] TEN && \
    spawn [/usr/bin/sleep 3] THREE &&\
    echo [Spawned \${FOREVER} then \${TEN} then \${THREE}] ; \
    wait-pid >FIRST ; \
    echo [\${FIRST} just completed.] ;
    wait-pid >SECOND ; \
    echo [\${SECOND} just completed.] ;
    kill-pid 15 \${FOREVER}"
```

### start-timer

**Compile flag**: `-DUSE_CMD_START_TIMER` (but is also included if [elapsed-time](#elapsed-time) or [export-elapsed-time](#export-elapsed-time) is included)

**Usage**: `start-timer`

Sets the current time in the global timer.  If never set, the global timer is set to the epoch.  This can be queried with the [elapsed-time](#elapsed-time) and [export-elapsed-time](#export-elapsed-time) commands.

### su-exec

**Compile flag**: `-DUSE_CMD_SU_EXEC`

**Usage**: `su-exec (UID) (GID) (command as an argument)`

Switches the current user to user id UID (not the user name) and group id GID (not the group name), then runs the third argument.  Unlike [`exec`](#exec), this will not keep running arguments if the first one fails to find a command but instead immediately exit the process.

### su-spawn

**Compile flag**: `-DUSE_CMD_SU_SPAWN`

**Usage**: `su-spawn (UID) (GID) (command as an argument) [env for pid]`

Switches the current user to user id UID (not the user name) and group id GID (not the group name), and launches the third argument as a new process in the background.  If the fourth argument is given, then the launched PID is stored in that value and exported to the environment variables.

Take note that, if the command execution fails to run, or the user ID cannot be changed, then the only way for presh to identify the failure is by inspecting the exit code through [`wait-pid`](#wait-pid).

### subcmd

**Compile flag**: `-DUSE_CMD_SUBCMD`

**Usage**: `subcmd [command as an argument [command ...]]`

Runs each argument as a whole command.  Useful when the sub-command must run only if the previous command succeeded to work around the [`&&`](#chaining-commands) limitation.

**Example:**

```bash
presh -c "\
  touch /tmp/file && \
  subcmd [\
    ln-s /tmp/file /tmp/foo && \
    ln-s /tmp/file /tmp/bar \
  ] ; \
  echo done"
```

### test-connect

**Compile flag:**: `-DUSE_CMD_TEST_CONNECT`

**Usage**: `test-connect (hostname) (port) (timeout)`

Attempts to open a socket connection to the server on the given port or service name.  If the server does not accept the connection within the given timeout seconds, then the command generates an error.

**Example 1:**

Connect to an IPv4 dot-notation host on port 80, with a maximum of 5 seconds waiting for a connection.

```bash
presh -c "\
  test-connect 127.0.0.1 80 5
  "
```

**Example 2:**

Connect to an IPv6 colon-notation host on port 80, with a maximum of 5 seconds waiting for a connection.

```bash
presh -c "test-connect ::0 80 5"
```

**Example 3:**

Connect to the server "google.com" using its registered service name "http", with a maximum of 2 seconds waiting for a connection.

```bash
presh -c "test-connect google.com http 2 && echo [We have Internet connectivity]"
We have Internet connectivity
```

### touch

**Compile flag**: `-DUSE_CMD_TOUCH`

**Usage**: `touch [file1 [file2 ...]]`

For each argument, if it does not exist, it is created.  If the argument exists and is not a file, then the command fails.  **Warning:** Unlike the standard `touch` command, this will not update the modified time of the file.

If this creates a file, the file will have the file permissions set in [`fmode`](#fmode).

### trunc

**Compile flag**: `-DUSE_CMD_TRUNC`

**Usage**: `trunc [file1 [file2 ...]]`

For each argument, sets the file length to 0 if the file exists, otherwise creates the file.  This is nearly identical to [touch](#touch), with the addition of setting file lengths to 0.

If this creates a file, the file will have the file permissions set in [`fmode`](#fmode).

### wait-pid

**Compile flag**: `-DUSE_CMD_WAIT_PID`

**Usage**: `wait-pid (pid number) [*(env name)]`

**Usage**: `wait-pid >(env name)`

**Usage**: `wait-pid >(env name) [*(env name)]`

If passed with a numeric argument, it waits for the PID with that number to finish running.  If passed with an argument that starts with `>`, then the command waits for the next child to finish, and stores its PID in the environment variable with the name after the `>`.

Multiple PID or `>ENV` arguments can be given, and the command will wait for each one to complete.  If a follow up argument in the form `*(ENV NAME)` is given, then the exit code from the process is stored in the given environment variable.

**Example 1:**

wait-pid + spawn + exit can be used together to create the equivalent of a normal shell's command execution.

In this example, because the environment variables REQUEST_PID and REQUEST_EXIT might have a chance of being already set, the variable replacement is escaped with an extra `$` in order to prevent the outer string parsing from replacing the value early.

```bash
presh -c "\
  if-then \
    [spawn [/usr/bin/process-request] REQUEST_PID] \
    [ \
      wait-pid \$\${REQUEST_PID} *REQUEST_EXIT && \
      exit \$\${REQUEST_EXIT}
    ] \
    [exit 1] \
  "
```

### while-error

**Compile flag**: `-DUSE_CMD_WHILE_ERROR`

**Usage**: `while-error (error-sub-cmd) (loop-sub-cmd)`

Executes the error sub command (first argument), and if it generates an error, runs the loop sub command (second argument) then repeats.  If the error sub-command executes and does not generate an error, then the statement ends.

Both arguments are required.  If the loop block isn't necessary, then you can use [`noop`](#noop) as the second argument.

**Example 1:**

```bash
presh -c "\
  while-error \
      [mv /tmp/config.txt config.txt] \
      [sleep 5 ; exec [download-config /tmp/config.txt]]"
```

This will run `mv /tmp/config.txt config.txt`, and if it fails (in this case, because /tmp/config.txt does not exist), then it waits for 5 seconds then executes the native program `download-config /tmp/config.txt`.  This would be useful for a program that needs to download a file, but the file may not be available.

### while-no-error

**Compile flag**: `-DUSE_CMD_WHILE_NO_ERROR`

**Usage**: `while-no-error (sub-cmd) (loop-sub-cmd)`

Identical to [`while-error`](#while-error), except that it stops looping when the first argument's sub command generates an error.

### write-fd

**Compile flag**: `-DUSE_CMD_WRITE_FD`

**Usage**: `write-fd (fd) [text] [text]`

Writes to the file descriptor the precise text given.  Each argument is written as is without spaces between them, and no newline is inserted except where explicitly added to the text.

**Example 1:**

With the following script:

```bash
#! presh -f

dup 12 new-file.txt
write-fd 12 [text1] [text2] [text3\ntext4]
```

This will generate the file `new-file.txt` with the contents:

```
text1text2text3
text4
```

### version

**Compile flag**: *always present*

**Usage**: `version`

Prints the version information to stdout.  Any additional arguments generates an error.




### Chaining Commands

Like most shells, you can chain commands together with `&&` and `;`.  `&&` stops the execution if the previous command failed and allows another command after it; and `;` resets the error count to 0 and allows another command to follow it.  If a new line is encountered, that acts like a `;`.  The common shell `||` chain (run if non-zero error code) is not supported; instead, you will need to take advantage of the [if-else](#if-else) command.

```bash
$ ls
presh
$ ./presh -c "echo abc && rmdir does-not-exist ; echo dce"
abc
ERROR rmdir: does-not-exist
dce
$ ./presh -c "echo abc && rmdir does-not-exist && echo dce"
abc
ERROR rmdir: does-not-exist
FAIL &&
$ ./presh -c "echo abc \
echo def"
abc
def
```

A `;` character erases the previous command's error state, which makes `&&` only sensitive to the previous command's error.

```bash
$ ./presh -c "touch a.txt \
  ; rm a.txt \
  ; # [ This next rm command should fail ] \
  ; rm a.txt \
  ; echo Continuing \
  && echo Ending"
ERROR rm: a.txt
Continuing
Ending
```

On top of this, if you terminate the commands with a `;`, then, even if the command ended with an error, the full `presh` execution will not ([`exit`](#exit), though, is immune to this).

```bash
$ ./presh -c "is-eq 1 2" && echo passed || echo failed
ERROR is-eq: 2
failed
$ ./presh -c "is-eq 1 2 ;" && echo passed || echo failed
ERROR is-eq: 2
passed
```

A [future feature](https://github.com/groboclown/precision-shell/issues/14) may allow changing the newline behavior via a compile flag.

### Environment Variables

**Compile flag**: `-DUSE_ENVIROMENT_INPUT`

With environment variable parsing enabled, argument parsing will replace text in the form `${VALUE}` with the environment variable contained in the `${}` marks.  Currently, these are not expanded into multiple arguments, but instead will be considered part of a single argument.  Because the [`echo`](#echo) command outputs one argument per line, this shows the behavior clearly:

```bash
$ echo "echo \${ABC} \${ABC}" > script.txt
$ cat script.txt
echo ${ABC} ${ABC}
$ export ABC="a b c"
$ ./presh -f script.txt
a b c
a b c
```

To escape an environment variable to not parse it or delay its parsing, use a double `$` character.  See [`wait-pid`](#wait-pid) for an example of where this is necessary.  In general, doubling-up the `$` to escape an environment variable is necessary in sub-commands when the environment variable is set inside other sub-commands.

```bash
$ presh "echo \$\${ABC}"
${ABC}
```

Along with the normal environment variables, it also adds the shell-like `${?}` environment reference to allow `;` separated commands to capture the error code of the previous command.  Note that this adds the environment variable `?`; it's not just a shell variable.

### Standard Script Flag

The tool also supports invoking it with the arguments `-c "commands"` to simulate running the second argument as a script.

### Command Parsing

The parsing is kept simple, and follows these rules:

* A space, tab (`\t`), and linefeed (`\r`) separates arguments.
* The parser will interpret newlines (`\n`) like a semi-colon (`;`), which is the ignore-error command separator.  A [future feature](https://github.com/groboclown/precision-shell/issues/14) may allow changing the newline behavior via a compile flag.
* Arguments are quoted with `[` and `]` pairs, and can be embedded in each other, so that a command like `echo [a [b]]` will output "a [b]" without needing to escape the `[` and `]` marks.
* Characters can be escaped by adding a backslash (`\`) character.  `\n` turns into a newline, `\r` into a linefeed, `\t` into a tab, and anything else is the character itself (for example, inserting a backslash is the standard `\\`).

### Script Files

**Compile flag**: `-DUSE_STREAMING_INPUT`

If you use the input-enabled build, then you can use these additional forms of the command:

* `presh -f script-file.txt`
  runs the commands contained in the file `script-file.txt`
* `presh -`
  reads commands from stdin.

### Better Error Reporting

**Compile flag**: `-DREQUIRE_FULL_CMD`

Some commands, like [`ln-s`](#ln-s), require an exact number of arguments.  Unless this flag is set, using the command with just one argument will not cause a failure.  With this flag, `presh` will generate an error.

### Passing Commands from stdin

**Compile flag**: `-DUSE_STREAMING_INPUT`

If you use the input-enabled build, then you can pass the argument `-` to have the tool read commands from stdin.


# Compiled Size

Last build size:

* Do-nothing build:
  * [glibc (Ubuntu)](#build-glibc.Dockerfile): 819,656 bytes
  * [glibc (Arch)](#build-glibc-arch.Dockerfile): 700,432 bytes
  * [clang/musl (Alpine)](#build-clang.Dockerfile): 25,752 bytes
  * [musl (Alpine)](#build-musl.Dockerfile): 21,944 bytes
  * [dietlibc (Alpine)](#build-dietlibc.Dockerfile): 13,256 bytes
* Minimal build:
  * glibc (Ubuntu): 823,752 bytes
  * glibc (Arch): 700,432 bytes
  * clang (Alpine): 25,752 bytes
  * musl (Alpine): 21,944 bytes
  * dietlibc (Alpine): 17,352 bytes
* Standard build:
  * glibc (Ubuntu): 831,944 bytes
  * glibc (Arch): 704,528 bytes
  * clang (Alpine): 29,848 bytes
  * musl (Alpine): 26,040 bytes
  * dietlibc (Alpine): 17,352 bytes
* Non-Network build:
  * glibc (Ubuntu): 844,232 bytes
  * glibc (Arch): 720,944 bytes
  * clang (Alpine): 46,240 bytes
  * musl (Alpine): 42,432 bytes
  * dietlibc (Alpine): 29,640 bytes
* Full build:
  * glibc (Ubuntu): 1,077,992 bytes
  * glibc (Arch): 921,936 bytes
  * clang (Alpine): 74,912 bytes
  * musl (Alpine): 71,104 bytes
  * dietlibc (Alpine): 46,128 bytes

*dietlibc [requires](https://www.fefe.de/dietlibc/FAQ.txt) that you either not distribute the compiled executable, or release the executable under GPL v2.*

These file sizes are *statically compiled*, so they don't have any external dependencies other than the Linux OS.

These were compiled within Docker containers, which are supplied in the code.  For each stdlib library (glibc, musl, dietlibc), the Linux distribution used to compile it is listed.  This is because the Arch Linux compile size is different than the Ubuntu compile size for the same library.  Your millage may differ depending on the distribution and compiler and other minor differences you use.

dietlibc exhibits slightly different behavior than the other libraries, specifically around the [`signal`](#signal-wait) command.  Please see the command documentation for a description of the differences.  If you select a different library for your compilation, please ensure that the provided test suite passes.


# Contributing

To contribute to the project, submit a PR or open a bug.  All code submitted must be licensed under the MIT license.

Contributing new commands requires (this list assumes that the change is for a single command contained in one file, but it can be multiple commands in a single file, or multiple files, each with their own command):

1. Read through the [source readme file](src/README.md).  This describes how to add commands, though it's lacking on the details that go into a full change.
2. Add cmd_mount.h.in in the src tree and include it in source control changes.
3. Add the cmd_mount.h file to the src/Makefile header list and include the Makefile in source control changes.
4. Generate the .h file by running make and add that to source control.
5. Include the new USE_CMD_* in the flag list in the Makefile.command-flags file, both in the list of flags, and in the INCLUDE_ALL_COMMANDS list.
6. Add new test scripts in the `tests` directory.  The [test readme file](tests/README.md) offers a brief overview of what goes into a test script.
7. Add documentation in the root [README.md](README.md) file, both in the initial command listing, and the detailed description.


## Reporting Security Issues

If you think you discovered an issue that allows for a remote attack on computer running Precision Shell, please open a bug report with enough information to describe what it is, then a representative with the project will reach out to find out more information on a private channel if it's determined to be severe enough to fix without reporting too much information.


# Developing

A guide to the special format for command code is in the [source tree](src/README.md).

To compile the shell, you will need a C compiler that includes a version of the standard C libraries.

To test, run:

```bash
chmod +x tests/*.sh && docker build -f build-(libname).Dockerfile .
```

You can build directly, but some of the tests require root privileges, which are safer to run from within a container.

To build through Docker and capture the built executables:

```bash
mkdir -p out
for libname in glibc musl ; do
    docker build -t local/presh-${libname} -f build-${libname}.Dockerfile .
    container=$( docker create local/presh-${libname} )
    docker cp "${container}:/opt/code/out/presh" out/presh.${libname}
    docker cp "${container}:/opt/code/out/presh-debug" out/presh-debug.${libname}
    docker rm "${container}"
done
```

The build generates combinations of the flags for different executables with various enabled actions.  The `version` command lists which flags the executable has enabled.

To enable different commands in the compiled executable from the Docker build, run the build command like so:

```bash
docker build \
  -t local/presh-${libname} -f build-${libname}.Dockerfile \
  --build-arg COMMANDS="rm rmdir chmod chown" \
  .
```

Additionally, for your own purposes, you can build it against the "dietlibc" library.  However, this has special considerations in that it's GPL v2, and [you can't distribute the binary under this program's MIT license](http://www.fefe.de/dietlibc/FAQ.txt).

```bash
docker build -t local/presh-dietlibc -f build-dietlibc.Dockerfile . \
    && ./extract-executables.sh local/presh-dietlibc -o out -s .dietlibc -d
```

To bump the version number, change the [version.txt](version.txt) file.  Version numbers must be in a dewey decimal format MAJOR.MINOR.PATCH (e.g. `1.2.3`).


## Releasing

Releases should:

1. Bump the version number in [version.txt](version.txt).
2. Bump the version number in [Dockerfile](Dockerfile).
3. Ensure the builds pass.  These run as part of the GitHub actions.
4. The root [README.md](README.md) file is updated with the latest binary file sizes.  The automated builds include the file sizes.
5. Author a new release in GitHub, with the title & tag set to the new version number.  No binary files are included here, but it auto-generates the source tarball.

Version numbers generally follow the guideline of bug fixes increment the patch (third number), added functionality but not backwards incompatible increases the minor (second number), and backwards incompatible changes increases the major (first number).  Some things such as documentation improvements or added platform support, which does not change the functionality of the tool itself, trigger a release, but these will increase the patch number.


# License

Precision Shell is licensed under the [MIT license](LICENSE).

Depending on how you compile the executable, the executable may be under a different license.  For example, compiling with the `dietlibc` library causes the compiled executable to be under GPL 2.0.
