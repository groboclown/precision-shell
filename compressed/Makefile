include ../Makefile.compiler-flags

.SUFFIXES: .h

# Normally, we want unused variable warnings, however, with very limited command flags,
#   we will see this appear.
# switch is disabled because there are command enums that are markers rather than
#   real values to switch on.
OUTDIR ?= ../out
CFLAGS += -Wall -pedantic -I. -I$(OUTDIR)
SRC ?= $(OUTDIR)/presh

# The binary blob for the different versions of the stub runner.
# Very explicitly done to allow for easy suffix.
SRC_FD := $(SRC)-fd
SRC_LK := $(SRC)-lk

# For loop magic.
DECOMPRESS_IMPLEMENTATIONS = tinflate tinyzzz-lzma tinyzzz-zstd
SRC_TYPES = fd lk
SRC_BIN_FILES = $(foreach ST,$(SRC_TYPES),$(SRC)-$(ST))


# At the moment, dietlibc doesn't run right on some stub runners.
# It has to do with syscall issues.
ifeq ($(CC),diet cc)
STD_CC = cc
CFLAGS += -D__dietlibc__
LFLAGS += ${DIETLIBC_LIBDIR}/syscall.o
else
STD_CC = $(CC)
endif


.PHONY: all
all: clean build

.PHONY: make-debug
make-debug:


.PHONY: build

.PHONY: clean
clean:
	rm -f \
		$(OUTDIR)/gzip-stripped \
		$(OUTDIR)/extract-elf \
		$(OUTDIR)/stub-run-fd.c \
		$(OUTDIR)/stub-run-lk.c \
		$(SRC_FD) \
		$(SRC_LK) \
		2>/dev/null || true

$(SRC):
	@echo "You must run 'make' in the src directory to build this."
	exit 1

# ---------------------------------------------------------------------
# Helper utilities
$(OUTDIR)/gzip-strip: gzip-strip.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $@

$(OUTDIR)/extract-elf: extract-elf.c extract-elf.h
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $< -o $@


# ---------------------------------------------------------------------
# The stub runner binaries and runners.
# Runs off the source ELF file.  Generates its own binary file.
$(SRC_LK): $(SRC) $(OUTDIR)/extract-elf
	mkdir -p $(OUTDIR)
	$(OUTDIR)/extract-elf $(SRC) $(OUTDIR)/stub-run-lk.c $(SRC_LK)

$(OUTDIR)/stub-run-lk.c: $(SRC_LK)

$(SRC_FD): $(SRC)
	mkdir -p $(OUTDIR)
	cp $(SRC) $(SRC_FD)

$(OUTDIR)/stub-run-fd.c: stub-run-fd.c
	mkdir -p $(OUTDIR)
	cp `pwd`/$< $@


# ---------------------------------------------------------------------
# Supported compression formats.
#   Generally, these have one target to create the compressed file,
#   and another to strip off the unnecessary information.
#   The output file name uses '$(SRC)-<compression>.bin' for the stripped file,
#   and '$(SRC)-<compression>.<format>' for the original compressed file.

# Unfortunately, by using defines, special variables like $< and $@ do not work.
# Also, setting make variables requires an eval.

define DECOMPRESS_BUILD

$(eval SRC_BIN=$(1))

# ...................
# Deflate (gzip)

$(SRC_BIN)-deflate.gz: $(SRC_BIN)
	mkdir -p $(OUTDIR)
	gzip -c -9 $(SRC_BIN) > $(SRC_BIN)-deflate.gz

$(SRC_BIN)-deflate.bin: $(SRC_BIN)-deflate.gz $(OUTDIR)/gzip-strip
	mkdir -p $(OUTDIR)
	$(OUTDIR)/gzip-strip $(SRC_BIN)-deflate.gz $(SRC_BIN)-deflate.bin

# each implementation that uses this algorithm...
$(SRC_BIN)-tinflate.bin: $(SRC_BIN)-deflate.bin
	cp $(SRC_BIN)-deflate.bin $(SRC_BIN)-tinflate.bin


# ...................
# LZMA (not lzma2)
# The header is already super tiny, so no need to strip it.

$(SRC_BIN)-lzma.lzma: $(SRC_BIN)
	mkdir -p $(OUTDIR)
	xz -9e --check=crc32 --format=lzma --lzma1 $(SRC_BIN) -c > $(SRC_BIN)-lzma.lzma

# one per implementation...
$(SRC_BIN)-tinyzzz-lzma.bin: $(SRC_BIN)-lzma.lzma
	mkdir -p $(OUTDIR)
	cp $(SRC_BIN)-lzma.lzma $(SRC_BIN)-tinyzzz-lzma.bin


# ...................
# zstd
$(SRC_BIN)-zstd.zst: $(SRC_BIN)
	mkdir -p $(OUTDIR)
	zstd -22 --ultra -f $(SRC_BIN) -o $(SRC_BIN)-zstd.zst

# one per implementation...
$(SRC_BIN)-tinyzzz-zstd.bin: $(SRC_BIN)-zstd.zst
	mkdir -p $(OUTDIR)
	cp $(SRC_BIN)-zstd.zst $(SRC_BIN)-tinyzzz-zstd.bin

# ...................
# LZ4
# Experiments show this to have larger files than other ones, even gz,
# so won't explore decompression implementation.

$(SRC_BIN)-lz4.lz4: $(SRC_BIN)
	mkdir -p $(OUTDIR)
	lz4 -12 --no-frame-crc -f $(SRC_BIN) $(SRC_BIN)-lz4.lz4



clean: clean-decompress-$(SRC_BIN)
.PHONY: clean-decompress-$(SRC_BIN)
clean-decompress-$(SRC_BIN):
	rm -f \
		$(SRC_BIN)-deflate.gz \
		$(SRC_BIN)-deflate.bin \
		$(SRC_BIN)-tinflate.bin \
		$(SRC_BIN)-lzma.lzma \
		$(SRC_BIN)-tinyzzz-lzma.bin \
		2>/dev/null || true

make-debug: DEBUG-$(SRC_BIN)

.PHONY: DEBUG-$(SRC_BIN)
DEBUG-$(SRC_BIN):
	echo "SRC_BIN=$(SRC_BIN)"

endef

# Perform the DECOMPRESS_BUILD one for each source binary file.
$(eval $(foreach SBIN,$(SRC_BIN_FILES),$(eval $(call DECOMPRESS_BUILD,$(SBIN)))))


# ---------------------------------------------------------------------
# Combinatorics of the compression formats and the stub runners.

define COMP_RUN_BUILD

$(eval COMP_IMPL = $(1))
$(eval SRC_TYPE = $(2))
$(eval SRC_BIN = $(SRC)-$(SRC_TYPE))
$(eval COMP_BIN = $(SRC_BIN)-$(1).bin)
$(eval BIN = $(SRC_BIN)-$(COMP_IMPL))

clean: clean-$(COMP_IMPL)-$(SRC_TYPE)

.PHONY: clean-$(COMP_IMPL)-$(SRC_TYPE)
clean-$(COMP_IMPL)-$(SRC_TYPE):
	rm -f \
		$(COMP_BIN).c \
		$(COMP_BIN)-lk.c \
		$(BIN) \
		2>/dev/null || true

$(COMP_BIN).c: $(COMP_BIN)
	mkdir -p $(OUTDIR)
	xxd -i $(COMP_BIN) > $(COMP_BIN).c.1
	sed -e 's|unsigned char [a-zA-Z0-9_]\+|unsigned char ___presh_z|g' $(COMP_BIN).c.1 > $(COMP_BIN).c.2
	rm -f $(COMP_BIN).c.1
	sed -e 's|unsigned int [a-zA-Z0-9_]\+|unsigned int ___presh_z_len|g' $(COMP_BIN).c.2 > $(COMP_BIN).c
	rm -f $(COMP_BIN).c.2
	echo "unsigned int ___presh_len = " `wc -c < $(SRC_BIN)` ";" >> $(COMP_BIN).c

$(BIN): stub.c $(COMP_BIN).c $(OUTDIR)/stub-run-$(SRC_TYPE).c stub-$(COMP_IMPL).c $(wildcard $(COMP_IMPL)/*.c) $(wildcard $(COMP_IMPL)/*.h) stub-common.c stub-common.h data-payload.h
	echo "Building $(BIN) for $(COMP_IMPL) with source type $(SRC_TYPE)"
	# $(CC) $(CFLAGS) $(LFLAGS) $(MINFLAGS) stub.c $(COMP_BIN).c $(OUTDIR)/stub-run-$(SRC_TYPE).c stub-$(COMP_IMPL).c $(wildcard $(COMP_IMPL)/*.c) stub-common.c -o $(BIN)
	$(CC) $(CFLAGS) $(LFLAGS) $(DFLAGS) stub.c $(COMP_BIN).c $(OUTDIR)/stub-run-$(SRC_TYPE).c stub-$(COMP_IMPL).c $(wildcard $(COMP_IMPL)/*.c) stub-common.c -o $(BIN)
	strip -x -R .comment -R .note -R .gnu.version -R .gnu.version_r $(BIN) || true

build: $(BIN)


make-debug: DEBUG-$(BIN)
.PHONY: DEBUG-$(BIN)
DEBUG-$(BIN):
	echo "COMP_IMPL=$(COMP_IMPL) SRC_TYPE=$(SRC_TYPE) BIN=$(BIN)"


endef

$(foreach SRC_TYPE,$(SRC_TYPES),$(foreach COMP_IMPL,$(DECOMPRESS_IMPLEMENTATIONS),$(eval $(call COMP_RUN_BUILD,$(COMP_IMPL),$(SRC_TYPE)))))
