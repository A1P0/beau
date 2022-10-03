/*
 * type.c
 * Type handling for lone
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <llvm-c/Types.h>
#include <llvm-c/Core.h>
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
        t->type = LLVMVoidType();
        t->name = "void";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* byte */
        t->type = LLVMInt8Type();
        t->sign = true;
        t->name = "byte";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* ubyte */
        t->type = LLVMInt8Type();
        t->sign = false;
        t->name = "ubyte";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* word */
        t->type = LLVMInt16Type();
        t->sign = true;
        t->name = "word";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* uword */
        t->type = LLVMInt16Type();
        t->sign = false;
        t->name = "uword";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* dword */
        t->type = LLVMInt32Type();
        t->sign = true;
        t->name = "dword";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* udword */
        t->type = LLVMInt32Type();
        t->sign = false;
        t->name = "udword";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* qword */
        t->type = LLVMInt64Type();
        t->sign = true;
        t->name = "qword";

        t->next = balloc(sizeof(type));
        t = t->next;

        /* uqword */
        t->type = LLVMInt64Type();
        t->sign = false;
        t->name = "uqword";
}