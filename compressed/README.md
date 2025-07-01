# Self-Compressed Executable

In an effort to try to save extra bytes, this directory's tools compresses the executable, and embeds that compressed version inside another executable, which expands it upon execution and runs it.

This uses a highly stripped down version of [tinf library](https://github.com/jibsen/tinf) by Joergen Ibsen.  This version only includes the one deflate function.

## Executable Execution

### Extract-And-Exec

The first method, `stub-run-fd.c`

In all but one case, the compressed version is much larger.

This comes partially from including all the ELF file format overhead in the included executable, but also the static version of the wrapper just has a lot of bulk to itself.

Another approach could instead construct a shared object (`.so`) of the base executable, uncompress the file in memory, then run `dlsym` to explicitly load the shared library.  That, unfortunately, means making a dynamic object for the main function.


### Self-Linked

The second method, `stub-run-lk.c`, performs the underlying OS link-and-run behavior itself, which eliminates the syscall and mem-mapped file descriptor, and strips the `presh` executable even further by eliminating all the ELF header, and instead hard-coding that into the generated linker.  The `extract-elf.c` program performs the ELF file examination and conversion to a new c program, and `stub-run-lk.c` unpacks the stripped down binary to pass to the generated code.

This code has bits from all over the Internet in it, as working examples are spread everywhere, but not well covered.  Stack Overflow was the primary source for this.

With all the effort put into this, it only saves about 1 kb of space over the extract-and-exec method.  Again, this doesn't give any kind of real size reduction.

On top of all this, the current self-linked method doesn't work (it SIGSEGV when running the LOAD section).  Most likely, it didn't correctly copy the blocks into the destination memory.


### Bring Your Own

If you have an alternative approach, you will need to add it to the make file, and create the `stub-run-(name).c` file that conforms to the `stub-run.h` declaration.


## Compression

This directory splits the compression into compression algorithm (e.g. zlib deflate) and decompression implementation.

The code here has a fine edge to walk - it needs to make a highly compressed version of the original file, while also having very small decompression code. 1 kilobyte will make or break an implementation.

The `Makefile` walks a careful, complex series of executions.  First, custom build targets attempt to construct the compressed version of each of the execution methods.

* The source executable must first be compressed down.  This is an intermediary stage.  It may go through several hoops before it's ready for its final form.  For example, the gzip one has a second stage where it strips the header and footer of the file.
* For each implementation of the decompression algorithm, copy the file to `$(SRC_BIN)-(implementation name).bin`.
* Add the implementation to the `DECOMPRESS_IMPLEMENTATIONS` variable.
* Create a directory with the implementation name, and add to it the implementation source files to compile.  The build only will look at the one directory, so giant implementations that span multiple directories won't work.  Also, if you're doing that, then the decompression code will likely also be giant, and not worth your time.
* Create the compression bridge file named `stub-(implementation name).c`.  It should call into the implementation code, such that it conforms to the `stub-decompress.h` declaration.
