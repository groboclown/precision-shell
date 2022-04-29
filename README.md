# fs-shell
Minimal, non-interactive file manipulation shell

Sometimes, you just don't need a shell.  You just need minimal file system operations.

Last build size is 18,520 bytes.  Statically linked.


## What It Does

The shell supports these commands:

* [noop](#noop) - do nothing.
* [echo](#echo) - send text to `stdout`.
* [rm](#rm) - remove files.
* [rmdir](#rmdir) - remove empty directories.
* [chmod](#chmod) - change file permissions.
* [chown](#chown) - change user and group owner for files.
* [ln-s](#ln-s) - create a symbolic link.
* [ln-h](#ln-h) - create a hard link.

It also supports command chaining through `&&` and `;` :

```bash
$ ls
fs-shell
$ ./fs-shell echo abc \&\& rmdir does-not-exist \; echo dce
abc
ERROR rmdir: does-not-exist
dce
$ ./fs-shell echo abc \&\& rmdir does-not-exist \&\& echo dce
abc
ERROR rmdir: does-not-exist
```


## What It Doesn't Do

* List files.
* Background jobs or job control.
* Provide splat pattern replacements.
* Allow for interactive execution.
* Copy files.
* Create normal files or directories.
* Alter file contents.
* Process controls.
* Report detailed error messages.
* Change file timestamps.
* File descriptor redirect.
* Tell you how to use it.  That's what this file is for.


## Help

### noop

Usage: `noop (arg1 (arg2 ...))`

Does nothing and ignores all arguments after it.

### echo

Usage: `echo (str1 (str2 ...))`

Sends to `stdout` each argument, one per line.  To have a multi-word statement on a single line, it must be passed as a single argument.

### rm

Usage: `rm (file1 (file2 ...))`

Removes each file passed as an argument.  The command will attempt to remove each file, and if any of them fail, then the whole command fails with an exit code equal to the sum of the error codes.

### rmdir

Usage: `rmdir (dir1 (dir2 ...))`

Removes each empty directory passed as an argument.  If a directory is not empty, the command will fail.  The command will attempt to remove each directory, and if any of them fail, then the whole command fails with an exit code equal to the sum of the error codes.

### chmod

Usage: `chmod (octal mode) (file1 (file2 ...))`

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

### chown

Usage: `chown (uid) (gid) (file1 (file2 ...))`

Changes the owner and group ID for each file, directory, or symlink argument.


### ln-s

Usage: `ln-s (src file) (dest file)`

Creates a symbolic link named dest file, pointing to src file.


### ln-h

Usage: `ln-h (src file) (dest file)`

Creates a hard link named dest file, pointing to src file.


## Why Would I Need It?

The tool was built with Docker images that use minimal OS resources.  If something like busybox is too big for you, but you need some simple file manipulation because you built upon another image, then this is right for you.


## What Does It Mean?

The "fs" in `fs-shell` can mean:

* "File System", because it only works with file system operations.
* "FROM scratch", because it's useful in Docker images that come from scratch (no OS files).


## Developing

To test, run:

```bash
docker build -f test.Dockerfile
```


## License

fs-shell is licensed under the [MIT license](LICENSE)
