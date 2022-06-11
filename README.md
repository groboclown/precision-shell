# fs-shell
Minimal, non-interactive file manipulation shell

Sometimes, you just don't need a shell.  You just need minimal file system operations.

Last build size:

* Minimal build:
    * glibc: 819,656 bytes
    * musl: 22,024 bytes
    * dietlibc: 13,256 bytes
* Handle OS signals build:
    * glibc: 823,752 bytes
    * musl: 26,120 bytes
    * dietlibc: 17,352 bytes
* Full build (OS signals, read script from stdin or file, execute a program):
    * glibc: 827,848 bytes
    * musl: 26,120 bytes
    * dietlibc: 17,424 bytes

*At some point, a decision should be made whether to keep these separate or just join them into a single executable.*

*dietlibc [requires](https://www.fefe.de/dietlibc/FAQ.txt) that you either not distribute the compiled executable, or release the executable under GPL v2.*


## What It Does

The shell supports these commands:

* [version](#version) - prints the current version.
* [noop](#noop) - do nothing.
* [echo](#echo) - send text to `stdout`.
* [rm](#rm) - remove files.
* [rmdir](#rmdir) - remove empty directories.
* [mv](#mv) - move a file from one name to another.
* [mkdir](#mkdir) - create an empty directory.
* [chmod](#chmod) - change file permissions.
* [chown](#chown) - change user and group owner for files.
* [ln-s](#ln-s) - create a symbolic link.
* [ln-h](#ln-h) - create a hard link.
* [sleep](#sleep) - wait for a number of seconds.
* [mknod](#mknod) - create a FIFO or UNIX socket node. *Only available in device-enabled builds.*
* [mkdev](#mkdev) - create a device OS node. *Only available in device-enabled builds.*
* [touch](#touch) - Update the access and modification times of each file to the current time, or, if a file does not exist, it is created empty. *Only available in input-enabled builds.*
* [trunc](#trunc) - Sets the file length to 0, and if the file does not exist, creates it. *Only available in input-enabled builds.*
* [dup-r, dup-w, dup-a](#dup) - duplicates a file to a file descriptor for the remaining commands in this execution. *Only available in input-enabled builds.*
* [signal .. wait](#signal-wait) - wait for an OS signal before continuing. *Only available in signal-enabled builds.*
* [exec](#exec) - switch execution to a new process. *Only available in exec-enabled builds.*

It also supports:
* [Chaining commands](#command-chaining) together with `&&` and `;`.
* [Standard script argument flag](#standard-script-flag) - if passed with the arguments `-c "commands"`, then the shell will parse the commands argument into individual commands.
* [Script files](#script-files) - as an argument if used with `-f script-file-name`.  *Only available in input-enabled builds.*
* [Commands from stdin](#passing-commands-from-stdin) - With the `-` argument, commands are parsed from stdin.  *Only available in input-enabled builds.*

This allows you to use it in Docker or Podman as a default shell, which is useful if you need file modifications to an image that has no shell.

```Dockerfile
FROM super-skinny-image:11.12

COPY fs-shell /bin/sh

# Because /bin/sh is now the shell, run commands will
#   run through it by executing "/bin/sh -c (arguments)"
RUN echo Startup \
    && rm /www/404.html /www/501.html \
    && rmdir /tmp \
    echo Complete
```


## What It Doesn't Do

* List files.
* Copy files.
* Modify files.  *With input-enabled builds, files can be truncated and written to.*
* Alter file contents.
* Process controls.
* Report detailed error messages.
* Change file timestamps.
* File descriptor redirect.
* Change current directory.
* Provide splat pattern replacements.
* Use environment variables.
* Flow control (if logic and loops) outside of early exit due to prior errors.
* Background jobs or job control.
* Allow for interactive execution.
* Anything with the network.
* Tell you how to use it.  That's what this document is for.


## Why Would I Need It?

The tool was built with Docker images that use minimal OS resources.  If something like busybox is too big for you, but you need some simple file manipulation because you built upon another image, then this is right for you.

See [sample.Dockerfile](sample.Dockerfile) for an example of using it with Docker and a `FROM scratch`, to show that no OS setup is necessary to run the shell.


## What Does It Mean?

The "fs" in `fs-shell` can mean:

* "FROM scratch", because it's useful in Docker images that come from the scratch image (e.g. no OS files).
* "File System", because it only works with file system operations.
* "Functionally Simple", because it's easy on the features.  It's the opposite of "Feature Rich".
* "Freakin' Stupid", because it does so little.


## Help

### version

Usage: `version`

Prints the version information to stdout.  Any additional arguments generates an error.

### noop

Usage: `noop [arg1 [arg2 ...]]`

Does nothing and ignores all arguments after it.

### echo

Usage: `echo [str1 [str2 ...]]`

Sends to `stdout` each argument, one per line.  To have a multi-word statement on a single line, it must be passed as a single argument.

### rm

Usage: `rm [file1 [file2 ...]]`

Removes each file passed as an argument.  The command will attempt to remove each file, and if any of them fail, then the whole command fails with an exit code equal to the sum of the error codes.

### rmdir

Usage: `rmdir [dir1 [dir2 ...]]`

Removes each empty directory passed as an argument.  If a directory is not empty, the command will fail.  The command will attempt to remove each directory, and if any of them fail, then the whole command fails with an exit code equal to the sum of the number of failed files.

### mv

Usage: `mv (src file) (target file)`

Renames the file referenced by the first argument to a new name referenced by the second argument.  If the operation fails, or if there is no second argument, then the command fails with an exit code of 1.

### mkdir

Usage: `mkdir (octal mode) [file1 [file2 ...]]`

Creates the listed directories with the permissions of the first argument.  The parent directory must exist, or it will generate an error.  If any creation fails, then the command fails with the number of failed directories.

### chmod

Usage: `chmod (octal mode) [file1 [file2 ...]]`

Changes the file permissions for each file or directory.  The mode must be passed as an octal number.

Example:

```bash
$ touch a.txt
$ chmod 600 a.txt
$ ls -l a.txt
-rw------- 1 user user   0 Jan 19 09:51 a.txt
$ fs-shell 770 a.txt
$ ls -l a.txt
-rwxrwx--- 1 user user   0 Jan 19 09:51 a.txt
```

This command will fail if the mode value is out of range or not a number.

### chown

Usage: `chown (uid) (gid) [file1 [file2 ...]]`

Changes the owner and group ID for each file, directory, or symlink argument.

### ln-s

Usage: `ln-s (src file) (dest file)`

Creates a symbolic link named dest file, pointing to src file.

### ln-h

Usage: `ln-h (src file) (dest file)`

Creates a hard link named dest file, pointing to src file.

### sleep

Usage: `sleep [seconds [seconds ...]]`

Sleeps for the number of seconds in the argument.  If no arguments are given, or if an argument is not a positive integer, then it does nothing (no error).  If multiple, positive integers are given, then it sleeps for the sum of them.

### mknod

Usage: `mknod (node type) [file1 [file2 ...]]`

Creates a special or ordinary file of the given type.  The node type is OS specific, but in general, the values supported are:

* `p` - pipe (FIFO queue)
* `s` - UNIX domain socket

### mkdev

Usage: `mkdev (major version) (minor version) (node type) [file1 [file2 ...]]`

The node type can be one of these:

* `b` - block device
* `c` and `u` - character device

The major and minor version reflect the OS kernel specific device number.

For example, to create the standard `/dev/null` device, you would run:

```bash
fs-shell mkdev c 1 3 /dev/null
```

The execution of this command requires running with root level privileges, or the tool will report an error.

### touch

*Only available in input-enabled builds.*

Usage: `touch [file1 [file2 ...]]`

For each argument, if it does not exist, it is created.  If the argument exists and is not a file, then the command fails.  **Warning:** Unlike the standard `touch` command, this will not update the modified time of the file.

### trunc

*Only available in input-enabled builds.*

Usage: `trunc [file1 [file2 ...]]`

For each argument, sets the file length to 0 if the file exists, otherwise creates the file.  This is nearly identical to [touch](#touch), with the addition of setting file lengths to 0.

### dup-r, dup-w, dup-a

*Only available in input-enabled builds.*

Usage: `dup-w (fd) (file)`
Usage: `dup-r (fd) (file)`
Usage: `dup-a (fd) (file)`

Opens the given file in either write + truncate mode (`dup-w`), write + append mode (`dup-a`), or read mode (`dup-r`), then duplicates that file descriptor to the "fd" argument.  This duplication remains in effect for the remainder of the execution, or until another `dup` command runs for the same file descriptor.

This is the equivalent of running a command like `echo > out.txt` to redirect stdout, stderr, or stdin to a file.

You can also use the special files "&1" and "&2" to redirect stdout or stderr, respectively, to the file descriptor.  See examples for interesting usages.

Note that this cannot be used for redirecting output from one command into another; that requires FIFO queues and job control, which this shell doesn't support.

**Example 1:**

Run `echo` with output sent to the file `here.txt`, appending existing text.

```bash
echo -n "foo" > here.txt
fs-shell -c "dup-a 1 here.txt && echo ' bar text' && echo more text"
```

And the file `here.txt` will contain:

```
foo bar text
more
text
```

**Example 2:**

Run `echo` with output sent to stderr

```bash
# These are equivalent
fs-shell -c "dup-w 1 &2 && echo hello"
bash -c "echo hello 1>&2"
```

**Example 3:**

Run `find` command with stdout redirected to file `out.txt` and stderr redirected to file `err.txt`

```bash
# These are equivalent
fs-shell -c "dup-w 1 out.txt && dup-w 2 err.txt && exec /usr/sbin/find /var -type f"
bash -c "find /var -type f >out.txt 2>err.txt"
```

**Example 4:**

Generate some text to a file named `contents.txt`, then and sort it into another file named `sorted.txt`.

```bash
fs-shell -c "dup-w 1 contents.txt \
    && echo foo bar baz \
    && dup-w 1 sorted.txt \
    && dup-r 0 contents.txt \
    && exec /usr/sbin/sort"
```


### signal-wait

*Only available in signal-enabled builds.*

Usage: `signal [signal1 [signal2]] [wait]`

Waits for any of the OS signal number arguments before continuing.  If no signal is given (just `signal wait`), then it waits for a standard OS interruption, which will kill the whole process.

Of note, once a signal is added to the list, it is registered for standard OS ignoring.  Only by adding the statement `wait` will the processing wait for the signal to be raised.  This can be used for interesting applications, such as:

```bash
./fs-shell signal 2 \; signal 15 wait
```

This will cause the shell to ignore SIGINT (2, usually sent by a ctrl-c input), and wait for SIGTERM (15).

*Note that `dietlibc` does not support this scenario, and will exit with an error if the to-be-ignored signals are received.*

### exec

*Only available in exec-enabled builds.*

Usage: `exec (cmd) [arg1 [arg2 ...]]`

Reads the remaining arguments (even `;` and `&&`, thus it ignores command chaining), and replaces the current process with the new one.  If the command argument is not given, then this fails.

Commands must be given in the full path; it doesn't look at any environment variable like `PATH`.

### Chaining Commands

Like most shells, you can chain commands together with `&&` and `;`.  `&&` stops the execution if the previous command failed and allows anothe command after it; and `;` resets the error count to 0 and allows another command to follow it.

```bash
$ ls
fs-shell
$ ./fs-shell -c "echo abc && rmdir does-not-exist ; echo dce"
abc
ERROR rmdir: does-not-exist
dce
$ ./fs-shell -c "echo abc && rmdir does-not-exist && echo dce"
abc
ERROR rmdir: does-not-exist
FAIL &&
```

The `exec` command, however, ignores the chain, and passes all arguments to the requested command:

```bash
$ ./fs-shell -c "echo abc ; echo def"
abc
def
# Use the "which" command to find where the echo command is located
# on the path.  Some systems have this under /usr/sbin/echo.
$ ./fs-shell -c "$( which echo ) abc ; echo def"
ERROR /usr/sbin/echo: abc
def
$ ./fs-shell -c "exec $( which echo ) abc ; echo def"
abc ; echo def
```


### Standard Script Flag

The tool also supports invoking it with the arguments `-c "commands"` to simulate running the second argument as a script.

The parsing is kept simple, and follows these rules:

* A space character (` `), tab, and linefeed (`\r`) separates arguments.
* The parser will handle newlines (`\n`) differently depending on whether you use an input-enabled build or not.  With an input-enabled build, newlines are treated like inserting a `;` between commands, whereas a non-input-enabled build treats newlines like a space.
* Pairs of quote characters (`"` and `'`) can encapsulate text, allowing space characters and other quote characters to be part of an argument, rather than separating arguments.
* Characters can be escaped by adding a backslash (`\`) character.  `\n` turns into a newline, `\r` into a linefeed, `\t` into a tab, and anything else is the character itself.  This is how quote characters can be added, as well as an alternate to adding a space to an argument.


### Script Files

If you use the input-enabled build, then you can pass the arguments `-f script-file-name` to run that file as a set of commands.

In this mode, the newline character acts like joining the two lines with a [`;`](#chaining-commands).


### Passing Commands from stdin

If you use the input-enabled build, then you can pass the argument `-` to have the tool read commands from stdin.


## Building

You can build the shell either by installing the autoconf tools locally and running `make`, or you can build it through a container technology by using the example `build-*.Dockerfile` files.

The make generates a collection of different forms of the shell, depending on your needs.  They have different file sizes.


## Developing

To compile the shell, you will need a C compiler that includes a version of the standard C libraries.

To test, run:

```bash
chmod +x tests/*.sh && docker build -f build-(libname).Dockerfile .
```

To build through Docker and capture the built executable:

```bash
for libname in glibc musl ; do
    docker build -t local/fs-shell-${libname} -f build-${libname}.Dockerfile . || exit 1
    ./extract-executables.sh local/fs-shell-${libname} -o out -s .${libname} -d || exit 1
done
```

The build generates combinations of the flags for different executables with various enabled actions.  The `version` command lists which flags the executable has enabled.

Additionally, for your own purposes, you can build it against the "dietlibc" library.  However, this has special considerations in that it's GPL v2, and [you can't distribute the binary under this program's MIT license](http://www.fefe.de/dietlibc/FAQ.txt).

```bash
docker build -t local/fs-shell-dietlibc -f build-dietlibc.Dockerfile . \
    && ./extract-executables.sh local/fs-shell-dietlibc -o out -s .dietlibc -d
```

To bump the version number, change the [version.txt](version.txt) file.  Version numbers must be in a dewey decimal format MAJOR.MINOR.PATCH (e.g. `1.2.3`).


## Contributing

To contribute to the project, submit a PR or open a bug.  All code submitted must be licensed under the MIT license.


## Some Future Items

Future versions will have some backwards-incompatible changes:

* `sleep` will be put under the "signal" category of commands.
* `mkdir` will no longer take a mode argument and instead use a default one; users who want a non-standard mode will need to run `chmod` afterwards to set the mode.


## License

fs-shell is licensed under the [MIT license](LICENSE).

Depending on how you compile the executable, the executable may be under a different license.  For example, compiling with the `dietlibc` library causes the compiled executable to be under GPL 2.0.
