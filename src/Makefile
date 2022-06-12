include ../version.txt

# MINFLAGS = -g0 -static -s -O3 -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
MINFLAGS = -g0 -s -Os -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
LFLAGS = -static
DFLAGS = -DDEBUG=1
CFLAGS += -Wall -pedantic -DVERSION_NUMBER=\"$(VERSION)-$(LIBNAME)\"
OUTDIR = ../out
SRC = main.c helpers.c globals.c command_runner.c command_list.c args-argv.c args-input.c globals.c \
	cmd_chmod.c \
	cmd_dup.c \
	cmd_echo.c \
	cmd_fmode.c \
	cmd_version.c
ALL_CMDS = \
	-DUSE_CMD_CHMOD=1 \
	-DUSE_CMD_CHOWN=1 \
	-DUSE_CMD_DUP_R=1 \
	-DUSE_CMD_DUP_W=1 \
	-DUSE_CMD_DUP_A=1 \
	-DUSE_CMD_ECHO=1 \
	-DUSE_CMD_EXEC=1 \
	-DUSE_CMD_FMODE=1 \
	-DUSE_CMD_LN_H=1 \
	-DUSE_CMD_LN_S=1 \
	-DUSE_CMD_MKDEV=1 \
	-DUSE_CMD_MKDIR=1 \
	-DUSE_CMD_MKNOD=1 \
	-DUSE_CMD_MV=1 \
	-DUSE_CMD_RM=1 \
	-DUSE_CMD_RMDIR=1 \
	-DUSE_CMD_SIGNAL=1 \
	-DUSE_CMD_SLEEP=1 \
	-DUSE_CMD_TOUCH=1 \
	-DUSE_CMD_TRUNC=1 \
	-DUSE_STREAMING_INPUT=1

.PHONY: all
all: clean \
	$(OUTDIR)/fs-shell $(OUTDIR)/fs-shell-debug


# Note that the arguments to the rm match the all parameters.
.PHONY: clean
clean:
	rm $(OUTDIR)/fs-shell* $(OUTDIR)/build-inventory.txt || echo "."


$(OUTDIR)/fs-shell: $(SRC)
	mkdir -p $(OUTDIR)
	basename $@ >> $(OUTDIR)/build-inventory.txt
	$(CC) $(CFLAGS) $(LFLAGS) $(MINFLAGS) $(ALL_CMDS) $? $(LDFLAGS) -o $@


$(OUTDIR)/fs-shell-debug: $(SRC)
	mkdir -p $(OUTDIR)
	basename $@ >> $(OUTDIR)/build-inventory.txt
	$(CC) $(CFLAGS) $(LFLAGS) $(DFLAGS) $(ALL_CMDS) $? $(LDFLAGS) -o $@
