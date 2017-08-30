CC=clang
VERSION="$(shell git rev-parse --abbrev-ref HEAD)-$(shell git rev-parse --short HEAD)"
CFLAGS=-g -Wall -std=c11 -Iinclude -DVERSION='${VERSION}'

LDFLAGS=-lreadline
OBJS=$(patsubst %.c,%.o,$(wildcard src/*.c))

%.o: %.c
	$(CC) ${CFLAGS} -c -o $@ $<

zepto: ${OBJS}
	$(CC) ${CFLAGS} ${LDFLAGS} -o zepto ${OBJS}


.PHONY: clean

nuparse: src/ast.o src/nuparser.o src/lexer.o
	$(CC) ${CFLAGS} ${LDFLAGS} -o nuparse src/ast.o src/nuparser.o src/lexer.o

clean:
	rm -vf ${OBJS}

