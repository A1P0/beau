lone: bin/main.o bin/lone.o bin/lex.o bin/parse.o
	cc bin/main.o bin/lone.o bin/lex.o bin/parse.o -o lone

bin/main.o: src/main.c
	cc -c src/main.c -o bin/main.o

bin/lone.o: src/lone.c
	cc -c src/lone.c -o bin/lone.o

bin/lex.o: src/lex.c
	cc -c src/lex.c -o bin/lex.o

bin/parse.o: src/parse.c
	cc -c src/parse.c -o bin/parse.o

clean:
	rm lone bin/*.o