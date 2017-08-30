CC=clang
CFLAGS=-g -Wall -std=c11 -Iinclude
LDFLAGS=-lreadline
OBJS=$(patsubst %.c,%.o,$(wildcard src/*.c))

%.o: %.c
	$(CC) ${CFLAGS} -c -o $@ $<

zepto: ${OBJS}
	$(CC) ${CFLAGS} ${LDFLAGS} -o zepto ${OBJS}

.PHONY: clean
clean:
	rm -vf ${OBJS}

