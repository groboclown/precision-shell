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


/*
 * Transform an ELF file into:
 *  1) A compressed binary blob, stored as a separate file.
 *     The user can use some external program to compress this blob.
 *  2) A c file that knows how to take the passed-in byte array and run it.
 *  3) A header file that defines the c file's function.
 * 
 * See https://refspecs.linuxfoundation.org/elf/elf.pdf
 * 
 * This will fail on NEEDED and DYNAMIC sections, which are not supported.
 */

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <string.h>
#include <libgen.h>
#include "extract-elf.h"


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
        fclose(blob_out);
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


int append_rela_array(Blob64_Rela_Array *rela, Elf64_Rela *new_rela) {
    // There's a bug in here that's overwriting memory.
    if (rela->rela == NULL) {
        rela->alloc = 30;
        rela->rela = malloc(rela->alloc * sizeof(Blob64_Rela));
        if (!rela->rela) {
            perror("Failed to allocate RELA array");
            return 1;
        }
    } else if (rela->count >= rela->alloc) {
        rela->alloc *= 2;
        Blob64_Rela *malloc_rela = realloc(rela->rela, rela->alloc * sizeof(Blob64_Rela));
        if (!malloc_rela) {
            perror("Failed to resize RELA array");
            return 1;
        }
        rela->rela = malloc_rela;
    }
    // Because this bit of meta-data is stuffed into the blob and not the source, tell
    // the audience what's being stored for debugging purposes.
    fprintf(stderr, "Adding RELA R_X86_64_IRELATIVE entry: r_offset: %lx,  r_addend: %lx\n", new_rela->r_offset, new_rela->r_addend);
    rela->rela[rela->count].r_addend = new_rela->r_addend;
    rela->rela[rela->count].r_offset = new_rela->r_offset;
    rela->count++;
    rela->size += sizeof(Blob64_Rela);
    return 0;
}

void free_rela_array(Blob64_Rela_Array *rela) {
    if (rela) {
        if (rela->rela) {
            free(rela->rela);
            rela->rela = NULL;
        }
    }
}


unsigned int compute_max_memory(unsigned int prev_max_mem, unsigned int vaddr, unsigned int memsz) {
    unsigned int end_addr = vaddr + memsz;
    if (end_addr > prev_max_mem) {
        return end_addr;
    }
    return prev_max_mem;
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
    int blob_pos = 0;

    if (get_elf_class(elf_blob) == ELFCLASS32) {
        fprintf(stderr, "Processing 32-bit ELF file\n");
        fprintf(stderr, "... is not supported yet.\n");
        fclose(blob_out);
        return 1;
    } else if (get_elf_class(elf_blob) == ELFCLASS64) {
        fprintf(stderr, "Processing 64-bit ELF file\n");
        Elf64_Ehdr *ehdr = (Elf64_Ehdr*)elf_blob->blob;
        sections->entry = ehdr->e_entry;
        sections->e_class = ELFCLASS64;
        Elf32_Word phoff   = ehdr->e_phoff;
        Elf32_Half phentsz = ehdr->e_phentsize;
        Elf32_Half phnum   = ehdr->e_phnum;
        if (phnum == PN_XNUM) {
            fprintf(stderr, "ELF file has an extended program header count (PN_XNUM), which is not supported.\n");
            fclose(blob_out);
            return 1;
        }

        // Initialize section arrays
        // pre-allocate rela size based on the 
        Blob64_Rela_Array rela = {.rela = NULL, .count = 0, .alloc = 0, .size = 0};

        // pre-allocate segment size arrays to the maximum size.
        sections->tls = NULL;
        sections->max_memory_size = 0;
        sections->load = malloc(phnum * sizeof(Elf_Section));
        sections->load_count = 0;
        sections->rela_count = 0;
        sections->rela_blob_offset = 0;
        sections->relro = malloc(phnum * sizeof(Elf_Section));
        sections->relro_count = 0;

        // Handle program headers.
        for(int i = 0; i < phnum; i++){
            int p = phoff + i*phentsz;
            Elf64_Phdr *phdr = (Elf64_Phdr*)(elf_blob->blob + p);
            Elf64_Word type = phdr->p_type;
            Elf64_Word memsz  = phdr->p_memsz;

            switch (type) {
                case PT_TLS:
                    if (sections->tls) {
                        fprintf(stderr, "Multiple PT_TLS segments found, only one is supported.\n");
                        fclose(blob_out);
                        return 1;
                    }
                    sections->tls = malloc(sizeof(Elf_Section));
                    blob_pos = write_section_blob64(elf_blob, sections->tls, blob_out, i, phdr, blob_pos);
                    if (blob_pos < 0) {
                        fclose(blob_out);
                        return 1;
                    }
                    sections->max_memory_size = compute_max_memory(sections->max_memory_size, phdr->p_vaddr, memsz);
                    break;
                case PT_LOAD:
                    blob_pos = write_section_blob64(elf_blob, &(sections->load[sections->load_count]), blob_out, i, phdr, blob_pos);
                    if (blob_pos < 0) {
                        fclose(blob_out);
                        return 1;
                    }
                    sections->max_memory_size = compute_max_memory(sections->max_memory_size, phdr->p_vaddr, memsz);
                    sections->load_count++;
                    break;
#ifdef PT_GNU_RELRO
                case PT_GNU_RELRO:
                    // This contains list of segments that are read-only after relocation.
                    // The main thing that matters here is the segment address & size.
                    sections->relro[sections->relro_count].ehdr64.segment_index = i;
                    sections->relro[sections->relro_count].ehdr64.vaddr = phdr->p_vaddr;
                    sections->relro[sections->relro_count].ehdr64.tb_memory_size = memsz;
                    set_section_flag_64(&(sections->relro[sections->relro_count]), phdr);
                    break;
#endif
#ifdef PT_GNU_STACK
                case PT_GNU_STACK:
                    // This indicates that the stack is executable.
                    // This should never be set, as it is a security risk, and modern
                    // systems will not allow it.
                    break;
#endif
            }
        }

        // Handle sections.
        Elf64_Shdr *shdr = (Elf64_Shdr *)(elf_blob->blob + ehdr->e_shoff);
        for (int i = 0; i < ehdr->e_shnum; i++) {
            if (shdr[i].sh_type == SHT_REL) {
                // REL section
                fprintf(stderr, "ELF contains REL section, which is not supported\n");
                fclose(blob_out);
                free_rela_array(&rela);
                return 1;
            } else if (shdr[i].sh_type == SHT_RELA) {
                // RELA section
                // These are custom data structures written to our blob, that store
                // only the required information.
                for (int j = 0; j < shdr[i].sh_size; j += shdr[i].sh_entsize) {
                    Elf64_Rela *rel = (Elf64_Rela *)(elf_blob->blob + shdr[i].sh_offset + j);
                    switch (ELF64_R_TYPE(rel->r_info)) {
                        case R_X86_64_IRELATIVE:
                            if (append_rela_array(&rela, rel)) {
                                fclose(blob_out);
                                free_rela_array(&rela);
                                return 1;
                            }
                            break;
                        default:
                            fprintf(stderr, "Unsupported RELA relocation type: %lu\n", ELF64_R_TYPE(rel->r_info));
                            fclose(blob_out);
                            free_rela_array(&rela);
                            return 1;
                    }
                }
            }
        }

        if (rela.size > 0) {
            sections->rela_count = rela.count;
            sections->rela_blob_offset = blob_pos;
            if (fwrite(rela.rela, 1, rela.size, blob_out) != rela.size) {
                perror("Failed to write RELA entries to blob file");
                free_rela_array(&rela);
                fclose(blob_out);
                return 1;
            }
            blob_pos += rela.size;
        }
        free_rela_array(&rela);

        // Completed working with the blob.
        fclose(blob_out);
    } else {
        fprintf(stderr, "Only 32-bit and 64-bit ELF files are supported\n");
        fclose(blob_out);
        return 1;
    }
    return 0;
}

void free_elf_sections(Elf_Sections *sections) {
    if (!sections) {
        return;
    }
    if (sections->tls) {
        free(sections->tls);
        sections->tls = NULL;
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
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input-elf> <output-c> <output-h> <output-blob>\n", argv[0]);
        return 1;
    }
    FILE *c_out = fopen(argv[2], "w");
    if (!c_out) {
        perror("Failed to open output C file");
        return 1;
    }

    // --- emit .h
    FILE *h_out = fopen(argv[3], "w");
    if (!h_out) {
        perror("Failed to open output h file");
        fclose(c_out);
        return 1;
    }
    fprintf(h_out,
        "#ifndef EMBEDDED_LOADER_H\n"
        "#define EMBEDDED_LOADER_H\n\n"
        "#define RUN_ERROR_LEN 17\n"
        "static char RUN_ERROR[RUN_ERROR_LEN];\n"
        "/* call this to map+run the embedded ELF blocks. */\n"
        "int run_embedded(unsigned char *data);\n\n"
        "#endif\n");
    fclose(h_out);

    // --- read entire ELF into memory
    Elf_Blob elf_blob = {.blob = NULL, .size = 0};
    if (read_elf_blob(argv[1], &elf_blob)) {
        fprintf(stderr, "Failed to read ELF blob from %s\n", argv[1]);
        free_elf_blob(&elf_blob);
        return 1;
    }

    // --- blob and section processing
    Elf_Sections sections = {
        .tls = NULL,
        .load = NULL,
        .load_count = 0,
        .relro = NULL,
        .relro_count = 0,
        .rela_count = 0,
        .rela_blob_offset = 0,
        .max_memory_size = 0,
        .e_class = ELFCLASSNONE, // Default to none
    };
    if (process_elf_segments(&elf_blob, &sections, argv[4])) {
        fprintf(stderr, "Failed to process ELF segments from %s into blob %s\n", argv[1], argv[4]);
        free_elf_blob(&elf_blob);
        free_elf_sections(&sections);
        fclose(c_out);
        return 1;
    }

    // --- start .c loader
    fprintf(c_out,
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "#include <stdint.h>\n"
        "#include <unistd.h>\n"
        "#include <sys/mman.h>\n"
        "#include \"%s\"\n\n"
        ,
        basename(argv[3])
    );
    if (sections.rela_count > 0) {
        // The structure is ELF class agnostic.
        fprintf(c_out,
            "typedef struct {\n"
                "  uint64_t r_offset;\n" // Elf64_Addr
                "  int64_t  r_addend;\n" // Elf64_Sxword
            "} Elf_Rela;\n\n"
        );
    }

    // --- emit loader runner ---

    fprintf(c_out,
        "static char RUN_ERROR[] = \"Launcher failed.\\n\";\n\n"

        // The primary runner
        "int run_embedded(unsigned char *data) {\n"

        // Allocate the executable space.
        "  void *base = mmap(NULL, %u,\n"
        "                    PROT_READ|PROT_WRITE|PROT_EXEC,\n"
        "                    MAP_PRIVATE|MAP_ANONYMOUS,\n"
        "                    0, 0);\n"
        "  if (!base) { write(STDERR_FILENO, RUN_ERROR, RUN_ERROR_LEN); return 10; }\n"

        // Start with clean memory.  This could be done piecewise, but this is smaller.
        "  memset(base, 0, %u);\n\n"
        ,
        sections.max_memory_size,
        sections.max_memory_size,
        sections.max_memory_size
    );

    // copy PT_LOAD segments ...
    for (unsigned int i = 0; i < sections.load_count; i++) {
        Elf_Section *load_section = &(sections.load[i]);
        if (get_elf_class(&elf_blob) == ELFCLASS32) {
            fprintf(c_out,
                "  // copy PT_LOAD segment %u\n"
                "  memcpy(base + 0x%lx, data + 0x%x, 0x%x);\n"
                ,
                load_section->ehdr32.segment_index,
                load_section->ehdr32.vaddr,
                load_section->ehdr32.blob_offset,
                load_section->ehdr32.td_blob_size
            );
        } else {
            fprintf(c_out,
                "  // copy PT_LOAD segment %u\n"
                "  memcpy(base + 0x%lx, data + 0x%x, 0x%x);\n"
                ,
                load_section->ehdr64.segment_index,
                load_section->ehdr64.vaddr,
                load_section->ehdr64.blob_offset,
                load_section->ehdr64.td_blob_size
            );
        }
    }

    // TLS setup
    if(sections.tls != NULL) {
        if (get_elf_class(&elf_blob) == ELFCLASS32) {
            fprintf(c_out,
                "  // setup TLS\n"
                "  memcpy(base + 0x%lx, data + 0x%x, 0x%x);\n"
                "  mprotect((unsigned char *)(base + 0x%lx), 0x%x, PROT_READ|PROT_WRITE);\n\n"
                ,
                sections.tls->ehdr32.vaddr,
                sections.tls->ehdr32.blob_offset,
                sections.tls->ehdr32.td_blob_size,
                sections.tls->ehdr32.vaddr,
                sections.tls->ehdr32.tb_memory_size
            );
        } else {
            fprintf(c_out,
                "  // setup TLS\n"
                "  memcpy(base + 0x%lx, data + 0x%x, 0x%x);\n"
                "  mprotect((unsigned char *)(base + 0x%lx), 0x%x, PROT_READ|PROT_WRITE);\n\n"
                ,
                sections.tls->ehdr64.vaddr,
                sections.tls->ehdr64.blob_offset,
                sections.tls->ehdr64.td_blob_size,
                sections.tls->ehdr64.vaddr,
                sections.tls->ehdr64.tb_memory_size
            );
        }
    }

    // apply REL (add base to *P)
    // This just adds the base address to the relocation offset.

    // apply RELA
    if (sections.rela_count > 0) {
        fprintf(c_out,
            "  // apply RELA relocations\n"
            "  Elf_Rela *rela_entries = (Elf_Rela *)(data + %u);\n"
            "  for (int i = 0; i < %u; i++) {\n"
            // "    fprintf(stderr, \"Applying RELA entry: r_offset: 0x%%lx, r_addend: 0x%%lx\\n\", rela_entries[i].r_offset, rela_entries[i].r_addend);\n"
            "    uintptr_t (*addend_fn)() = (uintptr_t (*)())((uintptr_t)(base + rela_entries[i].r_addend));\n"
            "    uintptr_t *into = (uintptr_t *)(base + rela_entries[i].r_offset);\n"
            "    *into = addend_fn();\n"
            // "    fprintf(stderr, \" -> turned into 0x%%lx\\n\", *into - (uintptr_t)base);\n"
            "  }\n\n"
            ,
            sections.rela_blob_offset,
            sections.rela_count
        );
    } else {
        fprintf(c_out, "  // no RELA relocations to apply\n");
    }

    // After relocations, apply LOAD protections.

    // copy PT_LOAD segments ...
    for (unsigned int i = 0; i < sections.load_count; i++) {
        Elf_Section *load_section = &(sections.load[i]);
        if (get_elf_class(&elf_blob) == ELFCLASS32) {
            fprintf(c_out,
            "  // protect PT_LOAD segment %u\n"
            "  mprotect((unsigned char *)(base + 0x%lx), 0x%x, %s);\n\n"
            ,
            load_section->ehdr32.segment_index,
            load_section->ehdr32.vaddr,
            load_section->ehdr32.tb_memory_size,
            load_section->ehdr32.final_memory_flag
            );
        } else {
            fprintf(c_out,
            "  // protect PT_LOAD segment %u\n"
            "  mprotect((unsigned char *)(base + 0x%lx), 0x%x, %s);\n\n"
            ,
            load_section->ehdr64.segment_index,
            load_section->ehdr64.vaddr,
            load_section->ehdr64.tb_memory_size,
            load_section->ehdr64.final_memory_flag
            );
        }
    }

    // After relocations, apply RELRO segments.
    for (unsigned int i = 0; i < sections.relro_count; i++) {
        Elf_Section *relro_section = &(sections.relro[i]);
        if (get_elf_class(&elf_blob) == ELFCLASS32) {
            fprintf(c_out,
            "  // apply RELRO segment %u\n"
            "  mprotect((unsigned char *)(base + 0x%lx), %s);\n\n"
            ,
            relro_section->ehdr32.segment_index,
            relro_section->ehdr32.vaddr,
            relro_section->ehdr32.final_memory_flag
            );
        } else {
            fprintf(c_out,
            "  // apply RELRO segment %u\n"
            "  mprotect((unsigned char *)(base + 0x%lx), %s);\n\n"
            ,
            relro_section->ehdr64.segment_index,
            relro_section->ehdr64.vaddr,
            relro_section->ehdr64.final_memory_flag
            );
        }
    }

    // jump to entry...
    fprintf(c_out,
        "  void (*entry_fn)() = (void(*)())(base + 0x%lx);\n"
        "  entry_fn();\n"
        "  // unreachable, but needed by the compiler.\n"
        "  return 0;\n"
        "}\n\n",
        sections.entry
    );

    // finish up
    free_elf_blob(&elf_blob);
    fclose(c_out);
    fprintf(stderr,
        "ELF extraction complete. C library written to %s, header to %s, data blob to %s\n",
        argv[2],
        argv[3],
        argv[4]
    );
    return 0;
}
