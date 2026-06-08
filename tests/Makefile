include ../Makefile.compiler-flags
BINDIR = ../out
OUTDIR = ../out/tests

# Debug versions are not tested, because they include a bunch
#   of extra output, and the tests check explicitly for output.
# This doesn't test every compression type, but instead that the general
#   construction and execution technique works as expected.
CMDS = presh presh-fd-tinyzzz-lzma presh-so-tinflate

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
	@echo "========================================================================"
	@test -f $(OUTDIR)/$@ || ( ls -l $(OUTDIR) ; >&2 echo "Did not compile $(OUTDIR)/$@" ; exit 1 )
	@echo "Testing $@"
	@mkdir -p $(OUTDIR)/$@
	@cp $(OUTDIR)/arg_runner $(OUTDIR)/$@/arg_runner
	@TEST_TMP_DIR=$(OUTDIR)/$@ PRESH=$(BINDIR)/$@ $(CURDIR)/all.sh

.PHONY: $(TEST_SCRIPTS)
$(TEST_SCRIPTS): $(OUTDIR)/arg_runner
	@mkdir -p $(OUTDIR)/$@
	@cp $(OUTDIR)/arg_runner $(OUTDIR)/$@/arg_runner
	@TEST_TMP_DIR=$(OUTDIR)/$(word 1, $(CMDS)) PRESH=$(BINDIR)/$(word 1, $(CMDS)) $(CURDIR)/run.sh $@
