
.PHONY: all
all: src-all compressed-all

src-all:
	${MAKE} -C src all

compressed-all: src
	${MAKE} -C compressed-exec all


.PHONY: clean
clean:
	test -d out && rm -r out
