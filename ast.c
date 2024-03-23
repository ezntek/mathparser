#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "lexer.h"

Ast_Node* Ast_Node_new() {
    Ast_Node* res;

    if ((res = malloc(sizeof(Ast_Node))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    *res = (Ast_Node){0};
    res->lchild = res->rchild = res->parent = NULL;

    return res;
}

Ast_Node* Ast_Node_from_token(Token tok) {
    Ast_Node* res;

    if ((res = malloc(sizeof(Ast_Node))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    *res = (Ast_Node){0};
    res->lchild = res->rchild = res->parent = NULL;

    if (tok.kind == TOKEN_NULL) {
        fputs("found null token in token list", stderr);
        exit(EXIT_FAILURE);
    }

    switch (tok.kind) {
        case TOKEN_LITERAL: {
            res->val.lit = atoi(tok.text);
        } break;
        case TOKEN_OPERATOR: {
            res->val.op = tok.text[0];
        } break;
        case TOKEN_SEPARATOR: {
            res->val.sep = tok.text[0];
        } break;
        default: {
            return res;
        } break;
    }

    res->kind = (Ast_NodeKind)tok.kind;

    return res;
}
