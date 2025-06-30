/*
MIT License

Copyright (c) 2025 groboclown

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef __EXTRACT_ELF_H__
#define __EXTRACT_ELF_H__

#include <elf.h>

typedef struct {
    unsigned char *blob;
    long size;
} Elf_Blob;

typedef struct {
    unsigned int segment_index;
    Elf32_Word elf_offset;
    int blob_offset;
    Elf64_Addr vaddr;           // virtual address (destination) of the segment
    Elf32_Word td_blob_size;    // size of data blob (on disk)
    Elf32_Word tb_memory_size;  // size of memory block (to malloc)

    // Extra information for miscellaneous sections

    // REL, RELA
    Elf32_Word  r_info;    /* Relocation type and symbol index */

    // RELA
    Elf32_Sword r_addend;  /* Addend */

    char final_memory_flag[60];  // e.g., "PROT_READ|PROT_EXEC|PROT_WRITE|PROT_NONE\0"
} Elf32_SectionInfo;

typedef struct {
    unsigned int segment_index;
    Elf64_Word elf_offset;
    unsigned int blob_offset;
    Elf64_Addr vaddr;           // virtual address (destination) of the segment
    Elf64_Word td_blob_size;    // size of data blob (on disk)
    Elf64_Word tb_memory_size;  // size of memory block (to malloc)

    // Extra information for miscellaneous sections

    // REL, RELA
    Elf64_Xword	r_info;			/* Relocation type and symbol index */

    // RELA
    Elf64_Sxword r_addend;		/* Addend */

    // Final memory flag
    // Because this will be executed by the generated C code, it's stored as a
    // character string.  If it's writable, then that's the default protection,
    // and this will be an empty string.
    char final_memory_flag[60];  // e.g., "PROT_READ|PROT_EXEC|PROT_WRITE|PROT_NONE\0"
} Elf64_SectionInfo;

typedef struct {
    Elf32_SectionInfo ehdr32;  // 32-bit ELF header
    Elf64_SectionInfo ehdr64;  // 64-bit ELF header
} Elf_Section;

typedef struct {
    Elf64_Addr entry;  // Entry point address
    Elf32_Half e_class;  // ELF class (ELFCLASS32 or ELFCLASS32)

    Elf_Section *tls;  // thread-local storage section
    Elf_Section *load;  // Loader segment array
    unsigned int load_count;  // count of loader segments
    Elf_Section *relro;    // RELRO (make read-only after relocation) entry array
    unsigned int relro_count;  // count of RELRO entries

    // rel entries are dynamically loaded libraries relocated at runtime.
    // However, this is not supported in this implementation.

    // rela, however, are relocations that are applied at runtime not associated with
    // dynamically loaded libraries.  And this must support them.
    unsigned int rela_blob_offset;  // offset in the blob where RELA entries start
    unsigned int rela_count;  // count of RELA entries

    unsigned int max_memory_size;  // maximum memory size of all segments + vaddr.
} Elf_Sections;

// The data structure that will be written into the blob for RELA.
// It doesn't need the 'info' section.
typedef struct {
    Elf64_Addr r_offset;  // Address of the relocation
    Elf64_Sxword r_addend;  // Addend
} Blob64_Rela;

typedef struct {
    Blob64_Rela *rela;  // RELA entries
    unsigned int count;  // count of RELA entries
    unsigned int alloc;  // allocated count of RELA entries
    size_t size;   // size of the RELA entries in bytes
} Blob64_Rela_Array;


#endif // __EXTRACT_ELF_H__
