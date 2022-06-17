include ../version.txt
include ../Makefile.command-flags


.SUFFIXES: .h.in .h

CMD_TEMPLATE_SCRIPT = python3 ../build-tools/generate-command-template.py
MKCMD = $(CMD_TEMPLATE_SCRIPT)
MKCMDLIST = $(CMD_TEMPLATE_SCRIPT) --cmd-list .in

# Include the version number from the version.txt
CFLAGS += -DVERSION_NUMBER=\"$(VERSION)-$(LIBNAME)\"

# See the compile-size-test.sh for investigation into
#   which compile flags makes the smallest executable.
# MINFLAGS = -g0 -s -Oz -Os -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
MINFLAGS = \
	-fno-builtin-strlen \
	-fomit-frame-pointer \
	-ffunction-sections \
	-fdata-sections \
	-fno-guess-branch-probability \
	-funsigned-char \
	-fno-unwind-tables \
	-fno-asynchronous-unwind-tables \
	-fno-builtin-printf \
	-Os \
	-fvisibility=hidden \
	-g0 \
	-s \
	-fno-strict-aliasing \
	-fno-align-functions \
	-fno-align-jumps \
	-fno-align-loops \
	-fno-align-labels
#	-Oz \
#	-falign-functions=1 \
#	-falign-jumps=1 \
#	-falign-labels=1 \
#	-falign-loops=1 \

# Allow inline.
# 	-finline-limit=0 \


LFLAGS = -static -static-libgcc
DFLAGS = -DDEBUG=1 -g

COMMAND_HEADERS_DIR = ./gen-cmd

# Normally, we want unused variable warnings, however, with very limited command flags,
#   we will see this appear.
CFLAGS += -Wall -pedantic -Wno-unused-variable -I$(COMMAND_HEADERS_DIR) -I.
OUTDIR = ../out
SRC = main.c helpers.c globals.c command_runner.c args-argv.c args-input.c

# cmd_find_cmd.h must always be first.
# cmd_err.h must always be last.
COMMANDS = \
	cmd_find_cmd.h \
	cmd_chmod.h \
	cmd_chown.h \
	cmd_dup.h \
	cmd_echo.h \
	cmd_exec.h \
	cmd_fmode.h \
	cmd_ln_h.h \
	cmd_ln_s.h \
	cmd_mkdir.h \
	cmd_mknod_mkdev.h \
	cmd_mv.h \
	cmd_noop.h \
	cmd_rm.h \
	cmd_rmdir.h \
	cmd_shared_virtual.h \
	cmd_signal.h \
	cmd_sleep.h \
	cmd_touch_trunc.h \
	cmd_version.h \
	cmd_err.h


.PHONY: all
all: clean src debug


.PHONY: src
src: \
	$(OUTDIR)/fs-shell

.PHONY: debug
debug: \
	$(OUTDIR)/fs-shell-debug

# Note that the arguments to the rm match the all parameters.
.PHONY: clean
clean:
	rm $(OUTDIR)/fs-shell* $(COMMAND_HEADERS_DIR) 2>/dev/null || echo ""


.PHONY: cmd_headers
cmd_headers: $(COMMANDS) $(COMMAND_HEADERS_DIR)/command_list.h

%.h: %.h.in
	mkdir -p $(COMMAND_HEADERS_DIR)
	$(MKCMD) $< $(COMMAND_HEADERS_DIR)/$*.h


$(COMMAND_HEADERS_DIR)/command_list.h: $(COMMANDS)
	mkdir -p $(COMMAND_HEADERS_DIR)
	$(MKCMDLIST) $(COMMANDS) $*.h


$(OUTDIR)/fs-shell: $(SRC) $(COMMAND_HEADERS_DIR)/command_list.h
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $(MINFLAGS) $(COMMAND_FLAGS) $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-debug: $(SRC) $(COMMAND_HEADERS_DIR)/command_list.h
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $(DFLAGS) $(COMMAND_FLAGS) $? $(LDFLAGS) -o $@
