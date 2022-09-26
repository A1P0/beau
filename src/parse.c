/*
 * parse.c
 * Parser for lone
 */
#include <stdio.h>
#include <stdlib.h>
#include "lone.h"
#include "lex.h"
#include "tok.h"
#include "node.h"
#include "parse.h"

/* strings representing node types */
const char *nodestr[] = {
        #define X(y) #y,
                NODETYPES
        #undef X
};

static void /* next token, update tokens in parser instance */
plex(parser *p)
{
        p->curr = p->next;
        p->next = lex(p->l);

        if (p->next == NULL) {
                p->next = lalloc(sizeof(tok));
                p->next->type = T_NONE;
        }
}

/* "include", [ name | str_lit ], { ",", (name | str_lit) }, ";" ; */
static node *
include(parser *p)
{
        node *n = NULL;

        if (p->curr->type != T_INCLUDE)
                return NULL;

        n = lalloc(sizeof(node));

        n->type = N_INCLUDE;

        plex(p);

        return n;

        return NULL;
}

static node * /* program = { definition | include } ; */
program(parser *p)
{
        node *n = NULL;

        n = include(p);
        if (n != NULL)
                return n;

        /*n = definition(p);
        if (n != NULL)
                return n;*/

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

        last = tree;
        while (p->curr->type != T_NONE && last != NULL) {
                last->next = program(p);
                last = last->next;
        }

        return tree;
}

node * /* parse input file into tree and return */
parse(char *filename)
{
        parser *p;

        p = lalloc(sizeof(parser));

        p->l = lex_open(filename);

        p->curr = NULL;
        p->next = NULL;

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

        printf("%s\n", nodestr[n->type]);

        if (n->left != NULL) {
                for (i=0;i<indent;i++) putchar('\t');
                printf("LEFT:\n");
                nprint(n->left, indent+1); 
        }

        if (n->middle != NULL) {
                for (i=0;i<indent;i++) putchar('\t');
                printf("MIDDLE:\n");
                nprint(n->middle, indent+1); 
        }

        if (n->right != NULL) {
                for (i=0;i<indent;i++) putchar('\t');
                printf("RIGHT:\n");
                nprint(n->right, indent+1); 
        }

        if (n->next != NULL) {
                for (i=0;i<indent;i++) putchar('\t');
                nprint(n->left, indent); 
        }
}