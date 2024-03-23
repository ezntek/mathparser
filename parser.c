#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "lexer.h"
#include "stack.h"

size_t gen_orphans(Ast_Node*** res, const Token* toks, size_t ntoks) {
    Ast_Node** nodes;
    size_t nodes_len = 0;
    size_t nodes_cap = 5;

    if ((nodes = malloc(sizeof(Ast_Node*))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < ntoks || toks[i].kind != TOKEN_EOF; i++) {
        if (nodes_len + 1 > nodes_cap) {
            nodes = realloc(nodes, sizeof(Ast_Node*) * 5);

            if (nodes == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        nodes[nodes_len++] = Ast_Node_from_token(toks[i]);
    }

    *res = nodes;
    return nodes_len;
}

Ast_Node* gen_ast(Ast_Node** orphans, size_t norphans) {
    Stack roots;
    Ast_Node* currnode;
    size_t pos = 0;

    Stack_init(&roots);

    while (pos < norphans) {
        currnode = orphans[pos];

        switch (currnode->kind) {
            case NODE_LITERAL: {
                if (pos > 0 && orphans[pos - 1]->kind == NODE_LITERAL) {
                    fprintf(stderr, "found invalid literal `%d` after `%d`\n!",
                            currnode->val.lit, orphans[pos - 1]->val.lit);
                    exit(EXIT_FAILURE);
                }

                if (Stack_is_empty(&roots)) {
                    Stack_push(&roots, Ast_Node_new());
                }

                if (Stack_top(&roots)->lchild != NULL) {
                    Stack_top(&roots)->lchild = currnode;
                    currnode->parent = Stack_top(&roots);
                } else if (Stack_top(&roots)->rchild) {
                    Stack_top(&roots)->rchild = currnode;
                    currnode->parent = Stack_top(&roots);
                }
            } break;
            case NODE_OPERATOR: {
                if (pos > 0 && orphans[pos - 1]->kind == NODE_OPERATOR) {
                    fprintf(stderr, "found invalid operator `%c` after `%c`\n",
                            currnode->val.op, orphans[pos - 1]->val.op);
                    exit(EXIT_FAILURE);
                }

                if (Stack_top(&roots)->lchild != NULL ||
                    Stack_top(&roots)->rchild != NULL) {
                    Stack_top(&roots)->kind = currnode->kind;
                    Stack_top(&roots)->val = currnode->val;
                    break;
                }

                if (OPORDER[Stack_top(&roots)->val.op] >=
                    OPORDER[currnode->val.op]) {
                    Ast_Node* oldroot = Stack_top(&roots);
                    Stack_push(&roots, Ast_Node_new());
                    Stack_top(&roots)->lchild = oldroot;
                    oldroot->parent = Stack_top(&roots);
                } else {
                    // yay tree reordering
                    Ast_Node* oldroot = Stack_pop(&roots);
                    Stack_push(&roots, Ast_Node_new());
                    Stack_top(&roots)->lchild = oldroot->rchild;
                    oldroot->rchild = Stack_top(&roots);
                    Stack_top(&roots)->parent = oldroot;
                    Stack_push(&roots, oldroot);
                }
            } break;
            case NODE_SEPARATOR: {
                fprintf(stderr, "brackets not implemented");
            } break;
        }
    }

    return Stack_top(&roots); // FIXME
}