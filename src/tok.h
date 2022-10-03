/*
 * tok.h
 * Token definitions for beau
 */
#ifndef _TOK_H_
#define _TOK_H_

#define TOKTYPES \
        X(T_NONE)       \
        X(T_NAME)       \
        X(T_DEC_LIT)    \
        X(T_CHAR_LIT)   \
        X(T_STR_LIT)    \
        X(T_TRUE)       \
        X(T_FALSE)      \
        X(T_NULL)       \
        X(T_AT)         \
        X(T_LBRACKET)   \
        X(T_RBRACKET)   \
        X(T_LPAREN)     \
        X(T_RPAREN)     \
        X(T_LBRACE)     \
        X(T_RBRACE)     \
        X(T_COMMA)      \
        X(T_SEMI)       \
        X(T_COLON)      \
        X(T_QUESTION)   \
        X(T_SLASH)      \
        X(T_ASTERISK)   \
        X(T_PERCENT)    \
        X(T_PLUS)       \
        X(T_MINUS)      \
        X(T_SHIFT_LEFT) \
        X(T_SHIFT_RIGHT)\
        X(T_GTHAN_EQTO) \
        X(T_GTHAN)      \
        X(T_LTHAN_EQTO) \
        X(T_LTHAN)      \
        X(T_AND)        \
        X(T_OR)         \
        X(T_NOTEQUAL)   \
        X(T_ISEQUAL)    \
        X(T_AMPER)      \
        X(T_PIPE)       \
        X(T_NOT)        \
        X(T_INC)        \
        X(T_DEC)        \
        X(T_ASSIGN)     \
        X(T_ASSIGN_SUB) \
        X(T_ASSIGN_ADD) \
        X(T_ASSIGN_MOD) \
        X(T_ASSIGN_MUL) \
        X(T_ASSIGN_DIV) \
        X(T_ASSIGN_AND) \
        X(T_ASSIGN_OR)  \
        X(T_RET)        \
        X(T_GOTO)       \
        X(T_CONTINUE)   \
        X(T_BREAK)      \
        X(T_CASE)       \
        X(T_SWITCH)     \
        X(T_WHILE)      \
        X(T_IF)         \
        X(T_ELSE)       \
        X(T_INCLUDE)

/* Types of tokens */
typedef enum toktype {
        #define X(y) y,
                TOKTYPES
        #undef X
} toktype;

/* Token object */
typedef struct tok {
        toktype type;   /* type of token */
        int offset;     /* offset in source file */
        int line;       /* line in source file */
        int length;     /* length of string (if applicable) */
        char *string;   /* string value (if needed) */
        char *file;     /* name of file its originated from */
} tok;

#endif