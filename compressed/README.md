# Self-Compressed Executable

In an effort to try to save extra bytes, this directory's tools compresses the executable, and embeds that compressed version inside another executable, which expands it upon execution and runs it.

This uses a highly stripped down version of [tinf library](https://github.com/jibsen/tinf) by Joergen Ibsen.  This version only includes the one deflate function.
