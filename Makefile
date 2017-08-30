CC=clang

USE_GC=$(shell pkg-config --exists bdw-gc; if [ $$? -eq 0 ]; then echo "1"; else echo "0"; fi)
GC_LIBS=$(shell pkg-config --libs bdw-gc)
CFLAGS=-g -DUSE_GC=${USE_GC}
LDFLAGS=-lreadline ${GC_LIBS}

%.o: %.c
	$(CC) ${CFLAGS} -c -o $@ $<

zepto: src/main.o src/ast.o src/parser.o src/eval.o
	$(CC) ${CFLAGS} ${LDFLAGS} -o zepto src/main.o src/ast.o src/parser.o src/eval.o

clean:
	rm -vf src/*.o

