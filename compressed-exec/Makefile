
# MINFLAGS = -g0 -static -s -O3 -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
MINFLAGS = -g0 -static -s -Os -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
CFLAGS += -Wall -pedantic -DVERSION_NUMBER=\"1.1\"
OUTDIR=../out


.PHONY: all
all: \
	${OUTDIR}/compress_lzg ${OUTDIR}/bin2c \
	${OUTDIR}/compressed-fs-shell

# Not minimized.
${OUTDIR}/compress_lzg: compress_lzg.c lzg_encode.c lzg_version.c lzg_checksum.c
	mkdir -p ${OUTDIR}
	$(CC) $(CFLAGS) -static $? $(LDFLAGS) -o $@

${OUTDIR}/bin2c: bin2c.c
	mkdir -p ${OUTDIR}
	$(CC) $(CFLAGS) -static $? $(LDFLAGS) -o $@


${OUTDIR}/compressed-fs-shell: ${OUTDIR}/compress_lzg ${OUTDIR}/bin2c
	test -d ${OUTDIR}/.compressed-fs-shell.d/* && rm -r ${OUTDIR}/.compressed-fs-shell.d || echo "Pass"
	mkdir -p ${OUTDIR}/.compressed-fs-shell.d
	PATH="${PATH}:${OUTDIR}" bash -x ./gen-self-compressed.sh ${OUTDIR}/fs-shell ${OUTDIR}/.compressed-fs-shell.d
	$(CC) $(CFLAGS) $(MINFLAGS) ${OUTDIR}/.compressed-fs-shell.d/*.c -o $@
