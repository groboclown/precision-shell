include ../Makefile.compiler-flags

.SUFFIXES: .h

# Normally, we want unused variable warnings, however, with very limited command flags,
#   we will see this appear.
# switch is disabled because there are command enums that are markers rather than
#   real values to switch on.
OUTDIR = ../out
CFLAGS += -Wall -pedantic -I. -I$(OUTDIR)
SRC = $(OUTDIR)/presh



.PHONY: all
all: clean build


.PHONY: build
build: \
	$(OUTDIR)/presh-zipped \
	$(OUTDIR)/presh-comp \

# Note that the arguments to the rm match the all parameters.
.PHONY: clean
clean:
	rm -f \
		$(OUTDIR)/presh-zipped \
		$(OUTDIR)/presh_payload.h \
		$(OUTDIR)/presh.bin.deflate \
		$(OUTDIR)/presh.bin.gz \
		$(OUTDIR)/gzip-stripped \
		2>/dev/null || true

$(SRC):
	@echo "You must run 'make' in the src directory to build this."
	exit 1

$(OUTDIR)/gzip-strip: gzip-strip.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $@

# This will strip the gzip header and tail, and use the raw DEFLATE data.
$(OUTDIR)/presh.bin.gz: $(SRC)
	mkdir -p $(OUTDIR)
	gzip -c -9 $< > $@

$(OUTDIR)/presh.bin.deflate: $(OUTDIR)/presh.bin.gz $(OUTDIR)/gzip-strip
	mkdir -p $(OUTDIR)
	$(OUTDIR)/gzip-strip $(OUTDIR)/presh.bin.gz $@

$(OUTDIR)/presh_payload.h: $(OUTDIR)/presh.bin.deflate
	mkdir -p $(OUTDIR)
	xxd -i $< > $@.1
	sed -e 's|unsigned char [a-zA-Z0-9_]\+|unsigned char ___presh_z|g' $@.1 > $@.2
	rm -f $@.1
	sed -e 's|unsigned int [a-zA-Z0-9_]\+|unsigned int ___presh_z_len|g' $@.2 > $@
	rm -f $@.2
	echo "unsigned int ___presh_len = " `wc -c < $(SRC)` ";" >> $@

# At the moment, dietlibc doesn't run right on the stub.
# It has to do with syscall issues.
ifeq ($(CC),diet cc)
STD_CC = cc
CFLAGS += -D__dietlibc__
else
STD_CC = $(CC)
endif

$(OUTDIR)/presh-zipped: stub.c tinflate.c $(OUTDIR)/presh_payload.h
	mkdir -p $(OUTDIR)
	$(STD_CC) $(CFLAGS) $(LFLAGS) $(MINFLAGS) -I$(OUTDIR) stub.c tinflate.c -o $@


$(OUTDIR)/presh-segments.h: $(SRC)
	readelf -lW $< \
  		| awk '/LOAD/ { printf("static segment_t SEGMENT_%d = { file_off: 0x%x, mem_sz: 0x%x, mem_vaddr: 0x%x, flags: 0x%x };\n", \
	++i, $$6, $$5, $$4, $$8) }' \
        > $@
	readelf -r $< \
		| tr -s ' ' \
		| awk '/_IRELATIV/ { printf("static irelativ_t IRELATIV_%d = { offset: 0x%x, addend: 0x%x };\n", ++i, $$1, $$4) }' \
		>> $@

$(OUTDIR)/extract-elf: extract-elf.c extract-elf.h
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $< -o $@

$(OUTDIR)/presh-comp.bin: $(OUTDIR)/extract-elf $(OUTDIR)/presh
	mkdir -p $(OUTDIR)
	$(OUTDIR)/extract-elf $(OUTDIR)/presh $(OUTDIR)/load-run.c $(OUTDIR)/load-run.h $(OUTDIR)/presh-comp.bin

$(OUTDIR)/presh-comp.bin.gz: $(OUTDIR)/presh-comp.bin
	mkdir -p $(OUTDIR)
	gzip -c -9 $< > $@

$(OUTDIR)/presh-comp.bin.deflate: $(OUTDIR)/presh-comp.bin.gz $(OUTDIR)/gzip-strip
	mkdir -p $(OUTDIR)
	$(OUTDIR)/gzip-strip $(OUTDIR)/presh-comp.bin.gz $@

$(OUTDIR)/load-run.c: $(OUTDIR)/presh-comp.bin

$(OUTDIR)/load-run.h: $(OUTDIR)/presh-comp.bin

$(OUTDIR)/presh-comp.bin.h: $(OUTDIR)/presh-comp.bin.deflate
	xxd -i $< > $@.1
	sed -e 's|unsigned char [a-zA-Z0-9_]\+|unsigned char ___presh_comp|g' $@.1 > $@.2
	rm -f $@.1
	sed -e 's|unsigned int [a-zA-Z0-9_]\+|unsigned int ___presh_comp_len|g' $@.2 > $@
	rm -f $@.2
	echo "unsigned int ___presh_len = " `wc -c < $(OUTDIR)/presh-comp.bin` ";" >> $@

$(OUTDIR)/presh-comp: stub2.c tinflate.c tinf.h $(OUTDIR)/presh-comp.bin.h $(OUTDIR)/load-run.c $(OUTDIR)/load-run.h
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $(MINFLAGS) -I$(OUTDIR) stub2.c tinflate.c $(OUTDIR)/load-run.c -o $@
