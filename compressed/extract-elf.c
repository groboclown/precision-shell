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


/*
 * Transform an ELF file into:
 *  1) A compressed binary blob, stored as a separate file.
 *     The user can use some external program to compress this blob.
 *  2) A c file that knows how to take the passed-in byte array and run it.
 *  3) A header file that defines the c file's function.
 * 
 * See https://refspecs.linuxfoundation.org/elf/elf.pdf
 *
 * This generator targets shared-object payloads (ET_DYN) and emits
 * a loader that remaps the PT_LOAD segments, applies relocations,
 * resolves imports against the wrapper, and calls main().
 * 
 * This takes advantage of the shared libc compilation of the presh
 * executable by attaching those by static compilation in the wrapper,
 * letting the wrapper's '__start' entrypoint handle the libc initialization,
 * then, at startup time, rewrite the presh pointers to reference the loaded
 * libc function locations.  This requires setting up some imports
 * in the loader, so that it can properly rewire the presh executable.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>
#include <sys/mman.h>
#include <string.h>
#include "extract-elf.h"

#ifndef R_AARCH64_IRELATIVE
// Some musl variants don't define this macro.  Reference the AArch64 ELF ABI.
#define R_AARCH64_IRELATIVE 1032
#endif


unsigned char get_elf_class(Elf_Blob *blob) {
    return blob->blob[EI_CLASS];
}

// --------------------------------------------------------------------
// -- ELF file reading.

// The caller must free the memory allocated in the
// passed-in argument.
int read_elf_blob(char *file, Elf_Blob *blob) {
    FILE *in = fopen(file, "rb");
    if (!in) {
        fprintf(stderr, "Failed to open input ELF file: %s\n", file);
        perror("Failed to open input ELF file");
        return 1;
    }
    fseek(in, 0, SEEK_END);
    long elf_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    unsigned char *buffer = malloc(elf_size);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate %lu bytes of memory for ELF blob\n", elf_size);
        perror("malloc");
        fclose(in);
        return 1;
    }
    if (fread(buffer, 1, elf_size, in) != elf_size) {
        fprintf(stderr, "Failed to read %lu bytes of memory for ELF file %s\n", elf_size, file);
        perror("fread");
        fclose(in);
        return 1;
    }
    fclose(in);

    if (memcmp(buffer, ELFMAG, SELFMAG)) {
        fprintf(stderr, "Not a valid ELF\n");
        free(buffer);
        return 1;
    }

    // Trick to see if the endianness of the current architecture matches the ELF file.
    // If the architecture is little-endian, the first byte of the ELF header will be ELFDATA2LSB.
    // If the architecture is big-endian, the first byte will be ELFDATA2MSB.
    // Then, we look at the first byte of the in-memory value of the short to see if that matches
    // the ELF file's endianness.
    unsigned short arch_endian = (ELFDATA2LSB) | (ELFDATA2MSB << 8);
    if (((unsigned char *)(&arch_endian))[0] != buffer[EI_DATA]) {
        fprintf(stderr, "ELF endianness does not match architecture endianness\n");
        free(buffer);
        return 1;
    }
    blob->blob = buffer;
    blob->size = elf_size;

    return 0;
}

void free_elf_blob(Elf_Blob *blob) {
    if (blob && blob->blob) {
        free(blob->blob);
        blob->blob = NULL;
        blob->size = 0;
    }
}

// --------------------------------------------------------------------
// -- Process the ELF sections and write them to the blob file.

void set_section_flag_64(Elf_Section *section, Elf64_Phdr *phdr) {
    section->ehdr64.final_memory_flag[0] = '\0';
    if (phdr->p_flags & PF_W) {
        strcat(section->ehdr64.final_memory_flag, "PROT_WRITE|");
    }
    if (phdr->p_flags & PF_R) {
        strcat(section->ehdr64.final_memory_flag, "PROT_READ|");
    }
    if (phdr->p_flags & PF_X) {
        strcat(section->ehdr64.final_memory_flag, "PROT_EXEC|");
    }
    strcat(section->ehdr64.final_memory_flag, "PROT_NONE"); // Default to none if no flags set.
}

int write_section_blob64(
    Elf_Blob *elf_blob,
    Elf_Section *section,
    FILE *blob_out,
    int section_index,
    Elf64_Phdr *phdr,
    int blob_pos
) {
    if (phdr->p_memsz < phdr->p_filesz) {
        fprintf(stderr, "Segment memory size is less than file size, which is not supported.\n");
        return -1;
    }
    if (fwrite(elf_blob->blob + phdr->p_offset, 1, phdr->p_filesz, blob_out) != phdr->p_filesz) {
        perror("Failed to write segment to blob file");
        return -1;
    }

    section->ehdr64.segment_index = section_index;
    section->ehdr64.elf_offset = phdr->p_offset;
    section->ehdr64.blob_offset = blob_pos;
    section->ehdr64.vaddr = phdr->p_vaddr;
    section->ehdr64.td_blob_size = phdr->p_filesz;
    section->ehdr64.tb_memory_size = phdr->p_memsz;
    set_section_flag_64(section, phdr);
    // Update the blob position.
    return blob_pos + phdr->p_filesz;
}


Elf64_Addr compute_max_memory(Elf64_Addr prev_max_mem, Elf64_Addr vaddr, Elf64_Xword memsz) {
    Elf64_Addr end_addr = vaddr + memsz;
    if (end_addr > prev_max_mem) {
        return end_addr;
    }
    return prev_max_mem;
}

Elf64_Addr compute_min_memory(Elf64_Addr prev_min_mem, Elf64_Addr vaddr) {
    if (prev_min_mem == 0 || vaddr < prev_min_mem) {
        return vaddr;
    }
    return prev_min_mem;
}

Elf64_Off vaddr_to_file_offset(Elf_Blob *elf_blob, Elf64_Addr vaddr, Elf64_Xword size) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_blob->blob;
    Elf64_Off phoff = ehdr->e_phoff;
    Elf64_Half phentsz = ehdr->e_phentsize;
    Elf64_Half phnum = ehdr->e_phnum;
    for (Elf64_Half i = 0; i < phnum; i++) {
        Elf64_Phdr *phdr = (Elf64_Phdr *)(elf_blob->blob + phoff + ((Elf64_Off)i * phentsz));
        if (phdr->p_type != PT_LOAD) {
            continue;
        }
        if (vaddr < phdr->p_vaddr) {
            continue;
        }
        if (vaddr + size > phdr->p_vaddr + phdr->p_memsz) {
            continue;
        }
        return phdr->p_offset + (Elf64_Off)(vaddr - phdr->p_vaddr);
    }
    return (Elf64_Off)-1;
}

int process_dynamic_section(Elf_Blob *elf_blob, Elf_Sections *sections, Elf64_Phdr *phdr) {
    if (phdr->p_filesz % sizeof(Elf64_Dyn) != 0) {
        fprintf(stderr, "PT_DYNAMIC size is not a multiple of Elf64_Dyn.\n");
        return 1;
    }
    Elf64_Dyn *dyn = (Elf64_Dyn *)(elf_blob->blob + phdr->p_offset);
    size_t dyn_count = phdr->p_filesz / sizeof(Elf64_Dyn);
    for (size_t i = 0; i < dyn_count; i++) {
        switch (dyn[i].d_tag) {
            case DT_SYMTAB:
                sections->dynsym_vaddr = dyn[i].d_un.d_ptr;
                break;
            case DT_STRTAB:
                sections->dynstr_vaddr = dyn[i].d_un.d_ptr;
                break;
            case DT_SYMENT:
                sections->dynsym_ent_size = dyn[i].d_un.d_val;
                break;
            case DT_RELA:
                sections->rela_vaddr = dyn[i].d_un.d_ptr;
                break;
            case DT_RELASZ:
                sections->rela_size = dyn[i].d_un.d_val;
                break;
            case DT_JMPREL:
                sections->jmprel_vaddr = dyn[i].d_un.d_ptr;
                break;
            case DT_PLTRELSZ:
                sections->jmprel_size = dyn[i].d_un.d_val;
                break;
            case DT_INIT:
                sections->init_vaddr = dyn[i].d_un.d_ptr;
                break;
            case DT_FINI:
                sections->fini_vaddr = dyn[i].d_un.d_ptr;
                break;
            case DT_INIT_ARRAY:
                sections->init_array_vaddr = dyn[i].d_un.d_ptr;
                break;
            case DT_INIT_ARRAYSZ:
                sections->init_array_size = dyn[i].d_un.d_val;
                break;
            case DT_FINI_ARRAY:
                sections->fini_array_vaddr = dyn[i].d_un.d_ptr;
                break;
            case DT_FINI_ARRAYSZ:
                sections->fini_array_size = dyn[i].d_un.d_val;
                break;
        }
    }
    return 0;
}

int search_symbol_table(Elf_Blob *elf_blob, Elf64_Shdr *symtab, Elf64_Shdr *strtab, const char *name, Elf64_Addr *value) {
    if (symtab->sh_entsize == 0 || strtab->sh_offset + strtab->sh_size > (Elf64_Off)elf_blob->size) {
        return 1;
    }
    const char *strings = (const char *)(elf_blob->blob + strtab->sh_offset);
    size_t symbol_count = symtab->sh_size / symtab->sh_entsize;
    Elf64_Sym *symbols = (Elf64_Sym *)(elf_blob->blob + symtab->sh_offset);
    for (size_t i = 0; i < symbol_count; i++) {
        if (symbols[i].st_name >= strtab->sh_size) {
            continue;
        }
        if (symbols[i].st_shndx == SHN_UNDEF) {
            continue;
        }
        if (strcmp(strings + symbols[i].st_name, name) == 0) {
            *value = symbols[i].st_value;
            return 0;
        }
    }
    return 1;
}

int find_symbol_value(Elf_Blob *elf_blob, const char *name, Elf64_Addr *value) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_blob->blob;
    if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0) {
        return 1;
    }
    Elf64_Shdr *shdr = (Elf64_Shdr *)(elf_blob->blob + ehdr->e_shoff);
    for (Elf64_Half i = 0; i < ehdr->e_shnum; i++) {
        if (shdr[i].sh_type != SHT_SYMTAB) {
            continue;
        }
        if (shdr[i].sh_link >= ehdr->e_shnum) {
            continue;
        }
        if (search_symbol_table(elf_blob, &shdr[i], &shdr[shdr[i].sh_link], name, value) == 0) {
            return 0;
        }
    }
    for (Elf64_Half i = 0; i < ehdr->e_shnum; i++) {
        if (shdr[i].sh_type != SHT_DYNSYM) {
            continue;
        }
        if (shdr[i].sh_link >= ehdr->e_shnum) {
            continue;
        }
        if (search_symbol_table(elf_blob, &shdr[i], &shdr[shdr[i].sh_link], name, value) == 0) {
            return 0;
        }
    }
    return 1;
}

typedef struct {
    unsigned long relative;
    unsigned long glob_dat;
    unsigned long jump_slot;
    unsigned long abs64;
    unsigned long irelative;
} Relocation_Types;

typedef struct {
    const char *name;
    int weak;
} Used_Import;

int get_relocation_types(Elf32_Half e_machine, Relocation_Types *types) {
    switch (e_machine) {
        case EM_X86_64:
            types->relative = R_X86_64_RELATIVE;
            types->glob_dat = R_X86_64_GLOB_DAT;
            types->jump_slot = R_X86_64_JUMP_SLOT;
            types->abs64 = R_X86_64_64;
            types->irelative = R_X86_64_IRELATIVE;
            return 0;
        case EM_AARCH64:
            types->relative = R_AARCH64_RELATIVE;
            types->glob_dat = R_AARCH64_GLOB_DAT;
            types->jump_slot = R_AARCH64_JUMP_SLOT;
            types->abs64 = R_AARCH64_ABS64;
            types->irelative = R_AARCH64_IRELATIVE;
            return 0;
        default:
            fprintf(stderr, "Unsupported ELF machine for shared-object loader: %u\n", e_machine);
            return 1;
    }
}

int get_dynamic_symbol_table(
    Elf_Blob *elf_blob,
    Elf64_Sym **dynsym,
    size_t *dynsym_count,
    const char **dynstr,
    size_t *dynstr_size
) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_blob->blob;
    if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0) {
        return 1;
    }
    Elf64_Shdr *shdr = (Elf64_Shdr *)(elf_blob->blob + ehdr->e_shoff);
    for (Elf64_Half i = 0; i < ehdr->e_shnum; i++) {
        if (shdr[i].sh_type != SHT_DYNSYM) {
            continue;
        }
        if (shdr[i].sh_link >= ehdr->e_shnum) {
            return 1;
        }
        if (shdr[i].sh_entsize != sizeof(Elf64_Sym)) {
            return 1;
        }
        Elf64_Shdr *strtab = &shdr[shdr[i].sh_link];
        if (shdr[i].sh_offset + shdr[i].sh_size > (Elf64_Off)elf_blob->size ||
                strtab->sh_offset + strtab->sh_size > (Elf64_Off)elf_blob->size) {
            return 1;
        }
        *dynsym = (Elf64_Sym *)(elf_blob->blob + shdr[i].sh_offset);
        *dynsym_count = shdr[i].sh_size / shdr[i].sh_entsize;
        *dynstr = (const char *)(elf_blob->blob + strtab->sh_offset);
        *dynstr_size = strtab->sh_size;
        return 0;
    }
    return 1;
}

int get_relocation_table(
    Elf_Blob *elf_blob,
    Elf64_Addr vaddr,
    Elf64_Xword size,
    Elf64_Rela **rela,
    size_t *rela_count
) {
    if (size == 0) {
        *rela = NULL;
        *rela_count = 0;
        return 0;
    }
    if (size % sizeof(Elf64_Rela) != 0) {
        return 1;
    }
    Elf64_Off file_offset = vaddr_to_file_offset(elf_blob, vaddr, size);
    if (file_offset == (Elf64_Off)-1 || file_offset + size > (Elf64_Off)elf_blob->size) {
        return 1;
    }
    *rela = (Elf64_Rela *)(elf_blob->blob + file_offset);
    *rela_count = size / sizeof(Elf64_Rela);
    return 0;
}

int append_used_import(
    Used_Import **imports,
    size_t *import_count,
    size_t *import_alloc,
    const char *name,
    int weak
) {
    for (size_t i = 0; i < *import_count; i++) {
        if (strcmp((*imports)[i].name, name) == 0) {
            if (!weak) {
                (*imports)[i].weak = 0;
            }
            return 0;
        }
    }
    if (*import_count >= *import_alloc) {
        size_t next_alloc = *import_alloc == 0 ? 16 : (*import_alloc * 2);
        Used_Import *new_imports = realloc(*imports, next_alloc * sizeof(Used_Import));
        if (!new_imports) {
            perror("Failed to grow import table");
            return 1;
        }
        *imports = new_imports;
        *import_alloc = next_alloc;
    }
    (*imports)[*import_count].name = name;
    (*imports)[*import_count].weak = weak;
    (*import_count)++;
    return 0;
}

int collect_imports_from_relocations(
    Elf64_Rela *rela,
    size_t rela_count,
    Elf64_Sym *dynsym,
    size_t dynsym_count,
    const char *dynstr,
    size_t dynstr_size,
    Used_Import **imports,
    size_t *import_count,
    size_t *import_alloc
) {
    for (size_t i = 0; i < rela_count; i++) {
        unsigned long sym_index = ELF64_R_SYM(rela[i].r_info);
        if (sym_index == 0) {
            continue;
        }
        if (sym_index >= dynsym_count) {
            fprintf(stderr, "Relocation references out-of-range dynamic symbol index %lu\n", sym_index);
            return 1;
        }
        Elf64_Sym *sym = &dynsym[sym_index];
        if (sym->st_shndx != SHN_UNDEF) {
            continue;
        }
        if (sym->st_name >= dynstr_size) {
            fprintf(stderr, "Dynamic symbol string offset out of range for index %lu\n", sym_index);
            return 1;
        }
        const char *name = dynstr + sym->st_name;
        if (append_used_import(
                imports,
                import_count,
                import_alloc,
                name,
                ELF64_ST_BIND(sym->st_info) == STB_WEAK
            )) {
            return 1;
        }
    }
    return 0;
}

int collect_shared_imports(
    Elf_Blob *elf_blob,
    Elf_Sections *sections,
    Used_Import **imports,
    size_t *import_count
) {
    Elf64_Sym *dynsym = NULL;
    const char *dynstr = NULL;
    size_t dynsym_count = 0;
    size_t dynstr_size = 0;
    Elf64_Rela *rela = NULL;
    size_t rela_count = 0;
    size_t import_alloc = 0;

    *imports = NULL;
    *import_count = 0;

    if (get_dynamic_symbol_table(elf_blob, &dynsym, &dynsym_count, &dynstr, &dynstr_size)) {
        fprintf(stderr, "Failed to locate dynamic symbol table metadata in shared object.\n");
        return 1;
    }
    if (get_relocation_table(elf_blob, sections->rela_vaddr, sections->rela_size, &rela, &rela_count)) {
        fprintf(stderr, "Failed to locate .rela.dyn contents in shared object.\n");
        free(*imports);
        *imports = NULL;
        return 1;
    }
    if (collect_imports_from_relocations(
            rela,
            rela_count,
            dynsym,
            dynsym_count,
            dynstr,
            dynstr_size,
            imports,
            import_count,
            &import_alloc
        )) {
        free(*imports);
        *imports = NULL;
        return 1;
    }
    if (get_relocation_table(elf_blob, sections->jmprel_vaddr, sections->jmprel_size, &rela, &rela_count)) {
        fprintf(stderr, "Failed to locate .rela.plt contents in shared object.\n");
        free(*imports);
        *imports = NULL;
        return 1;
    }
    if (collect_imports_from_relocations(
            rela,
            rela_count,
            dynsym,
            dynsym_count,
            dynstr,
            dynstr_size,
            imports,
            import_count,
            &import_alloc
        )) {
        free(*imports);
        *imports = NULL;
        return 1;
    }
    return 0;
}

void emit_used_import_resolver(FILE *c_out, Used_Import *imports, size_t import_count) {
    // Create the replacement pointers for the libc functions.
    for (size_t i = 0; i < import_count; i++) {
        fprintf(c_out,
            "extern void ___presh_import_%lu(void) __asm__(\"%s\")%s;\n",
            (unsigned long)i,
            imports[i].name,
            imports[i].weak ? " __attribute__((weak))" : ""
        );
    }
    fprintf(c_out, "\n");
    fprintf(c_out,
        "static uintptr_t resolve_import_symbol(const char *name) {\n"
    );
    for (size_t i = 0; i < import_count; i++) {
        fprintf(c_out,
            "  if (strcmp(name, \"%s\") == 0) { return (uintptr_t)&___presh_import_%lu; }\n",
            imports[i].name,
            (unsigned long)i
        );
    }
    fprintf(c_out,
        "  return 0;\n"
        "}\n\n"
    );
}

size_t get_init_array_count(Elf_Sections *sections) {
    return (size_t)(sections->init_array_size / sizeof(uintptr_t));
}

size_t get_fini_array_count(Elf_Sections *sections) {
    return (size_t)(sections->fini_array_size / sizeof(uintptr_t));
}

void emit_init_helper(FILE *c_out, Elf_Sections *sections) {
    size_t init_array_count = get_init_array_count(sections);
    if (sections->init_vaddr == 0 && init_array_count == 0) {
        return;
    }
    fprintf(c_out,
        "static void call_init_entries(uintptr_t base, int argc, char *argv[], char *envp[]) {\n"
    );
    if (sections->init_vaddr != 0) {
        fprintf(c_out,
            "  ((embedded_init_fn)(base + 0x%lxUL))(argc, argv, envp);\n",
            (unsigned long)sections->init_vaddr
        );
    }
    if (init_array_count == 1) {
        fprintf(c_out,
            "  ((embedded_init_fn)(*(uintptr_t *)(base + 0x%lxUL)))(argc, argv, envp);\n",
            (unsigned long)sections->init_array_vaddr
        );
    } else if (init_array_count > 1) {
        fprintf(c_out,
            "  uintptr_t *init_array = (uintptr_t *)(base + 0x%lxUL);\n"
            "  for (size_t i = 0; i < %luUL; i++) {\n"
            "    if (init_array[i] != 0) {\n"
            "      ((embedded_init_fn)init_array[i])(argc, argv, envp);\n"
            "    }\n"
            "  }\n",
            (unsigned long)sections->init_array_vaddr,
            (unsigned long)init_array_count
        );
    }
    fprintf(c_out, "}\n\n");
}

void emit_fini_helper(FILE *c_out, Elf_Sections *sections) {
    size_t fini_array_count = get_fini_array_count(sections);
    if (sections->fini_vaddr == 0 && fini_array_count == 0) {
        return;
    }
    fprintf(c_out,
        "static void call_fini_entries(uintptr_t base) {\n"
    );
    if (fini_array_count == 1) {
        fprintf(c_out,
            "  ((embedded_fini_fn)(*(uintptr_t *)(base + 0x%lxUL)))();\n",
            (unsigned long)sections->fini_array_vaddr
        );
    } else if (fini_array_count > 1) {
        fprintf(c_out,
            "  uintptr_t *fini_array = (uintptr_t *)(base + 0x%lxUL);\n"
            "  for (size_t i = %luUL; i > 0; i--) {\n"
            "    if (fini_array[i - 1] != 0) {\n"
            "      ((embedded_fini_fn)fini_array[i - 1])();\n"
            "    }\n"
            "  }\n",
            (unsigned long)sections->fini_array_vaddr,
            (unsigned long)fini_array_count
        );
    }
    if (sections->fini_vaddr != 0) {
        fprintf(c_out,
            "  ((embedded_fini_fn)(base + 0x%lxUL))();\n",
            (unsigned long)sections->fini_vaddr
        );
    }
    fprintf(c_out, "}\n\n");
}


int process_elf_segments(Elf_Blob *elf_blob, Elf_Sections *sections, char *blob_file) {
    // This requires proper endian decoding...
    //if (get_elf_uint32(elf_blob, EI_VERSION) != EV_CURRENT) {
    //    fprintf(stderr, "Unsupported ELF version\n");
    //    return 1;
    //}

    FILE *blob_out = fopen(blob_file, "wb");
    if (!blob_out) {
        perror("blob out");
        return 1;
    }
    if (get_elf_class(elf_blob) != ELFCLASS64) {
        fprintf(stderr, "Only 64-bit ELF files are supported.\n");
        fclose(blob_out);
        return 1;
    }

    fprintf(stderr, "Processing 64-bit ELF file\n");
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_blob->blob;
    if (ehdr->e_type != ET_DYN) {
        fprintf(stderr, "Only ET_DYN shared objects are supported by this extractor (requires building as so, not static).\n");
        fclose(blob_out);
        return 1;
    }

    Elf32_Word phoff = ehdr->e_phoff;
    Elf32_Half phentsz = ehdr->e_phentsize;
    Elf32_Half phnum = ehdr->e_phnum;
    int blob_pos = 0;
    int saw_tls = 0;

    sections->e_machine = ehdr->e_machine;
    sections->min_vaddr = 0;
    sections->max_vaddr = 0;
    if (phnum == PN_XNUM) {
        fprintf(stderr, "ELF file has an extended program header count (PN_XNUM), which is not supported.\n");
        fclose(blob_out);
        return 1;
    }

    sections->load = malloc(phnum * sizeof(Elf_Section));
    if (!sections->load) {
        perror("Failed to allocate PT_LOAD metadata");
        fclose(blob_out);
        return 1;
    }
    sections->load_count = 0;

    sections->relro = malloc(phnum * sizeof(Elf_Section));
    if (!sections->relro) {
        perror("Failed to allocate PT_GNU_RELRO metadata");
        fclose(blob_out);
        return 1;
    }
    sections->relro_count = 0;

    for (int i = 0; i < phnum; i++) {
        int p = phoff + (i * phentsz);
        Elf64_Phdr *phdr = (Elf64_Phdr *)(elf_blob->blob + p);
        Elf64_Word type = phdr->p_type;
        Elf64_Word memsz = phdr->p_memsz;

        switch (type) {
            case PT_TLS:
                saw_tls = 1;
                break;
            case PT_LOAD:
                blob_pos = write_section_blob64(
                    elf_blob,
                    &(sections->load[sections->load_count]),
                    blob_out,
                    i,
                    phdr,
                    blob_pos
                );
                if (blob_pos < 0) {
                    fclose(blob_out);
                    return 1;
                }
                sections->min_vaddr = compute_min_memory(sections->min_vaddr, phdr->p_vaddr);
                sections->max_vaddr = compute_max_memory(sections->max_vaddr, phdr->p_vaddr, memsz);
                sections->load_count++;
                break;
            case PT_DYNAMIC:
                if (process_dynamic_section(elf_blob, sections, phdr)) {
                    fclose(blob_out);
                    return 1;
                }
                break;
#ifdef PT_GNU_RELRO
            case PT_GNU_RELRO:
                sections->relro[sections->relro_count].ehdr64.segment_index = i;
                sections->relro[sections->relro_count].ehdr64.vaddr = phdr->p_vaddr;
                sections->relro[sections->relro_count].ehdr64.tb_memory_size = memsz;
                set_section_flag_64(&(sections->relro[sections->relro_count]), phdr);
                sections->relro_count++;
                break;
#endif
#ifdef PT_GNU_STACK
            case PT_GNU_STACK:
                break;
#endif
            default:
                break;
        }
    }

    if (sections->load_count == 0) {
        fprintf(stderr, "No PT_LOAD segments found.\n");
        fclose(blob_out);
        return 1;
    }
    if (saw_tls) {
        fprintf(stderr, "PT_TLS is not supported by the shared-object loader prototype.\n");
        fclose(blob_out);
        return 1;
    }
    if (sections->dynsym_vaddr == 0 || sections->dynstr_vaddr == 0 || sections->dynsym_ent_size != sizeof(Elf64_Sym)) {
        fprintf(stderr, "Shared object is missing required dynamic symbol metadata.\n");
        fclose(blob_out);
        return 1;
    }

    fclose(blob_out);
    return 0;
}

void free_elf_sections(Elf_Sections *sections) {
    if (!sections) {
        return;
    }
    if (sections->load) {
        free(sections->load);
        sections->load = NULL;
    }
    sections->load_count = 0;
    if (sections->relro) {
        free(sections->relro);
        sections->relro = NULL;
    }
    sections->relro_count = 0;
}


// --------------------------------------------------------------------
// -- Main program.
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input-elf> <output-c> <output-blob>\n", argv[0]);
        return 1;
    }
    FILE *c_out = fopen(argv[2], "w");
    if (!c_out) {
        perror("Failed to open output C file");
        return 1;
    }

    // --- read entire ELF into memory
    Elf_Blob elf_blob = {.blob = NULL, .size = 0};
    if (read_elf_blob(argv[1], &elf_blob)) {
        fprintf(stderr, "Failed to read ELF blob from %s\n", argv[1]);
        free_elf_blob(&elf_blob);
        fclose(c_out);
        return 1;
    }

    // --- blob and section processing
    Elf_Sections sections = {
        .load = NULL,
        .load_count = 0,
        .relro = NULL,
        .relro_count = 0,
    };
    if (process_elf_segments(&elf_blob, &sections, argv[3])) {
        fprintf(stderr, "Failed to process ELF segments from %s into blob %s\n", argv[1], argv[3]);
        free_elf_blob(&elf_blob);
        free_elf_sections(&sections);
        fclose(c_out);
        return 1;
    }

    Used_Import *used_imports = NULL;
    size_t used_import_count = 0;
    Elf64_Addr main_symbol = 0;
    Relocation_Types reloc_types = {0, 0, 0, 0, 0};
    if (find_symbol_value(&elf_blob, "main", &main_symbol)) {
        fprintf(stderr, "Failed to locate main() in shared-object payload.\n");
        free_elf_blob(&elf_blob);
        free_elf_sections(&sections);
        fclose(c_out);
        return 1;
    }
    if (get_relocation_types(sections.e_machine, &reloc_types)) {
        free_elf_blob(&elf_blob);
        free_elf_sections(&sections);
        fclose(c_out);
        return 1;
    }
    if (collect_shared_imports(&elf_blob, &sections, &used_imports, &used_import_count)) {
        free_elf_blob(&elf_blob);
        free_elf_sections(&sections);
        fclose(c_out);
        return 1;
    }

    // --- start .c loader
    fprintf(c_out,
        "#define _GNU_SOURCE\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "#include <stdint.h>\n"
        "#include <elf.h>\n"
        "#include <arpa/inet.h>\n"
        "#include <dirent.h>\n"
        "#include <fcntl.h>\n"
        "#include <grp.h>\n"
        "#include <netdb.h>\n"
        "#include <signal.h>\n"
        "#include <sys/mman.h>\n"
        "#include <sys/socket.h>\n"
        "#include <sys/stat.h>\n"
        "#include <sys/sysmacros.h>\n"
        "#include <sys/types.h>\n"
        "#include <sys/wait.h>\n"
        "#include <time.h>\n"
        "#include <unistd.h>\n"
        "#include \"stub-common.h\"\n"
        "#include \"stub-run.h\"\n\n"
        // Ensure page alignment, as required by mprotect.
        "static uintptr_t page_floor(uintptr_t value, uintptr_t page_size) {\n"
        "  return value & ~(page_size - 1);\n"
        "}\n\n"
        "static uintptr_t page_ceil(uintptr_t value, uintptr_t page_size) {\n"
        "  return (value + page_size - 1) & ~(page_size - 1);\n"
        "}\n\n"
        "static int map_segment(uintptr_t vaddr, uintptr_t filesz, uintptr_t memsz,\n"
        "    unsigned char *src, uintptr_t page_size) {\n"
        "  uintptr_t start = page_floor(vaddr, page_size);\n"
        "  uintptr_t end = page_ceil(vaddr + memsz, page_size);\n"
        "  void *mapped = mmap((void *)start, end - start,\n"
        "      PROT_READ|PROT_WRITE|PROT_EXEC,\n"
        "      MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,\n"
        "      -1, 0);\n"
        "  if (mapped == MAP_FAILED) {\n"
        "    return 0;\n"
        "  }\n"
        "  memcpy((void *)vaddr, src, filesz);\n"
        "  return 1;\n"
        "}\n\n"
        "static int protect_segment(uintptr_t vaddr, uintptr_t memsz, int prot,\n"
        "    uintptr_t page_size) {\n"
        "  uintptr_t start = page_floor(vaddr, page_size);\n"
        "  uintptr_t end = page_ceil(vaddr + memsz, page_size);\n"
        "  return mprotect((void *)start, end - start, prot) == 0;\n"
        "}\n\n"
    );

    emit_used_import_resolver(c_out, used_imports, used_import_count);
    fprintf(c_out,
        "#define EMBEDDED_RELOC_RELATIVE %luUL\n"
        "#define EMBEDDED_RELOC_GLOB_DAT %luUL\n"
        "#define EMBEDDED_RELOC_JUMP_SLOT %luUL\n"
        "#define EMBEDDED_RELOC_ABS64 %luUL\n"
        "#define EMBEDDED_RELOC_IRELATIVE %luUL\n\n"
        "typedef int (*embedded_main_fn)(int, char **);\n"
        "typedef void (*embedded_init_fn)(int, char **, char **);\n"
        "typedef void (*embedded_fini_fn)(void);\n\n"
        "static uintptr_t resolve_symbol_value(uintptr_t base, const Elf64_Sym *sym, const char *dynstr, int *ok) {\n"
        "  if (sym->st_shndx != SHN_UNDEF) {\n"
        "    return base + (uintptr_t)sym->st_value;\n"
        "  }\n"
        "  uintptr_t value = resolve_import_symbol(dynstr + sym->st_name);\n"
        "  if (value == 0 && ELF64_ST_BIND(sym->st_info) != STB_WEAK) {\n"
        "    *ok = 0;\n"
        "  }\n"
        "  return value;\n"
        "}\n\n"
        "static int apply_rela_table(uintptr_t base, const Elf64_Sym *dynsym,\n"
        "    const char *dynstr, const Elf64_Rela *rela, size_t rela_count) {\n"
        "  for (size_t i = 0; i < rela_count; i++) {\n"
        "    unsigned long type = ELF64_R_TYPE(rela[i].r_info);\n"
        "    unsigned long sym_index = ELF64_R_SYM(rela[i].r_info);\n"
        "    uintptr_t *target = (uintptr_t *)(base + (uintptr_t)rela[i].r_offset);\n"
        "    switch (type) {\n"
        "      case 0:\n"
        "        break;\n"
        "      case EMBEDDED_RELOC_RELATIVE:\n"
        "        *target = base + (uintptr_t)rela[i].r_addend;\n"
        "        break;\n"
        "      case EMBEDDED_RELOC_GLOB_DAT:\n"
        "      case EMBEDDED_RELOC_JUMP_SLOT:\n"
        "      case EMBEDDED_RELOC_ABS64: {\n"
        "        int ok = 1;\n"
        "        uintptr_t value = 0;\n"
        "        if (sym_index != 0) {\n"
        "          value = resolve_symbol_value(base, &dynsym[sym_index], dynstr, &ok);\n"
        "        }\n"
        "        if (!ok) {\n"
        "          return 0;\n"
        "        }\n"
        "        *target = value + (uintptr_t)rela[i].r_addend;\n"
        "        break;\n"
        "      }\n"
        "      case EMBEDDED_RELOC_IRELATIVE: {\n"
        "        uintptr_t (*resolver_fn)(void) = (uintptr_t (*)(void))(base + (uintptr_t)rela[i].r_addend);\n"
        "        *target = resolver_fn();\n"
        "        break;\n"
        "      }\n"
        "      default:\n"
        "        return 0;\n"
        "    }\n"
        "  }\n"
        "  return 1;\n"
        "}\n\n",
        reloc_types.relative,
        reloc_types.glob_dat,
        reloc_types.jump_slot,
        reloc_types.abs64,
        reloc_types.irelative
    );
    emit_init_helper(c_out, &sections);
    emit_fini_helper(c_out, &sections);

    fprintf(c_out,
        "int run_embedded(unsigned char *data, unsigned int dataLen, int argc, char *argv[], char *envp[]) {\n"
        "  (void)dataLen;\n"
        "  long raw_page_size = sysconf(_SC_PAGESIZE);\n"
        "  uintptr_t page_size = raw_page_size > 0 ? (uintptr_t)raw_page_size : 4096;\n"
        "  uintptr_t reserve_start = page_floor(0x%lxUL, page_size);\n"
        "  uintptr_t reserve_end = page_ceil(0x%lxUL, page_size);\n"
        "  uintptr_t reserve_size = reserve_end - reserve_start;\n"
        "  unsigned char *reservation = mmap(NULL, reserve_size, PROT_NONE,\n"
        "      MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);\n"
        "  if (reservation == MAP_FAILED) {\n"
        "    WRITE_LAUNCH_ERROR();\n"
        "    return 70;\n"
        "  }\n"
        "  uintptr_t base = (uintptr_t)reservation - reserve_start;\n",
        (unsigned long)sections.min_vaddr,
        (unsigned long)sections.max_vaddr
    );

    for (unsigned int i = 0; i < sections.load_count; i++) {
        Elf_Section *load_section = &(sections.load[i]);
        fprintf(c_out,
            "  if (!map_segment(base + 0x%lxUL, 0x%lxUL, 0x%lxUL, data + 0x%x, page_size)) {\n"
            "    WRITE_LAUNCH_ERROR();\n"
            "    return 71;\n"
            "  }\n",
            (unsigned long)load_section->ehdr64.vaddr,
            (unsigned long)load_section->ehdr64.td_blob_size,
            (unsigned long)load_section->ehdr64.tb_memory_size,
            load_section->ehdr64.blob_offset
        );
    }

    fprintf(c_out,
        "  Elf64_Sym *dynsym = (Elf64_Sym *)(base + 0x%lxUL);\n"
        "  const char *dynstr = (const char *)(base + 0x%lxUL);\n"
        "  if (!apply_rela_table(base, dynsym, dynstr, (const Elf64_Rela *)(base + 0x%lxUL), %luUL)) {\n"
        "    WRITE_LAUNCH_ERROR();\n"
        "    return 72;\n"
        "  }\n"
        "  if (!apply_rela_table(base, dynsym, dynstr, (const Elf64_Rela *)(base + 0x%lxUL), %luUL)) {\n"
        "    WRITE_LAUNCH_ERROR();\n"
        "    return 73;\n"
        "  }\n",
        (unsigned long)sections.dynsym_vaddr,
        (unsigned long)sections.dynstr_vaddr,
        (unsigned long)sections.rela_vaddr,
        (unsigned long)(sections.rela_size / sizeof(Elf64_Rela)),
        (unsigned long)sections.jmprel_vaddr,
        (unsigned long)(sections.jmprel_size / sizeof(Elf64_Rela))
    );

    for (unsigned int i = 0; i < sections.load_count; i++) {
        Elf_Section *load_section = &(sections.load[i]);
        fprintf(c_out,
            "  if (!protect_segment(base + 0x%lxUL, 0x%lxUL, %s, page_size)) {\n"
            "    WRITE_LAUNCH_ERROR();\n"
            "    return 74;\n"
            "  }\n",
            (unsigned long)load_section->ehdr64.vaddr,
            (unsigned long)load_section->ehdr64.tb_memory_size,
            load_section->ehdr64.final_memory_flag
        );
    }

    for (unsigned int i = 0; i < sections.relro_count; i++) {
        Elf_Section *relro_section = &(sections.relro[i]);
        fprintf(c_out,
            "  if (!protect_segment(base + 0x%lxUL, 0x%lxUL, %s, page_size)) {\n"
            "    WRITE_LAUNCH_ERROR();\n"
            "    return 75;\n"
            "  }\n",
            (unsigned long)relro_section->ehdr64.vaddr,
            (unsigned long)relro_section->ehdr64.tb_memory_size,
            relro_section->ehdr64.final_memory_flag
        );
    }

    if (sections.init_vaddr != 0 || get_init_array_count(&sections) > 0) {
        fprintf(c_out,
            "  call_init_entries(base, argc, argv, envp);\n"
        );
    }
    fprintf(c_out,
        "  embedded_main_fn main_fn = (embedded_main_fn)(base + 0x%lxUL);\n"
        "  int ret = main_fn(argc, argv);\n",
        (unsigned long)main_symbol
    );
    if (sections.fini_vaddr != 0 || get_fini_array_count(&sections) > 0) {
        fprintf(c_out,
            "  call_fini_entries(base);\n"
        );
    }
    fprintf(c_out,
        "  return ret;\n"
        "}\n\n"
    );

    free(used_imports);

    // finish up
    free_elf_blob(&elf_blob);
    fclose(c_out);
    fprintf(stderr,
        "ELF extraction complete. C library written to %s, data blob to %s\n",
        argv[2],
        argv[3]
    );
    return 0;
}
