SUBDIRS = src compressed tests

NO_GETADDRINFO ?= 0

.PHONY: all clean subdirs $(SUBDIRS) help

subdirs: $(SUBDIRS)

all: clean subdirs

clean:
	test -d out && rm -rf out || echo "."

$(SUBDIRS):
	$(MAKE) -C $@ NO_GETADDRINFO=$(NO_GETADDRINFO)

help:
	@echo "Compiles and tests the program."
	@echo "  By default, the shell compiles with no commands."
	@echo "  In order to select commands to include, you must set 'COMMAND_FLAGS'"
	@echo "  to include the commands to build; for example:"
	@echo "     make COMMAND_FLAGS='\$$CMD_CHMOD \$$CMD_CHOWN'"
