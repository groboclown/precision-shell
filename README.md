# fs-shell
Minimal, non-interactive file manipulation shell

Sometimes, you just don't need a shell.  You just need minimal file system operations.

Last build size is 22,024 bytes.  Statically linked.


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

If `-c` is the first argument and the argument count is 2, then the code performs special, limited parsing of the second argument under the assumption that the shell needs to handle that itself.  This allows it to simulate how other shell tools work, and to work as a Docker drop-in replacement for `/bin/sh`.

It also supports command chaining through `&&` and `;` :

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

You can also use it in Docker or Podman as a default shell, which is useful if you need file modifications to an image that has no shell.

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
* Change current directory.
* Use environment variables.
* Anything with the network.
* Tell you how to use it.  That's what this file is for.


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

This command will fail if the mode value is out of range or not a number.

### chown

Usage: `chown (uid) (gid) (file1 (file2 ...))`

Changes the owner and group ID for each file, directory, or symlink argument.


### ln-s

Usage: `ln-s (src file) (dest file)`

Creates a symbolic link named dest file, pointing to src file.


### ln-h

Usage: `ln-h (src file) (dest file)`

Creates a hard link named dest file, pointing to src file.


## Developing

To test, run:

```bash
chmod +x tests/*.sh && docker build -f test.Dockerfile .
```

To build through Docker and capture the built executable:

```bash
docker build -t local/fs-shell -f build.Dockerfile .
container=$( docker create local/fs-shell )
docker cp "${container}":/opt/code/fs-shell fs-shell
docker cp "${container}":/opt/code/fs-shell-debug fs-shell-debug
docker rm "${container}"
```

The build generates 2 versions of the shell: one with extra debug statements sent to stdout; and the normal, minimized version.


## License

fs-shell is licensed under the [MIT license](LICENSE)
