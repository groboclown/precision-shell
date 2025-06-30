# Self-Compressed Executable

In an effort to try to save extra bytes, this directory's tools compresses the executable, and embeds that compressed version inside another executable, which expands it upon execution and runs it.

This uses a highly stripped down version of [tinf library](https://github.com/jibsen/tinf) by Joergen Ibsen.  This version only includes the one deflate function.


## Extract-And-Exec

The first method, `stub.c`

In all but one case, the compressed version is much larger.

This comes partially from including all the ELF file format overhead in the included executable, but also the static version of the wrapper just has a lot of bulk to itself.

Another approach could instead construct a shared object (`.so`) of the base executable, uncompress the file in memory, then run `dlsym` to explicitly load the shared library.  That, unfortunately, means making a dynamic object for the main function.


## Self-Linked

The second method, `stub2.c`, performs the underlying OS link-and-run behavior itself, which eliminates the syscall and mem-mapped file descriptor, and strips the `presh` executable even further by eliminating all the ELF header, and instead hard-coding that into the generated linker.  The `extract-elf.c` program performs the ELF file examination and conversion to a new c program, and `stub2.c` unpacks the stripped down binary to pass to the generated code.

This code has bits from all over the Internet in it, as working examples are spread everywhere, but not well covered.  Stack Overflow was the primary source for this.

With all the effort put into this, it only saves about 1 kb of space over the extract-and-exec method.  Again, this doesn't give any kind of real size reduction.

On top of all this, the current self-linked method doesn't work (it SIGSEGV when running the LOAD section).
