include ../Makefile.compiler-flags
BINDIR = ../out
OUTDIR = ../out/tests

# Debug versions are not tested, because they include a bunch
#   of extra output.
CMDS = presh

TEST_SCRIPTS := $(wildcard */*.sh)

.PHONY: all clean tests $(CMDS)
all: clean tests


clean:
	test -d $(OUTDIR) && rm -rf $(OUTDIR) 2>/dev/null || true

$(OUTDIR)/arg_runner: _arg_runner.c
	@mkdir -p $(OUTDIR)
	@$(CC) $(CFLAGS) $(LFLAGS) $? $(LDFLAGS) -o $@

tests: $(CMDS)

$(CMDS): $(OUTDIR)/arg_runner
	@mkdir -p $(OUTDIR)/$@
	@cp $(OUTDIR)/arg_runner $(OUTDIR)/$@/arg_runner
	@TEST_TMP_DIR=$(OUTDIR)/$@ PRESH=$(BINDIR)/$@ $(CURDIR)/all.sh

.PHONY: $(TEST_SCRIPTS)
$(TEST_SCRIPTS): $(OUTDIR)/arg_runner
	@mkdir -p $(OUTDIR)/$@
	@cp $(OUTDIR)/arg_runner $(OUTDIR)/$@/arg_runner
	@TEST_TMP_DIR=$(OUTDIR)/$(word 1, $(CMDS)) PRESH=$(BINDIR)/$(word 1, $(CMDS)) $(CURDIR)/run.sh $@
