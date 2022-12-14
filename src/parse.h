/*
 * parse.h
 * Definitions for Beau parser
 */
#ifndef _PARSE_H_
#define _PARSE_H_
#include "lex.h"
#include "tok.h"
#include "node.h"

/* parser instance */
typedef struct parser {
        lexer *l;       /* lexer instance */
        tok *curr;      /* current token */
        tok *next;      /* next token */
        node *tree;     /* root of tree */
} parser;

void plex(parser *p);
node *literal(parser *p);
node *parse(char *filename);
void nprint(node *n, int indent);
#endif