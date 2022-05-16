include ../version.txt

# MINFLAGS = -g0 -static -s -O3 -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
MINFLAGS = -g0 -static -s -Os -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
CFLAGS += -Wall -pedantic -DVERSION_NUMBER=\"$(VERSION)-$(LIBNAME)\"
OUTDIR = ../out

.PHONY: all
all: clean \
	$(OUTDIR)/fs-shell $(OUTDIR)/fs-shell-debug \
	$(OUTDIR)/fs-shell-signal $(OUTDIR)/fs-shell-signal-debug \
	$(OUTDIR)/fs-shell-input $(OUTDIR)/fs-shell-input-debug \
	$(OUTDIR)/fs-shell-signal-input $(OUTDIR)/fs-shell-signal-input-debug \
	$(OUTDIR)/fs-shell-exec $(OUTDIR)/fs-shell-exec-debug \
	$(OUTDIR)/fs-shell-signal-exec $(OUTDIR)/fs-shell-signal-exec-debug \
	$(OUTDIR)/fs-shell-input-exec $(OUTDIR)/fs-shell-input-exec-debug \
	$(OUTDIR)/fs-shell-signal-input-exec $(OUTDIR)/fs-shell-signal-input-exec-debug \
	$(OUTDIR)/fs-shell-lean $(OUTDIR)/fs-shell-lean-debug \
	$(OUTDIR)/fs-shell-fat $(OUTDIR)/fs-shell-fat-debug


# Note that the arguments to the rm match the all parameters.
.PHONY: clean
clean:
	rm $(OUTDIR)/fs-shell* || echo "."


$(OUTDIR)/fs-shell: fs-shell.c commands.c args-argv.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(MINFLAGS) $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-debug: fs-shell.c commands.c args-argv.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -DDEBUG=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-signal: fs-shell.c commands.c args-argv.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(MINFLAGS) -DUSE_SIGNALS=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-signal-debug: fs-shell.c commands.c args-argv.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -DDEBUG=1 -DUSE_SIGNALS=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-input: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(MINFLAGS) -DUSES_INPUT=1 $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-input-debug: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -DDEBUG=1 -DUSES_INPUT=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-signal-input: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(MINFLAGS) -DUSE_SIGNALS=1 -DUSES_INPUT=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-signal-input-debug: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -DDEBUG=1 -DUSE_SIGNALS=1 -DUSES_INPUT=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-exec: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(MINFLAGS) -DUSE_EXEC=1 $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-exec-debug: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -DDEBUG=1 -DUSE_EXEC=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-signal-exec: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(MINFLAGS) -DUSE_SIGNALS=1 -DUSE_EXEC=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-signal-exec-debug: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -DDEBUG=1 -DUSE_SIGNALS=1 -DUSE_EXEC=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-input-exec: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(MINFLAGS) -DUSE_EXEC=1 -DUSES_INPUT=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-input-exec-debug: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -DDEBUG=1 -DUSE_EXEC=1 -DUSES_INPUT=1 -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-signal-input-exec: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(MINFLAGS) -DUSE_SIGNALS=1 -DUSES_INPUT=1-DUSE_EXEC=1  -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-signal-input-exec-debug: fs-shell.c commands.c args-input.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -DDEBUG=1 -DUSE_SIGNALS=1 -DUSES_INPUT=1 -DUSE_EXEC=1  -static $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-lean: $(OUTDIR)/fs-shell
	cp $? $@

$(OUTDIR)/fs-shell-lean-debug: $(OUTDIR)/fs-shell-debug
	cp $? $@

$(OUTDIR)/fs-shell-fat: $(OUTDIR)/fs-shell-signal-input-exec
	cp $? $@
	
$(OUTDIR)/fs-shell-fat-debug: $(OUTDIR)/fs-shell-signal-input-exec-debug
	cp $? $@
