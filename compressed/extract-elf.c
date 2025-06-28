/*
 * Transform an ELF file into a C header file which contains:
 *  1) A compressed binary blob, stored as an array.
 *  2) How to divide the uncompressed binary blob into segments.
 *  3) Information about the TLS (thread local storage) variable state.
 *  4) RELR relocation information.
 * This shouldn't need to handle RELA relocations, as the as the blob is
 * statically linked.
 */
