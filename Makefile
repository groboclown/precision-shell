SUBDIRS = src tests

.PHONY: all clean subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

all: clean subdirs

clean:
	test -d out && rm -rf out || echo "."

$(SUBDIRS):
	$(MAKE) -C $@
