BINDIR = ../out
OUTDIR = ../out/tests
LFLAGS = -static -static-libgcc

# Debug versions are not tested, because they include a bunch
#   of extra output.
CMDS = fs-shell

.PHONY: all clean tests $(CMDS)
all: clean tests


clean:
	test -d $(OUTDIR) && rm -rf $(OUTDIR) 2>/dev/null || echo ""

$(OUTDIR)/arg_runner: _arg_runner.c
	@mkdir -p $(OUTDIR)
	@$(CC) $(CFLAGS) $(LFLAGS) $? $(LDFLAGS) -o $@

tests: $(CMDS)

$(CMDS): $(OUTDIR)/arg_runner
	@mkdir -p $(OUTDIR)/$@
	@cp $(OUTDIR)/arg_runner $(OUTDIR)/$@/arg_runner
	@TEST_TMP_DIR=$(OUTDIR)/$@ FS_SHELL=$(BINDIR)/$@ $(CURDIR)/_all.sh
