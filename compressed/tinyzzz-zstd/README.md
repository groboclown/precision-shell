# LZMA Compression

Taken from [TinyZZZ](https://github.com/WangXuan95/TinyZZZ), on June 30, 2025.

Distributed under the [MIT License](LICENSE).

The original source lives at `zstdD.c.orig`.  Because the decompression wrapper assumes the source was compressed correctly - it does not need to examine arbitrary files, but a known byte array.
