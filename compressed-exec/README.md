# About

Program to compress a static executable into the data area of a stub executable.

To build this, it needs to examine the source executable to figure out how to re-map it into the loaded data section.

Some Linux commands of interest:
    * readelf - parse an elf file
    * objdump - print out, disassemble, and other goodies, for an object file.
    * objcopy - copy ELF data into other formats.
    * nm - list symbols in the object file.

Some stdlib commands of interest:
    * mprotect - change the memory protection of a block of memory.  Start pointer should be masked with `(& ~0xfff)`

The uncompressing executable must:
    * Identify memory chunks - blocks of data in the original file, how they map to program space memory, and the protection of the final memory.
    * Create the memory chunks with `mmap()`, write the uncompressed chunks to those blocks of memory, then set the protection with `mprotect()`.
    * Set the entry function to the start address, and call that.

To accomplish this, the construction program must:
    * Use relocatable (PIE) input format data for the executable.
    * Generate an executable with the correctly located blocks.  This should be memory in an area outside the main program.
    * Parse the executable file, to find the blocks of data + their size + their memory map location.
    * Compress the data and put it into a format that can be included by the file.
    * Generate the memory allocator code + memory copy code.

