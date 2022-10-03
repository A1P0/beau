/*
 * type.h
 * Definitions for type handling
 */
#ifndef _TYPE_H_
#define _TYPE_H_
#include <stdbool.h>

typedef struct type {
        
        bool sign;
        char *name;
        struct type *next;
} type;

type *type_search(char *name);
void type_init();
#endif