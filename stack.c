#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "stack.h"

void Stack_init(Stack* s) {
    s->cap = 10;
    s->top = s->btm = calloc(s->cap, sizeof(Ast_Node*));
}

void Stack_deinit(Stack* s) {
    while (s->top != s->btm)
        Stack_pop(s);
}

bool Stack_is_empty(Stack* s) { return (s->top == s->btm); }

void Stack_push(Stack* s, Ast_Node* newelem) {
    if (s->top == s->btm + s->cap) {
        s->btm = realloc(s->btm, s->cap *= 3);

        if (s->btm == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        s->top = s->btm + s->cap;
    }

    *s->top = newelem;
    s->top++;
}

Ast_Node* Stack_pop(Stack* s) {
    if (s->btm == s->top) {
        fputs("Stack Underflow", stderr);
        return NULL;
    }

    s->top--;
    return *s->top;
}

Ast_Node* Stack_top(Stack* s) { return (*s->top); }