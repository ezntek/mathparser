#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexer.h"

size_t gen_orphans(Ast_Node*** res, const Token* toks, size_t ntoks) {
    Ast_Node** nodes;
    size_t nodes_len = 0;
    size_t nodes_cap = 5;

    if ((nodes = malloc(sizeof(Ast_Node*) * nodes_cap)) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; toks[i].kind != TOKEN_EOF; i++) {
        if (nodes_len + 1 > nodes_cap) {
            nodes = realloc(nodes, sizeof(Ast_Node*) * (nodes_cap *= 5));

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
    Ast_Node* curr_node;
    Ast_Node* curr_root = Ast_Node_new();
    size_t pos = 0;

    while (pos < norphans) {
        curr_node = orphans[pos];

        switch (curr_node->kind) {
            case NODE_LITERAL: {
                if (pos > 0 && orphans[pos - 1]->kind == NODE_LITERAL) {
                    fprintf(stderr, "found invalid literal `%d` after `%d`\n!",
                            curr_node->val.lit, orphans[pos - 1]->val.lit);
                    exit(EXIT_FAILURE);
                }

                if (curr_root->lchild == NULL) {
                    curr_root->lchild = curr_node;
                    curr_node->parent = curr_node;
                } else if (curr_root->rchild == NULL) {
                    curr_root->rchild = curr_node;
                    curr_node->parent = curr_root;
                }
            } break;
            case NODE_OPERATOR: {
                if (pos > 0 && orphans[pos - 1]->kind == NODE_OPERATOR) {
                    fprintf(stderr, "found invalid operator `%c` after `%c`\n",
                            curr_node->val.op, orphans[pos - 1]->val.op);
                    exit(EXIT_FAILURE);
                }

                if (curr_root->lchild == NULL || curr_root->rchild == NULL) {
                    curr_root->kind = curr_node->kind;
                    curr_root->val = curr_node->val;
                    break;
                }

                if (OPORDER[curr_root->val.op] < OPORDER[curr_node->val.op]) {
                    Ast_Node* old_rchild = curr_root->rchild;
                    curr_root->rchild = Ast_Node_new();
                    curr_root = curr_root->rchild;
                    curr_root->kind = curr_node->kind;
                    curr_root->val = curr_node->val;

                    curr_root->lchild = old_rchild;
                    curr_root->parent = old_rchild->parent;
                    old_rchild->parent = curr_root;
                } else {
                    while (curr_root->parent != NULL)
                        curr_root = curr_root->parent;

                    Ast_Node* new_parent = Ast_Node_new();
                    Ast_Node* old_root = curr_root;
                    curr_root->parent = new_parent;
                    curr_root = curr_root->parent;
                    curr_root->kind = curr_node->kind;
                    curr_root->val = curr_node->val;
                    curr_root->lchild = old_root;
                }
            } break;
            case NODE_SEPARATOR: {
                fprintf(stderr, "brackets not implemented\n");
            } break;
        }

        pos++;
    }

    while (curr_root->parent != NULL)
        curr_root = curr_root->parent;

    return curr_root;
}

void sprint_ast(Ast_Node* root, char* restrict buf) {
    char left[250];
    char right[250];
    char* node_kind_s;

    if (root->lchild != NULL) {
        sprint_ast(root->lchild, left);
    } else {
        strcpy(left, "<null>");
    }

    if (root->rchild != NULL) {
        sprint_ast(root->rchild, right);
    } else {
        strcpy(right, "<null>");
    }

    switch (root->kind) {
        case NODE_LITERAL: {
            snprintf(buf, 350, // NOLINT
                     "{%d, left%s, right%s}", root->val.lit, left, right);
        } break;
        case NODE_OPERATOR: {
            snprintf(buf, 350, // NOLINT
                     "{%c, left%s, right%s}", root->val.sep, left, right);
        } break;
        case NODE_SEPARATOR: {
            sprintf(buf, "not implemented"); // NOLINT
        } break;
    }
}

void print_ast(Ast_Node* root) {
    char buf[1000];
    sprint_ast(root, buf);
    printf("%s\n", buf);
}