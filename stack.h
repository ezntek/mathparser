#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "ast.h"

typedef struct {
    Ast_Node** btm;
    Ast_Node** top;
    size_t cap;
} Stack;

void Stack_init(Stack* s);
void Stack_deinit(Stack* s);
bool Stack_is_empty(Stack* s);
void Stack_push(Stack* s, Ast_Node* newelem);
Ast_Node* Stack_pop(Stack* s);
Ast_Node* Stack_top(Stack* s);