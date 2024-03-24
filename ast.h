#pragma once

#include "lexer.h"
#include <stdbool.h>

typedef int Ast_Literal;

typedef enum {
    OPEN_PAREN = '(',
    CLOSE_PAREN = ')',
} Ast_Separator;

typedef enum {
    OP_ADD = '+',
    OP_SUB = '-',
    OP_MUL = '*',
    OP_DIV = '/',
    OP_POW = '^',
    OP_MOD = '%',
} Ast_Operator;

typedef union {
    Ast_Literal lit;
    Ast_Separator sep;
    Ast_Operator op;
} Ast_NodeValue;

typedef enum {
    NODE_LITERAL = TOKEN_LITERAL,
    NODE_SEPARATOR = TOKEN_SEPARATOR,
    NODE_OPERATOR = TOKEN_OPERATOR,
} Ast_NodeKind;

typedef struct Ast_Node {
    Ast_NodeValue val;
    Ast_NodeKind kind;
    struct Ast_Node* lchild;
    struct Ast_Node* rchild;
    struct Ast_Node* parent;
} Ast_Node;

static const int OPORDER[] = {
    [OP_ADD] = 0, [OP_SUB] = 0, [OP_MUL] = 1,
    [OP_DIV] = 1, [OP_MOD] = 1, [OP_POW] = 2,
};

Ast_Node* Ast_Node_new(void);
/*
 * NOTE: passing in a EOFTOKEN returns a half initialized Ast_Node
 */
Ast_Node* Ast_Node_from_token(Token tok);

/*
 * res is a pointer to a list of pointers (T***)
 */
size_t gen_orphans(Ast_Node*** res, const Token* toks, size_t ntoks);

/*
 * orphans is a list of Ast_Node*s
 */
Ast_Node* gen_ast(Ast_Node** orphans, size_t norphans);

void print_ast(Ast_Node* root);
void sprint_ast(Ast_Node* root, char* restrict buf);