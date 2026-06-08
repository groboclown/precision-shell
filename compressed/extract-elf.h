/*
MIT License

Copyright (c) 2025,2026 groboclown

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
    Elf32_Half e_machine;  // ELF machine (EM_X86_64, EM_AARCH64, ...)
    Elf64_Addr min_vaddr;  // Lowest PT_LOAD address
    Elf64_Addr max_vaddr;  // Highest PT_LOAD end address

    Elf_Section *load;  // Loader segment array
    unsigned int load_count;  // count of loader segments
    Elf_Section *relro;    // RELRO (make read-only after relocation) entry array
    unsigned int relro_count;  // count of RELRO entries

    // Shared-object dynamic loader metadata.
    Elf64_Addr dynsym_vaddr;
    Elf64_Addr dynstr_vaddr;
    Elf64_Xword dynsym_ent_size;
    Elf64_Addr rela_vaddr;
    Elf64_Xword rela_size;
    Elf64_Addr jmprel_vaddr;
    Elf64_Xword jmprel_size;
    Elf64_Addr init_vaddr;
    Elf64_Addr fini_vaddr;
    Elf64_Addr init_array_vaddr;
    Elf64_Xword init_array_size;
    Elf64_Addr fini_array_vaddr;
    Elf64_Xword fini_array_size;
} Elf_Sections;


#endif // __EXTRACT_ELF_H__
