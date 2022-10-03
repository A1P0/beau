/*
 * type.c
 * Type handling for lone
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "beau.h"
#include "type.h"

type *TYPES;

type * /* find type of given name in table and return */
type_search(char *name)
{
        type *t;

        t = TYPES;

        while (t != NULL) {

                if (!strcmp(name, t->name))
                        return t;

                t = t->next;

        }

        return NULL;
}

void
type_init()
{
        type *t;
        
        TYPES = balloc(sizeof(type));

        t = TYPES;

        /* void */
        // t->type = ;
        t->name = "void";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* byte */
        // t->type = ;
        t->sign = true;
        t->name = "byte";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* ubyte */
        // t->type = ;
        t->sign = false;
        t->name = "ubyte";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* word */
        // t->type = ;
        t->sign = true;
        t->name = "word";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* uword */
        // t->type = ;
        t->sign = false;
        t->name = "uword";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* dword */
        // t->type = ;
        t->sign = true;
        t->name = "dword";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* udword */
        // t->type = ;
        t->sign = false;
        t->name = "udword";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* qword */
        // t->type = ;
        t->sign = true;
        t->name = "qword";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* uqword */
        // t->type = ;
        t->sign = false;
        t->name = "uqword";
}