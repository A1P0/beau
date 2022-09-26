/*
 * main.c
 * Entry point, contents volatile as hell, early development testing.
 */
#include <stdio.h>
#include "lone.h"
#include "lex.h"

/* How lone was invoked */
char *arg0;
/* strings of token types for printing token table */
extern char *tokstr[];

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

        printf("%-12s%12s%24s\n", "TYPE", "LINE", "STRING");

        while (t != NULL) {
                printf("%-12s%12d%24s\n", tokstr[t->type], t->line, t->string);
                t = lex(l);
        }

        return 0;
}