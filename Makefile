beau: bin/main.o bin/beau.o bin/lex.o bin/parse.o bin/rvalue.o bin/compile.o \
	bin/type.o bin/sym.o
	cc bin/main.o bin/beau.o bin/lex.o bin/parse.o bin/rvalue.o \
	bin/compile.o bin/type.o bin/sym.o \
	-o beau

bin/main.o: src/main.c
	cc -c src/main.c -o bin/main.o

bin/beau.o: src/beau.c
	cc -c src/beau.c -o bin/beau.o

bin/lex.o: src/lex.c
	cc -c src/lex.c -o bin/lex.o

bin/parse.o: src/parse.c
	cc -c src/parse.c -o bin/parse.o

bin/rvalue.o: src/rvalue.c
	cc -c src/rvalue.c -o bin/rvalue.o

bin/qbe.o: src/qbe.c
	cc -c src/compile.c -o bin/compile.o

bin/type.o: src/type.c
	cc -c src/type.c -o bin/type.o

bin/sym.o: src/sym.c
	cc -c src/sym.c -o bin/sym.o

clean:
	rm -f beau bin/*.o