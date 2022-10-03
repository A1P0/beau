/*
 * rvalue.c
 * rvalue parsing for Beau
 */
#include <stdio.h>
#include "beau.h"
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
        node *r = NULL;

        /* "@", rvalue */
        if (p->curr->type == T_AT) {

                plex(p);

                n = balloc(sizeof(node));

                n->type = N_AT;
                n->token = p->curr;

                r = rvalue(p);

                if (r == NULL)
                        bfatal("%s: Line %d: Expecting rvalue after @.",
                                p->l->name, p->curr->line);

                n->right = r;

                return n;

        }

        /* name */
        if (p->curr->type == T_NAME) {

                n = balloc(sizeof(node));

                n->type = N_NAME;
                n->token = p->curr;

                plex(p);

                return n;

        }

        return n;
}

static node * /* "%", lvalue */
addressof(parser *p)
{
        node *n = NULL;
        node *lval = NULL;

        if (p->curr->type != T_PERCENT)
                return NULL;
                
        plex(p);

        lval = lvalue(p);

        if (lval == NULL)
                bfatal("%s: Line %d: Expecting lvalue as operand of %.",
                        p->l->name, p->curr->line);

        n = balloc(sizeof(node));

        n->type = N_ADDRESSOF;
        n->right = lval;

        return n;
}

static node * /* inc_dec, lvalue */
prefix_incdec(parser *p)
{
        node *n = NULL;
        node *lval = NULL;

        if (p->curr->type != T_INC && p->curr->type != T_DEC)
                return NULL;

        n = balloc(sizeof(node));

        if (p->curr->type == T_INC)
                n->type = N_INC;
        else
                n->type = N_DEC;

        plex(p);

        lval = lvalue(p);

        if (lval == NULL)
                bfatal("%s: Line %d: Expecting lvalue as operand of "
                        "prefix increment operator.", p->l->name,
                        p->curr->line);

        n->right = lval;

        return n;
}

static node * /* lvalue, inc_dec */
postfix_incdec(parser *p, node *lval)
{
        node *n = NULL;

        if (p->curr->type != T_INC && p->curr->type != T_DEC)
                return NULL;

        n = balloc(sizeof(node));

        if (p->curr->type == T_INC)
                n->type = N_INC;
        else
                n->type = N_DEC;

        n->token = p->curr;
        n->left = lval;

        plex(p);

        return n;
}

static node * /* lvalue, assign, rvalue */
assign(parser *p, node *lval)
{
        node *n = NULL;

        if (!(p->curr->type >= T_ASSIGN && p->curr->type <= T_ASSIGN_OR))
                return NULL;

        n = balloc(sizeof(node));

        n->type = N_ASSIGN;
        n->token = p->curr;
        n->left = lval;

        plex(p);

        n->right = rvalue(p);

        if (n->right == NULL)
                bfatal("%s: Line %d: Expecting an rvalue on "
                        "right side of assignment.",
                        p->l->name, p->curr->line);

        return n;
}

static node * /* unary, rvalue */
unary(parser *p)
{
        node *n = NULL;
        node *r = NULL;
        toktype t = N_NONE;

        if (p->curr->type != T_NOT && p->curr->type != T_MINUS)
                return NULL;

        t = p->curr->type;

        plex(p);

        r = rvalue(p);

        if (r == NULL)
                bfatal("%s: Line %d: Expecting an rvalue as operand of unary "
                        "operation.", p->l->name, p->curr->line);

        n = balloc(sizeof(node));

        if (t == T_NOT)
                n->type = N_UNARY_NOT;
        else
                n->type = N_UNARY_MINUS;

        if (r->left != NULL) {

                n->right = r->left;
                r->left = n;

                return r;

        }

        n->right = r;

        return n;
}

static char
precedence(nodetype t)
{
        switch (t) {

        case N_MUL: case N_DIV: case N_MOD:
                return 10;

        case N_ADD: case N_SUB:
                return 9;

        case N_SHIFT_LEFT: case N_SHIFT_RIGHT:
                return 8;

        case N_LTHAN: case N_GTHAN: case N_LTHAN_EQTO:
        case N_GTHAN_EQTO:
                return 7;

        case N_NOTEQUAL: case N_ISEQUAL:
                return 6;

        case N_BITWISE_AND: case N_BITWISE_OR:
                return 5;

        }

        return 11;
}

static node * /* rvalue, binary, rvalue */
binary(parser *p, node *l)
{
        node *n = NULL;
        node *r = NULL;
        nodetype t = N_NONE;

        switch (p->curr->type) {

                case T_PIPE: t = N_BITWISE_OR; break;

                case T_AMPER: t = N_BITWISE_AND; break;

                case T_ISEQUAL: t = N_ISEQUAL; break;

                case T_NOTEQUAL: t = N_NOTEQUAL; break;

                case T_LTHAN: t = N_LTHAN; break;

                case T_LTHAN_EQTO: t = N_LTHAN_EQTO; break;

                case T_GTHAN: t = N_GTHAN; break;

                case T_GTHAN_EQTO: t = N_GTHAN_EQTO; break;

                case T_SHIFT_LEFT: t = N_SHIFT_LEFT; break;

                case T_SHIFT_RIGHT: t = N_SHIFT_RIGHT; break;

                case T_MINUS: t = N_SUB; break;

                case T_PLUS: t = N_ADD; break;

                case T_PERCENT: t = N_MOD; break;

                case T_ASTERISK: t = N_MUL; break;

                case T_SLASH: t = N_DIV; break;

        }

        if (t == N_NONE) return NULL;

        n = balloc(sizeof(node));

        n->type = t;
        n->token = p->curr;
        n->left = l;

        plex(p);
        r = rvalue(p);

        if (r == NULL)
                bfatal("%s: Line %d: Expecting an rvalue on right side of "
                        "binary operator.", p->l->name, p->curr->line);

        if (precedence(n->type) >= precedence(r->type)) {

                n->right = r->left;
                r->left = n;

                return r;

        }

        n->right = r;


        return n;
}

static node * /* "(", rvalue, ")" */
parens(parser *p)
{
        node *n = NULL;
        node *r = NULL;

        if (p->curr->type != T_LPAREN)
                return NULL;

        plex(p);

        r = rvalue(p);

        if (r == NULL)
                bfatal("%s: Line %d: Expecting an rvalue inside of ( ).",
                        p->l->name, p->curr->line);

        if (p->curr->type != T_RPAREN)
                bfatal("%s: Line %d: Missing ).",
                        p->l->name, p->curr->line);

        plex(p);

        n = balloc(sizeof(node));

        n->type = N_PARENS;
        n->middle = r;

        return n;
}

static node * /* rvalue, "(", { rvalue, { ",", rvalue  } }, ")" */
funcall(parser *p, node *l)
{
        node *n = NULL;
        node *nn = NULL;

        if (p->curr->type != T_LPAREN)
                return NULL;

        plex(p);

        n = balloc(sizeof(node));

        n->type = N_FUNCALL;
        n->left = l;

        if (p->curr->type == T_RPAREN) {

                /* no arguments */
                plex(p);

                n->right = NULL;

                return n;

        }

        n->right = rvalue(p);
        nn = n->right;

        while (nn != NULL) {

                if (p->curr->type != T_COMMA) 
                        break;

                plex(p);

                nn->next = rvalue(p);

                nn = nn->next;
        }

        if (p->curr->type != T_RPAREN)
                bfatal("%s: Line %d: Expecting ) to end function call.",
                        p->l->name, p->curr->line);

        plex(p);

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
        | rvalue, "(", { rvalue, { ",", rvalue  } }, ")" ; */
node *
rvalue(parser *p)
{
        node *n = NULL;
        node *lval = NULL;
        node *rval = NULL;

        /* inc_dec, lvalue */
        n = prefix_incdec(p);
        if (n != NULL) return n;

        /* "%", lvalue */
        n = addressof(p);
        if (n != NULL) return n;

        /* unary, rvalue */
        n = unary(p);
        if (n != NULL) return n;

        /* rules starting with an lvalue */
        lval = lvalue(p);
        if (lval != NULL) {

                /* lvalue, inc_dec */
                n = postfix_incdec(p, lval);
                if (n != NULL) return n;

                /* lvalue, assign, rvalue */
                n = assign(p, lval);
                if (n != NULL) return n;

                /* an lvalue can be an rvalue: setup for following tests */
                rval = lval;

        }

        /* check for a literal */
        if (rval == NULL) rval = literal(p);

        /* check for paren expression */
        if (rval == NULL) rval = parens(p);

        /* if we still got nothing, we got nothing */
        if (rval == NULL)
                return NULL;

        /*
         * Rules starting with rvalues
         */

        /* rvalue, binary, rvalue */
        n = binary(p, rval);
        if (n != NULL) return n;

        /* rvalue, "(", { rvalue, { ",", rvalue  } }, ")" */
        n = funcall(p, rval);
        if (n != NULL) return n;

        /* just an lvalue? */
        return rval;
}