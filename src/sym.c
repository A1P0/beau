/*
 * scope.c
 * Symbols and scoping for beau
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>
#include "beau.h"
#include "sym.h"
/* symbols */
sym *SYMS = NULL;
static sym *last = NULL;

sym * /* Search for a sym with given name in `s` (global scope if null) */
sym_search(char *name, sym *s)
{
        if (s == NULL)
                s = SYMS;
        else
                s = s->child;

        while (s != NULL) {

                if (!strcmp(name, s->name))
                        return s;

                s = s->next;

        }

        return NULL;
}

void /* add symbol to `scope->child`, global if NULL */
sym_add(sym *s, sym *scope)
{
        if (scope != NULL) {

                if (scope->child == NULL) {
                
                        scope->child = s;

                        scope->last = SYMS;
                
                        return;

                }

                scope->last->next = s;
                scope->last = scope->last->next;

                return;

        }

        if (SYMS == NULL) {
                
                SYMS = s;

                last = SYMS;
                
                return;

        }

        last->next = s;
        last = last->next;
}