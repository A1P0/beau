/*
 * main.c
 * Entry point, contents volatile as hell, early development testing.
 */
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include "beau.h"
#include "lex.h"
#include "parse.h"
#include "qbe.h"

/* How beau was invoked */
char *arg0;
/* flags for some options */
bool TOKENTABLE = false;
bool NODETREE = false;
/* strings of token types for printing token table */
extern char *tokstr[];

static void /* display options */
help()
{
        printf(
                "Current options:\n"
                "\t-c [filename]\tAttempt to compile said file.\n"
                "\t-t\tPrint table of tokens encountered while compiling.\n"
                "\t-n\tPrint tree of nodes encountered while compiling.\n"
        );
}

void compileopt(char *filename)
{
        node *tree;

        tree = parse(filename);

        if (NODETREE)
                nprint(tree, 0);

        compile(tree, filename);
}

int /* volatile testing */
main(int argc, char *argv[])
{
        int opt;

        arg0 = argv[0];

        while ((opt = getopt(argc, argv, "-:htnc:")) != -1)  {

                switch (opt) {

                case 'h':
                        help();
                        return 0;

                case 't':
                        TOKENTABLE = true;
                        break;

                case 'n':
                        NODETREE = true;
                        break;

                case 'c':
                        compileopt(optarg);
                        break;

                case '?':
                        bfatal("Unknown option '%c' supplied.",
                                optopt);
                        break;

                case ':':
                        bfatal("Missing argument for option '%c'.",
                                optopt);
                        break;

                case 1:
                        bfatal("Unexpected argument '%s'.", optarg);
                        break;
                        
                }

        }

        return 0;
}