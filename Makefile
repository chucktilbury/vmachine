
make_dirs := $(shell mkdir -p bin)

all: lib vm asm

lib:
	make -C src/lib

 bin/libvm.a: lib

vm: bin/libvm.a
	make -C src/vm

asm: bin/libvm.a
	make -C src/asm

clean:
	make -C src/lib clean
	make -C src/vm clean
	make -C src/asm clean

format:
	make -C src/lib format
	make -C src/vm format
	make -C src/asm format

.PHONY: docs

docs:
	make -C docs

distclean: clean
	make -C docs clean

read: docs
	firefox docs/out/html/index.html

