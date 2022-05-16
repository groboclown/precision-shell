/*
 * Creates lines of C code that has a constant with encoded binary data of
 * another file.
 * 
 * A C version of bin2c.sh, which is very, very slow.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

#define INITIAL_SPACE 4
#define ITEM_SIZE 6
#define COLUMN_SIZE 80
#define VALUES_PER_LINE ((COLUMN_SIZE - INITIAL_SPACE) / ITEM_SIZE)


int main(int argc, char *argv[])
{
    uint8_t *buf;
    char *const_name;
    unsigned int i, file_size, read_count, need_comma;

    FILE *f_input, *f_output;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s (binary file) (name of constant) (output file)\n", argv[0]);
        return 1;
    }
    const_name = argv[2];

    f_input = fopen(argv[1], "rb");
    if (f_input == NULL) {
        fprintf(stderr, "%s: can't open %s for reading\n", argv[0], argv[1]);
        return 2;
    }

    buf = (uint8_t *) malloc(BUFFER_SIZE);
    if (buf == NULL) {
        fprintf(stderr, "%s: No free memory", argv[0]);
        return 3;
    }

    f_output = fopen(argv[3], "w");
    if (f_output == NULL) {
        fclose(f_input);
        fprintf(stderr, "%s: can't open %s for writing\n", argv[0], argv[2]);
        return -1;
    }

    need_comma = 0;

    fprintf(f_output, "const uint8_t %s[] = {\n    ", const_name);
    file_size = 0;
    while (1) {
        read_count = fread(buf, sizeof(uint8_t), BUFFER_SIZE, f_input);
        if (read_count <= 0) {
            break;
        }
        for (i = 0; i < read_count; i++) {
            file_size++;
            if (need_comma) {
                fprintf(f_output, ", ");
            }
            need_comma = 1;

            if ((file_size % VALUES_PER_LINE) == 0) {
                fprintf(f_output, "\n    ");
            }
            fprintf(f_output, "0x%.2x", buf[i] & 0xff);
        }
    }
    fprintf(f_output, "\n};\n\n");
    fprintf(f_output, "#define SIZE_%s %i\n\n", const_name, file_size);

    fclose(f_input);
    fclose(f_output);

    return 0;
}
