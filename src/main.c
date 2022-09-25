/*
 * main.c
 * Entry point, contents volatile as hell, early development testing.
 */
#include <stdio.h>
#include "lone.h"
#include "lex.h"

/* How lone was invoked */
char *arg0;

int /* volatile testing */
main(int argc, char *argv[])
{
        lexer *l;
        tok *t;

        arg0 = argv[0];

        if (argv[1] == NULL)
                return 0;

        l = lex_open(argv[1]);

        t = lex(l);
        while (t != NULL) {
                t = lex(l);
        }

        return 0;
}