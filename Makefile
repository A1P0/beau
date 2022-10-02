lone: bin/main.o bin/lone.o bin/lex.o bin/parse.o bin/rvalue.o bin/compile.o
	$(CXX) bin/main.o bin/lone.o bin/lex.o bin/parse.o bin/rvalue.o \
	bin/compile.o -o lone $$(llvm-config --ldflags --libs)

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

clean:
	rm lone bin/*.o