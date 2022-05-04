
# MINFLAGS = -g0 -static -s -O3 -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
MINFLAGS = -g0 -static -s -Os -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels

all: fs-shell fs-shell-debug


fs-shell: fs-shell.c commands.c
	$(CC) $(CFLAGS) $(MINFLAGS) $? $(LDFLAGS) -o $@


fs-shell-debug: fs-shell.c commands.c
	$(CC) $(CFLAGS) -DDEBUG=1 -static $? $(LDFLAGS) -o $@


clean:
	test -f fs-shell && rm fs-shell
	test -f fs-shell-debug && rm fs-shell-debug
