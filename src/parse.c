/*
 * parse.c
 * Parser for beau
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "beau.h"
#include "lex.h"
#include "tok.h"
#include "node.h"
#include "parse.h"
#include "rvalue.h"

extern const char *tokstr[];
extern const char *nodestr[];
extern const bool TOKENTABLE;

/* strings representing node types */
const char *nodestr[] = {
        #define X(y) #y,
                NODETYPES
        #undef X
};

void /* next token, update tokens in parser instance */
plex(parser *p)
{
        p->curr = p->next;
        p->next = lex(p->l);

        if (p->next == NULL) {

                p->next = balloc(sizeof(tok));

                p->next->type = T_NONE;
                p->next->line = p->l->line;

        }

        if (TOKENTABLE && p->curr != NULL)
                printf("%-12s%12d%24s\n",
                        tokstr[p->curr->type], p->curr->line, p->curr->string);
}

node * /* num_lit | char_lit | str_lit | bool_lit | "null" ; */
literal(parser *p)
{
        node *n = NULL;

        switch (p->curr->type) {

        case T_DEC_LIT: case T_CHAR_LIT: case T_STR_LIT: case T_TRUE:
        case T_FALSE: case T_NULL:

                n = balloc(sizeof(node));

                n->type = N_LITERAL;
                n->token = p->curr;

                plex(p);

        }

        return n;
}

static node * /* "include", [ name | str_lit ], ";" ; */
include(parser *p)
{
        node *n = NULL;

        if (p->curr->type != T_INCLUDE)
                return NULL;

        plex(p);

        if (p->curr->type != T_NAME && p->curr->type != T_STR_LIT)
                bfatal("%s: Line %d: Expecting a name or string in include "
                        "statement.", p->l->name, p->curr->line);

        n = balloc(sizeof(node));

        n->type = N_INCLUDE;
        n->token = p->curr;

        plex(p);

        if (p->curr->type != T_SEMI)
                bfatal("%s: Line %d: Expecting a ; to end include statement. ",
                        p->l->name, n->token->line);

        plex(p);

        return n;
}

static node * /* name, [ "@" ], name, [ ":", literal ] ; */
vardef(parser *p)
{
        node *n = NULL;

        if (p->curr->type != T_NAME)
                return NULL;

        n = balloc(sizeof(node));

        n->token = p->curr;
        n->datatype = p->curr->string;
        n->type = N_VARDEF;

        plex(p);

        if (p->curr->type == T_AT) {

                n->ptr = true;

                plex(p);

        }

        if (p->curr->type != T_NAME)
                bfatal("%s: Line %d: Expecting a name for definition.",
                        p->l->name, p->curr->line);

        n->name = p->curr->string;
        plex(p);

        if (p->curr->type != T_COLON)
                return n;

        plex(p);
        n->middle = literal(p);

        if (n->middle == NULL)
                bfatal("%s: Line %d: Expecting a literal at end of definition.",
                        p->l->name, p->curr->line);

        return n;
}

static node * /* "ret", [rvalue] */
ret_stmt(parser *p)
{
        node *n = NULL;

        if (p->curr->type != T_RET)
                return NULL;

        plex(p);

        n = balloc(sizeof(node));

        n->type = N_RET;
        n->right = rvalue(p);

        return n;
}

/* label_stmt | block_stmt | if_stmt | while_stmt | switch_stmt
              | case_stmt | break_stmt | goto_stmt | ret_stmt | rval_stmt ; */
static node * 
statement(parser *p)
{
        node *n = NULL;
        node *nn = NULL;
        int line = 0;

        /* null statement */
        if (p->curr->type == T_SEMI) {

                plex(p);

                n = balloc(sizeof(node));

                n->type = N_NONE;

                return n;

        }

        /* block_stmt */
        if (p->curr->type == T_LBRACE) {

                line = p->curr->line;

                plex(p);

                n = statement(p);

                nn = n;
                while (nn != NULL) {

                        /* make skip to last node if block stmt */
                        while (nn->next != NULL)
                                nn = nn->next;

                        nn->next = statement(p);

                        nn = nn->next;

                }

                if (p->curr->type != T_RBRACE) {
                        printf("wtf: %s\n", tokstr[p->curr->type]);
                        bfatal("%s: Line %d: Started block statement missing "
                                "closing }.", p->l->name, line);
                }

                plex(p);

                if (n == NULL) {

                        n = balloc(sizeof(node));

                        n->type = N_NONE;

                }

                return n;

        }

        /* rval_stmt */
        n = rvalue(p);
        if (n != NULL) goto done;
        
        /* ret_stmt */
        n = ret_stmt(p);
        if (n != NULL) goto done;

        if (n == NULL)
                return NULL;

        done:

        if (p->curr->type != T_SEMI)
                bfatal("%s: Line %d: Expecting a ; to end statement.",
                        p->l->name,p->curr->line);

        plex(p);

        return n;
}

static node * /* { ( vardef, ";" ) | funcdef } ; */
definition(parser *p)
{
        node *n = NULL;
        node *l = NULL;

        if (p->curr->type != T_NAME)
                return NULL;

        n = balloc(sizeof(node));
        
        n->token = p->curr;
        n->datatype = p->curr->string;
        n->ptr = false;

        plex(p);

        if (p->curr->type == T_AT) {

                n->ptr = true;

                plex(p);

        }

        if (p->curr->type != T_NAME)
                bfatal("%s: Line %d: Expecting a name for definition.",
                        p->l->name, p->curr->line);

        n->name = p->curr->string;
        plex(p);

        if (p->curr->type == T_SEMI) {

                /* vardef, no init */
                n->type = N_VARDEF;
                plex(p);

                return n;

        }

        /* test for function */
        if (p->curr->type == T_LPAREN) {

                plex(p);
                n->left = vardef(p);

                n->type = N_FUNCDEF;

                /* potential list */
                l = n->left;
                while (p->curr->type == T_COMMA) {

                        plex(p);

                        l->next = vardef(p);
                        l = l->next;

                }

                /* ) */
                if (p->curr->type != T_RPAREN)
                        bfatal("%s: Line %d: Missing closing ) in function "
                                "definition." , p->l->name, p->curr->line);

                plex(p);

                if (p->curr->type == T_SEMI) {

                        /* function prototype, finish up. */
                        plex(p);

                        return n;

                }

                if (p->curr->type != T_COLON)
                        bfatal("%s: Line %d: Expecting ; or : in function "
                                "definition.", p->l->name, p->curr->line);

                plex(p);

                n->right = statement(p);

                return n;

        }

        /* extended variable definition? */
        if (p->curr->type != T_COLON)
                bfatal("%s: Line %d: Expecting a ; to end definition or a : "
                        "to initialize." , p->l->name, p->curr->line);

        plex(p);

        n->middle = literal(p);

        if (n->middle == NULL)
                bfatal("%s: Line %d: A literal is required to initialize a "
                        "variable.", p->l->name, p->curr->line);

        if (p->curr->type != T_SEMI)
                bfatal("%s: Line %d: Expecting a ; to end definition",
                        p->l->name, p->curr->line);

        plex(p);

        n->type = N_VARDEF;

        return n;
}

static node * /* program = { definition | include } ; */
program(parser *p)
{
        node *n = NULL;

        n = include(p);
        if (n != NULL)
                return n;

        n = definition(p);
        if (n != NULL)
                return n;

        return NULL;
}

static node * /* parse root level */
root(parser *p)
{
        node *tree = NULL;
        node *last = NULL;

        tree = program(p);
        if (tree == NULL)
                return NULL;

        /* see if there's more appended */
        last = tree;
        while (last->next != NULL)
                last = last->next;

        while (p->curr->type != T_NONE && last != NULL) {

                last->next = program(p);
                last = last->next;

                /* see if there's more appended */
                while (last->next != NULL)
                        last = last->next;

        }

        return tree;
}

node * /* parse input file into tree and return */
parse(char *filename)
{
        parser *p;

        p = balloc(sizeof(parser));

        p->l = lex_open(filename);

        p->curr = NULL;
        p->next = NULL;

        if (TOKENTABLE)
                printf("%-12s%12s%24s\n", "TOKEN TYPE", "LINE", "STRING");

        plex(p); plex(p);
        p->tree = root(p);

        lex_close(p->l);

        return p->tree;
}

void /* print a given node and children/siblings */
nprint(node *n, int indent)
{
        int i;

        if (n == NULL)
                return;

        for (i=0;i<indent;i++) putchar('\t');

        printf("%s", nodestr[n->type]);

        if (n->type == N_NAME)
                printf(", '%s'", n->token->string);

        printf("\n");

        if (n->left != NULL) {

                for (i=0;i<indent;i++) putchar('\t');

                printf(" |->L:\n");
                nprint(n->left, indent+1); 

        }

        if (n->middle != NULL) {

                for (i=0;i<indent;i++) putchar('\t');

                printf(" |->M:\n");
                nprint(n->middle, indent+1); 

        }

        if (n->right != NULL) {

                for (i=0;i<indent;i++) putchar('\t');

                printf(" |->R:\n");
                nprint(n->right, indent+1); 

        }

        if (n->next != NULL)
                nprint(n->next, indent);

}