/*
 * lex.c
 * Lexer for lone
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lone.h"
#include "lex.h"
#include "tok.h"

static void /* retrieves next char from file, lexer->ch */
lgetc(lexer *l)
{
        l->ch = fgetc(l->file);
}

tok * /* Returns next token */
lex(lexer *l)
{
        tok *t = NULL;

        /* loop over whitespace */
        while ((l->ch != EOF) && (l->ch == ' ' || l->ch == '\t'
                || l->ch == '\n')) lgetc(l);

        if (l->ch == EOF) return NULL;

        /* not whitespace, not EOF, start checking for toks */
        printf("%c\n", l->ch);

        lgetc(l);

        t = lalloc(sizeof(tok));

        return t;
}

lexer * /* allocate lexer instance, open file, and return */
lex_open(char *filename)
{
        lexer *l;

        l = lalloc(sizeof(lexer));

        l->file = fopen(filename, "r");

        if (l->file == NULL)
                lfatal("Unable to open '%s': %s", filename, strerror(errno));

        l->name = filename;

        l->line = 1;

        /* populate first character */
        lgetc(l);

        return l;
}

void /* close file, free() lexer instance */
lex_close(lexer *l)
{
        if (l == NULL) return;

        fclose(l->file);

        free(l);
}