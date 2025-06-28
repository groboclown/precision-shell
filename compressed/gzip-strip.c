// Strip the gzip header and tail, and use the raw DEFLATE data.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define GZIP_MAGIC0 0x1f
#define GZIP_MAGIC1 0x8b
#define CM_DEFLATE  8

// gzip FLG bits
#define FLG_FTEXT    0x01
#define FLG_FHCRC    0x02
#define FLG_FEXTRA   0x04
#define FLG_FNAME    0x08
#define FLG_FCOMMENT 0x10

static void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.gz output.deflate\n", argv[0]);
        return 1;
    }

    FILE *fin  = fopen(argv[1], "rb");
    FILE *fout = fopen(argv[2], "wb");
    if (!fin)  die("fopen input");
    if (!fout) die("fopen output");

    // 1) Read and validate fixed 10-byte header
    uint8_t hdr[10];
    if (fread(hdr, 1, 10, fin) != 10) die("read header");
    if (hdr[0] != GZIP_MAGIC0 || hdr[1] != GZIP_MAGIC1 || hdr[2] != CM_DEFLATE) {
        fprintf(stderr, "Not a gzip/DEFLATE file\n");
        return 1;
    }
    uint8_t flg = hdr[3];

    // 2) Skip MTIME(4), XFL(1), OS(1) already in hdr[4..9]

    // 3) Skip extra fields if present
    if (flg & FLG_FEXTRA) {
        uint16_t xlen;
        if (fread(&xlen, 2, 1, fin) != 1) die("read xlen");
        // xlen is little-endian
        fseek(fin, xlen, SEEK_CUR);
    }

    // 4) Skip original filename if present
    if (flg & FLG_FNAME) {
        int c;
        while ((c = fgetc(fin)) != EOF && c != 0) { }
        if (c != 0) die("skip fname");
    }

    // 5) Skip file comment if present
    if (flg & FLG_FCOMMENT) {
        int c;
        while ((c = fgetc(fin)) != EOF && c != 0) { }
        if (c != 0) die("skip fcomment");
    }

    // 6) Skip header CRC16 if present
    if (flg & FLG_FHCRC) {
        fseek(fin, 2, SEEK_CUR);
    }

    // 7) Calculate how many bytes remain before the 8-byte trailer
    long data_start = ftell(fin);
    if (data_start < 0) die("ftell");
    fseek(fin, 0, SEEK_END);
    long total_size = ftell(fin);
    if (total_size < data_start + 8) {
        fprintf(stderr, "File too small\n");
        return 1;
    }
    long data_len = total_size - data_start - 8;

    // 8) Copy raw DEFLATE bytes
    fseek(fin, data_start, SEEK_SET);
    #define BUF_SZ 8192
    uint8_t buf[BUF_SZ];
    while (data_len > 0) {
        size_t to_read = data_len > BUF_SZ ? BUF_SZ : (size_t)data_len;
        size_t n = fread(buf, 1, to_read, fin);
        if (n == 0) die("read data");
        fwrite(buf, 1, n, fout);
        data_len -= n;
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
