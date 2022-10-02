/*
 * type.h
 * Definitions for type handling
 */
#ifndef _TYPE_H_
#define _TYPE_H_
#include <stdbool.h>
#include <llvm-c/Types.h>
#include <llvm-c/Core.h>

typedef struct type {
        LLVMTypeRef type;
        bool sign;
        char *name;
        struct type *next;
} type;

type *type_search(char *name);
void type_init();
#endif