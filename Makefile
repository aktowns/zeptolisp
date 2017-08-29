CFLAGS=-lreadline -g

%.o: %.c
	clang ${CFLAGS} -c -o $@ $<

zepto: src/main.o src/ast.o src/parser.o src/eval.o
	clang ${CFLAGS} -o zepto src/main.o src/ast.o src/parser.o src/eval.o

clean:
	rm -vf src/*.o

