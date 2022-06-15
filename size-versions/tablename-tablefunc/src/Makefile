include ../version.txt
include ../Makefile.command-flags

# Include the version number from the version.txt
CFLAGS += -DVERSION_NUMBER=\"$(VERSION)-$(LIBNAME)\"

# dietlibc has issues with data sections containing pointers.
ifdef BROKEN_DATA_POINTERS
	CFLAGS += -DBROKEN_DATA_POINTERS=1
endif

# See the compile-size-test.sh for investigation into
#   which compile flags makes the smallest executable.
# MINFLAGS = -g0 -s -Oz -Os -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
MINFLAGS = \
	-finline-limit=0 \
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

LFLAGS = -static -static-libgcc
DFLAGS = -DDEBUG=1 -g

CFLAGS += -Wall -pedantic
OUTDIR = ../out
SRC = main.c helpers.c globals.c command_runner.c command_list.c command_common.c args-argv.c args-input.c \
	cmd_find_cmd.c \
	cmd_err.c \
	cmd_noop.c \
	cmd_version.c \
	cmd_chmod.c \
	cmd_chown.c \
	cmd_dup.c \
	cmd_echo.c \
	cmd_exec.c \
	cmd_fmode.c \
	cmd_ln_h.c \
	cmd_ln_s.c \
	cmd_mv.c \
	cmd_mkdir.c \
	cmd_mknod_mkdev.c \
	cmd_rm.c \
	cmd_rmdir.c \
	cmd_signal.c \
	cmd_sleep.c \
	cmd_touch_trunc.c


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
	rm $(OUTDIR)/fs-shell* $(OUTDIR)/build-inventory.txt 2>/dev/null || echo ""


$(OUTDIR)/fs-shell: $(SRC)
	mkdir -p $(OUTDIR)
	basename $@ >> $(OUTDIR)/build-inventory.txt
	$(CC) $(CFLAGS) $(LFLAGS) $(MINFLAGS) $(COMMAND_FLAGS) $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-debug: $(SRC)
	mkdir -p $(OUTDIR)
	basename $@ >> $(OUTDIR)/build-inventory.txt
	$(CC) $(CFLAGS) $(LFLAGS) $(DFLAGS) $(COMMAND_FLAGS) $? $(LDFLAGS) -o $@
