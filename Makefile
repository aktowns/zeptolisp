CFLAGS=-lreadline -g

%.o: %.c
	clang ${CFLAGS} -c -o $@ $<

zepto: src/main.o src/ast.o src/parser.o
	clang ${CFLAGS} -o zepto src/main.o src/ast.o src/parser.o

clean: 
	rm -vf src/*.o

