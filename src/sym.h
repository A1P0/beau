/*
 * sym.h
 * Definitions for symbols and scoping
 */
#ifndef _SCOPE_H_
#define _SCOPE_H_

/* Symbol types */
typedef enum symtype {
        S_NONE,
        S_VAR,
        S_FUNC
} symtype;
/*
 * Main type for symbols and scoping
 * Used to construct one tree of symbols that imply scoping
 */
typedef struct sym {
        char *name;
        char *datatype;
        symtype type;
        struct sym *child;
        struct sym *next;
} sym;
#endif