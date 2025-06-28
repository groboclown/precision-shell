# Self-Compressed Executable

In an effort to try to save extra bytes, this directory's tools compresses the executable, and embeds that compressed version inside another executable, which expands it upon execution and runs it.

This uses a highly stripped down version of [tinf library](https://github.com/jibsen/tinf) by Joergen Ibsen.  This version only includes the one deflate function.


## Results

In all but one case, the compressed version is much larger.

This comes partially from including all the ELF file format overhead in the included executable, but also the static version of the wrapper just has a lot of bulk to itself.

Another approach could instead construct a shared object (`.so`) of the base executable, uncompress the file in memory, then run `dlsym` to explicitly load the shared library.  That, unfortunately, means making a dynamic object for the main function.

The really minimal size would break the ELF file into only the chunks it needs, compress that together, and encode into data all the blob sections and relocation information.  It's just that easy.  Then, the executable would uncompress the blob, allocate the memory sections, copy the memory into the right places, then perform a call to the memory address of the start.  Would all that code cause the size to bloat *even more*?
