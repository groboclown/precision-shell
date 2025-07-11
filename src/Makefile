include ../version.txt
include ../Makefile.command-flags
include ../Makefile.compiler-flags

.SUFFIXES: .h.in .h

CMD_TEMPLATE_RUNNER = python3
CMD_TEMPLATE_CHECK = $(CMD_TEMPLATE_RUNNER) --version
CMD_TEMPLATE_SCRIPT = $(CMD_TEMPLATE_RUNNER) ../build-tools/generate-command-template.py
MKCMD = $(CMD_TEMPLATE_SCRIPT)
MKCMDLIST = $(CMD_TEMPLATE_SCRIPT) --cmd-list .in

# Include the version number from the version.txt
CFLAGS += -DVERSION_NUMBER=\"$(VERSION)-$(LIBNAME)\"

COMMAND_HEADERS_DIR = ./gen-cmd

NO_GETADDRINFO ?= 0

# Normally, we want unused variable warnings, however, with very limited command flags,
#   we will see this appear.
# switch is disabled because there are command enums that are markers rather than
#   real values to switch on.
CFLAGS += -Wall -pedantic -Wno-unused-variable -Wno-switch -I$(COMMAND_HEADERS_DIR) -I.
OUTDIR = ../out
SRC = main.c helpers.c globals.c command_runner.c args.c load_input.c

# cmd_find_cmd.h must always be first.
# cmd_version.h must always be second-to-last.
#   This is for test purposes.  By having the required command
#   "version" be at the end of the "version" command list, it
#   makes checking for command existence easier.
# cmd_err.h must always be last.
COMMANDS = \
	cmd_find_cmd.h \
	cmd_cat.h \
	cmd_cd.h \
	cmd_chmod.h \
	cmd_chown.h \
	cmd_dnslookup.h \
	cmd_dup.h \
	cmd_echo.h \
	cmd_exec.h \
	cmd_exit.h \
	cmd_export.h \
	cmd_file_stat.h \
	cmd_fmode.h \
	cmd_for_each.h \
	cmd_http_request.h \
	cmd_if_else.h \
	cmd_is_eq.h \
	cmd_ln_h.h \
	cmd_ln_s.h \
	cmd_ls_ls.h \
	cmd_kill_pid.h \
	cmd_mkdir.h \
	cmd_mknod_mkdev.h \
	cmd_mv.h \
	cmd_noop.h \
	cmd_not.h \
	cmd_pwd.h \
	cmd_rm.h \
	cmd_rmdir.h \
	cmd_shared_address.h \
	cmd_shared_connect.h \
	cmd_shared_sub_args.h \
	cmd_shared_virtual.h \
	cmd_signal.h \
	cmd_sleep.h \
	cmd_spawn.h \
	cmd_su_exec.h \
	cmd_su_spawn.h \
	cmd_subcmd.h \
	cmd_test_connect.h \
	cmd_timer.h \
	cmd_touch_trunc.h \
	cmd_wait_pid.h \
	cmd_while.h \
	cmd_write_fd.h \
	cmd_version.h \
	cmd_err.h

# Note: cmd_ls_ls.h is not enabled.  Either this should
#   be used, or cmd_ls.h + cmd_file_stat.h, but not both.


.PHONY: all
all: clean src debug $(OUTDIR)/test_server


.PHONY: src
src: \
	$(OUTDIR)/presh

.PHONY: debug
debug: \
	$(OUTDIR)/presh-debug

# Note that the arguments to the rm match the all parameters.
.PHONY: clean
clean:
	rm $(OUTDIR)/presh* $(COMMAND_HEADERS_DIR) 2>/dev/null || echo ""

.PHONY: all-clean
all-clean: clean
	rm gen-cmd/*


.PHONY: cmd_headers
cmd_headers: $(COMMANDS) $(COMMAND_HEADERS_DIR)/command_list.h


# Note: If Python is not installed and the header does not exist, then
#   this will fail.  Otherwise, only build the headers if Python is
#   installed.

%.h: %.h.in
	mkdir -p $(COMMAND_HEADERS_DIR)
	$(CMD_TEMPLATE_CHECK) > /dev/null 2>&1 ; \
		test $$? != 0 -a -f $(COMMAND_HEADERS_DIR)/$*.h \
		&& echo "Skipping generation" \
		|| $(MKCMD) $< $(COMMAND_HEADERS_DIR)/$*.h


$(COMMAND_HEADERS_DIR)/command_list.h: $(COMMANDS)
	mkdir -p $(COMMAND_HEADERS_DIR)
	$(CMD_TEMPLATE_CHECK) > /dev/null 2>&1 ; \
		test $$? != 0 -a -f $(COMMAND_HEADERS_DIR)/command_list.h \
		&& echo "Skipping generation" \
		|| $(MKCMDLIST) $(COMMANDS) $*.h


# clang can only take one .c file as input.

$(OUTDIR)/presh: $(SRC) $(COMMAND_HEADERS_DIR)/command_list.h
	mkdir -p $(OUTDIR)
	cat $(SRC) > $(OUTDIR)/_presh-all.c
	$(CC) $(CFLAGS) $(LFLAGS) $(MINFLAGS) $(COMMAND_FLAGS) $(OUTDIR)/_presh-all.c $(LDFLAGS) -o $@


$(OUTDIR)/presh-debug: $(SRC) $(COMMAND_HEADERS_DIR)/command_list.h
	mkdir -p $(OUTDIR)
	cat $(SRC) > $(OUTDIR)/_presh-all.c
	$(CC) $(CFLAGS) $(LFLAGS) $(DFLAGS) $(COMMAND_FLAGS) $(OUTDIR)/_presh-all.c $(LDFLAGS) -o $@


$(OUTDIR)/test_server: test_server.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $? $(LDFLAGS) -o $@
