/*
 * lex.h
 * Definitions for lone lexer
 */
#ifndef _LEX_H_
#define _LEX_H_
#include "tok.h"

/* Lexer instance */
typedef struct lexer {
        FILE *file;     /* FILE handle */
        char *name;     /* Name of file */
        int line;       /* How many lines we've processed */
        char ch;        /* current character */
} lexer;

tok *lex(lexer *l);
lexer *lex_open(char *filename);
void lex_close(lexer *l);
#endif