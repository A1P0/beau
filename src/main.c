/*
 * main.c
 * Entry point, contents volatile as hell, early development testing.
 */
#include <stdio.h>
#include <getopt.h>
#include "lone.h"
#include "lex.h"
#include "parse.h"

/* How lone was invoked */
char *arg0;
/* strings of token types for printing token table */
extern char *tokstr[];

static void /* display options */
help()
{
        printf(
                "Current options:\n\t-l [filename]\tPrints table of tokens "
                "in current file.\n"
                "\t-p [filename]\tParses input file and prints syntax tree.\n"
        );
}

static void /* lex input file and print table of tokens */
lexopt(char *filename)
{
        lexer *l;
        tok *t;

        l = lex_open(filename);

        t = lex(l);

        printf("%-12s%12s%24s\n", "TOKEN TYPE", "LINE", "STRING");

        while (t != NULL) {

                printf("%-12s%12d%24s\n", tokstr[t->type], t->line, t->string);

                t = lex(l);
                
        }

        lex_close(l);
}

static void /* parse input file and print syntax tree */
parseopt(char *filename)
{
        node *tree;

        tree = parse(filename);

        nprint(tree, 0);
}

int /* volatile testing */
main(int argc, char *argv[])
{
        int opt;

        arg0 = argv[0];

        while ((opt = getopt(argc, argv, "-:hl:p:")) != -1)  {
                switch (opt) {
                case 'h':
                        help();
                        return 0;

                case 'l':
                        lexopt(optarg);
                        return 0;

                case 'p':
                        parseopt(optarg);
                        return 0;

                case '?':
                        lfatal("Unknown option '%c' supplied.",
                                optopt);
                        break;

                case ':':
                        lfatal("Missing argument for option '%c'.",
                                optopt);
                        break;

                case 1:
                        lfatal("Unexpected argument '%s'.", optarg);
                        break;
                }
        }

        help();

        return 0;
}