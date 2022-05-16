
.PHONY: all
all: src-all

src-all:
	${MAKE} -C src all

.PHONY: clean
clean:
	test -d out && rm -r out
