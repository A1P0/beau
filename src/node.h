/*
 * node.h
 * Node definitions for L1
 */
#ifndef _NODE_H_
#define _NODE_H_

#define NODETYPES \
        X(N_NONE)       \
        X(N_LVALUE)     \
        X(N_LITERAL)    \
        X(N_DIV)        \
        X(N_MUL)        \
        X(N_MOD)        \
        X(N_ADD)        \
        X(N_SUB)        \
        X(N_SHIFT_RIGHT)\
        X(N_SHIFT_LEFT) \
        X(N_GTHAN_EQTO) \
        X(N_GTHAN)      \
        X(N_LTHAN_EQTO) \
        X(N_LTHAN)      \
        X(N_AND)        \
        X(N_OR)         \
        X(N_NOTEQUAL)   \
        X(N_ISEQUAL)    \
        X(N_BITWISE_AND)\
        X(N_BITWISE_OR) \
        X(N_UNARY_MINUS)\
        X(N_UNARY_NOT)  \
        X(N_INC)        \
        X(N_DEC)        \
        X(N_ASSIGN_SUB) \
        X(N_ASSIGN_ADD) \
        X(N_ASSIGN_MOD) \
        X(N_ASSIGN_MUL) \
        X(N_ASSIGN_DIV) \
        X(N_CONDITIONAL)\
        X(N_FUNCALL)    \
        X(N_ADDRESSOF)  \
        X(N_RET)        \
        X(N_GOTO)       \
        X(N_CONTINUE)   \
        X(N_BREAK)      \
        X(N_CASE)       \
        X(N_SWITCH)     \
        X(N_WHILE)      \
        X(N_IF)         \
        X(N_LABEL)      \
        X(N_BLOCK)      \
        X(N_FUNCDEF)    \
        X(N_VARDEF)     \
        X(N_INCLUDE)

#endif