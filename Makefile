CC=clang
VERSION="$(shell git rev-parse --abbrev-ref HEAD)-$(shell git rev-parse --short HEAD)"
CFLAGS=-g -Wall -std=gnu11 -Iinclude -DVERSION='${VERSION}'
LDFLAGS=-lreadline
OBJS=$(patsubst %.c,%.o,$(wildcard src/*.c))

%.o: %.c
	$(CC) ${CFLAGS} -c -o $@ $<

zepto: ${OBJS}
	$(CC) ${CFLAGS} ${LDFLAGS} -o zepto ${OBJS}

.PHONY: clean
clean:
	rm -vf ${OBJS}

