/*
 * rvalue.c
 * rvalue parsing for lone
 */
#include <stdio.h>
#include "lone.h"
#include "parse.h"
#include "node.h"
#include "tok.h"
#include "rvalue.h"

/* lvalue  = name
        | "@", rvalue ; */
static node *
lvalue(parser *p)
{
        node *n = NULL;

        /* name */
        if (p->curr->type == T_NAME) {
                n = lalloc(sizeof(node));
                n->type = N_NAME;
                n->token = p->curr;
                plex(p);
                return n;
        }

        /* "@", rvalue */
        if (p->curr->type == T_AT) {
                plex(p);

                n = lalloc(sizeof(node));

                n->type = N_AT;
                n->token = p->curr;

                n->right = rvalue(p);

                if (n->right == NULL) {
                        lfatal("%s: Line %d: Expecting rvalue after @.",
                                p->l->name, p->curr->line);
                }

                return n;
        }

        return n;
}

/* rvalue  = "(", rvalue, ")"
        | lvalue
        | literal
        | lvalue, assign, rvalue
        | inc_dec, lvalue
        | lvalue, inc_dec
        | unary, rvalue
        | "%", lvalue
        | rvalue, binary, rvalue
        | rvalue, "?", rvalue, ":", rvalue
        | rvalue, "(", { rvalue, { ",", rvalue  } }, ")" ; */
node *
rvalue(parser *p)
{
        node *n = NULL;

        n = lvalue(p);
        if (n != NULL)
                return n;

        return n;
}