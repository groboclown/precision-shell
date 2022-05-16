BINDIR = ../out
OUTDIR = ../out/tests

# Debug versions are not tested, because they include a bunch
#   of extra output.
CMDS = fs-shell \
	fs-shell-signal \
	fs-shell-input \
	fs-shell-signal-input \
	fs-shell-exec \
	fs-shell-signal-exec \
	fs-shell-input-exec \
	fs-shell-signal-input-exec


.PHONY: all clean tests $(CMDS)
all: clean $(CMDS)


clean:
	test -d $(OUTDIR) && rm -rf $(OUTDIR) || echo "."


tests: $(CMDS)


$(CMDS):
	mkdir -p $(OUTDIR)/$@
	TEST_TMP_DIR=$(OUTDIR)/$@ FS_SHELL=$(BINDIR)/$@ $(CURDIR)/_all.sh
