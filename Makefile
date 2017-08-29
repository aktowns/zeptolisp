CC=clang
CFLAGS=-g 
LDFLAGS=-lreadline

%.o: %.c
	$(CC) ${CFLAGS} -c -o $@ $<

zepto: src/main.o src/ast.o src/parser.o src/eval.o
	$(CC) ${CFLAGS} ${LDFLAGS} -o zepto src/main.o src/ast.o src/parser.o src/eval.o

clean:
	rm -vf src/*.o

