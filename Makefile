
# MINFLAGS = -static -s -Os -fomit-frame-pointer -fno-strict-aliasing
# MINFLAGS = -g0 -static --enable-static-nss --static -s -Os -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels
MINFLAGS = -g0 -static -s -Os -fomit-frame-pointer -fno-strict-aliasing -fno-align-functions -fno-align-jumps -fno-align-loops -fno-align-labels


fs-shell: fs-shell.c
	$(CC) $(CFLAGS) $(MINFLAGS) $? $(LDFLAGS) -o $@
