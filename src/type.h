/*
 * type.h
 * Definitions for type handling
 */
#ifndef _TYPE_H_
#define _TYPE_H_
#include <stdbool.h>

typedef enum basetype {
        B_VOID,
        B_BYTE,
        B_WORD,
        B_DWORD,
        B_QWORD
} basetype;

typedef struct type {
        basetype type;
        bool sign;
        char *name;
        struct type *next;
} type;

type *type_search(char *name);
void type_init();
#endif