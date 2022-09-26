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

const char *tokstr[] = {
        #define X(y) #y,
                TOKTYPES
        #undef X
};

static void /* retrieves next char from file, lexer->ch */
lgetc(lexer *l)
{
        l->ch = fgetc(l->file);
}

static void /* test for a name */
name(lexer *l, tok *t)
{
        if ((l->ch >= 'a' && l->ch <= 'z') || (l->ch >= 'A' && l->ch <= 'Z')
        || l->ch == '_') {

                t->offset = ftell(l->file);
                t->length = 1;

                while ((l->ch >= 'a' && l->ch <= 'z')
                || (l->ch >= 'A' && l->ch <= 'Z')
                || (l->ch >= '0' && l->ch <= '9')
                || l->ch == '_') {

                        t->length++;
                        lgetc(l);

                }
                /* todo: get string */
                t->type = T_NAME;
        }
        
        return;
}

tok * /* Returns next token */
lex(lexer *l)
{
        char last = ' ';
        tok *t = NULL;

        whitespace:

        /* End of file */
        if (l->ch == EOF) return NULL;

        /* loop over whitespace */
        while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n') {
                /* line count */
                if (l->ch == '\n')
                        l->line++;

                lgetc(l);
        }

        /* test for comment, /, and /= */
        if (l->ch == '/') {
                
                lgetc(l);

                /* comment */
                if (l->ch == '*') {

                        while (l->ch != EOF) {
                                
                                if (l->ch == '/' && last == '*') {
                                        lgetc(l);
                                        break;
                                }

                                if (l->ch == '\n')
                                        l->line++;

                                last = l->ch;
                                lgetc(l);
                        }

                        if (l->ch == EOF) {
                                lfatal("%s: Line %d: Comment not closed.",
                                        l->name, l->line);
                        }

                        goto whitespace;

                }

                /* / or /= */

                t = lalloc(sizeof(tok));

                t->line = l->line;
                t->file = l->name;
                t->type = T_SLASH;

                if (l->ch == '=') {
                        t->type = T_ASSIGN_DIV;
                        lgetc(l);
                }

                return t;
        }

        t = lalloc(sizeof(tok));

        t->line = l->line;
        t->file = l->name;
        t->type = T_NONE;

        /* easy single char no conflict tokens */
        switch (l->ch) {
        case '@': t->type = T_AT;       break;
        case '[': t->type = T_LBRACKET; break;
        case ']': t->type = T_RBRACKET; break;
        case '(': t->type = T_LPAREN;   break;
        case ')': t->type = T_RPAREN;   break;
        case '{': t->type = T_LBRACE;   break;
        case '}': t->type = T_RBRACE;   break;
        case ',': t->type = T_COMMA;    break;
        case ';': t->type = T_SEMI;     break;
        case ':': t->type = T_COLON;    break;
        case '?': t->type = T_QUESTION; break;
        }

        if (t->type != T_NONE) {
                lgetc(l);

                return t;
        }

        /* conflict tokens/simple multi char */
        switch (l->ch) {
        case '+': /* +, ++ , and += */
                t->type = T_PLUS;
                lgetc(l);

                if (l->ch == '+') {
                        t->type = T_INC;
                        lgetc(l);
                }

                if (l->ch == '=') {
                        t->type = T_ASSIGN_ADD;
                        lgetc(l);
                }

                return t;

        case '-': /* -, --, -= */
                t->type = T_MINUS;
                lgetc(l);

                if (l->ch == '-') {
                        t->type = T_DEC;
                        lgetc(l);
                }

                if (l->ch == '=') {
                        t->type = T_ASSIGN_SUB;
                        lgetc(l);
                }

                return t;

        case '<': /* <, <<, and <= */
                t->type = T_LTHAN;
                lgetc(l);

                if (l->ch == '<') {
                        t->type = T_SHIFT_LEFT;
                        lgetc(l);
                }

                if (l->ch == '=') {
                        t->type = T_LTHAN_EQTO;
                        lgetc(l);
                }

                return t;

        case '>': /* >, >>, and >= */
                t->type = T_GTHAN;
                lgetc(l);

                if (l->ch == '>') {
                        t->type = T_SHIFT_RIGHT;
                        lgetc(l);
                }

                if (l->ch == '=') {
                        t->type = T_GTHAN_EQTO;
                        lgetc(l);
                }

                return t;

        case '!': /* ! and != */
                t->type = T_NOT;
                lgetc(l);

                if (l->ch == '=') {
                        t->type = T_NOTEQUAL;
                        lgetc(l);
                }

                return t;

        case '=': /* = and == */
                t->type = T_ASSIGN;
                lgetc(l);

                if (l->ch == '=') {
                        t->type = T_ISEQUAL;
                        lgetc(l);
                }

                return t;

        case '/': /* / and /= */
                t->type = T_SLASH;
                lgetc(l);

                if (l->ch == '=') {
                        t->type = T_ASSIGN_DIV;
                        lgetc(l);
                }

                return t;

        case '*': /* * and *= */
                t->type = T_ASTERISK;
                lgetc(l);

                if (l->ch == '=') {
                        t->type = T_ASSIGN_MUL;
                        lgetc(l);
                }

                return t;

        case '%': /* % and %= */
                t->type = T_PERCENT;
                lgetc(l);

                if (l->ch == '=') {
                        t->type = T_ASSIGN_MOD;
                        lgetc(l);
                }

                return t;

        case '&': /* & and &= */
                t->type = T_AMPER;
                lgetc(l);

                if (l->ch == '=') {
                        t->type = T_ASSIGN_AND;
                        lgetc(l);
                }

                return t;

        case '|': /* | and |= */
                t->type = T_PIPE;
                lgetc(l);

                if (l->ch == '=') {
                        t->type = T_ASSIGN_OR;
                        lgetc(l);
                }

                return t;

        }

        /* test for a name */
        name(l, t);
        if (t->type != T_NONE)
                return t;

        if (t->type == T_NONE)
                return NULL;

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