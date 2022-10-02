lone: bin/main.o bin/lone.o bin/lex.o bin/parse.o bin/rvalue.o bin/compile.o \
	bin/type.o bin/sym.o
	$(CXX) bin/main.o bin/lone.o bin/lex.o bin/parse.o bin/rvalue.o \
	bin/compile.o bin/type.o bin/sym.o \
	-o lone $$(llvm-config --ldflags --libs)

bin/main.o: src/main.c
	cc -c src/main.c -o bin/main.o

bin/lone.o: src/lone.c
	cc -c src/lone.c -o bin/lone.o

bin/lex.o: src/lex.c
	cc -c src/lex.c -o bin/lex.o

bin/parse.o: src/parse.c
	cc -c src/parse.c -o bin/parse.o

bin/rvalue.o: src/rvalue.c
	cc -c src/rvalue.c -o bin/rvalue.o

bin/compile.o: src/compile.c
	cc $$(llvm-config --cflags) -c src/compile.c -o bin/compile.o

bin/type.o: src/type.c
	cc $$(llvm-config --cflags) -c src/type.c -o bin/type.o

bin/sym.o: src/sym.c
	cc $$(llvm-config --cflags) -c src/sym.c -o bin/sym.o

clean:
	rm lone bin/*.o