/*
 * sym.h
 * Definitions for symbols and scoping
 */
#ifndef _SCOPE_H_
#define _SCOPE_H_
#include <stdbool.h>
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>
#include "type.h"
/* Symbol types */
typedef enum symtype {
        S_NONE,
        S_VAR,
        S_FUNC
} symtype;
/*
 * Main type for symbols and scoping
 * Used to construct one tree of symbols/scopes
 */
typedef struct sym {
        char *name;
        type *datatype;
        symtype type;
        bool pointer;
        LLVMValueRef value;
        struct sym *child;
        struct sym *last;
        struct sym *next;
} sym;

sym *sym_search(char *name, sym *s);
void sym_add(sym *s, sym *scope);
#endif