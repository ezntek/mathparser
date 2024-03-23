#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "stack.h"

void Stack_init(Stack* s) {
    s->cap = 10;
    s->btm = calloc(s->cap, sizeof(Ast_Node*));
    s->top = NULL;

    if (s->btm == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
}

void Stack_deinit(Stack* s) {
    while (s->top != s->btm)
        Stack_pop(s);
}

bool Stack_is_empty(Stack* s) { return (s->top == NULL); }

void Stack_push(Stack* s, Ast_Node* newelem) {
    if (s->top == s->btm + s->cap) {
        s->btm = realloc(s->btm, s->cap *= 3);

        if (s->btm == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        s->top = s->btm + s->cap;
    }

    if (s->top == NULL) {
        s->top = s->btm;
        *s->top = newelem;
    } else {
        *s->top = newelem;
        s->top++;
    }
}

Ast_Node* Stack_pop(Stack* s) {
    Ast_Node* res;

    if (s->top == NULL) {
        fputs("Stack Underflow", stderr);
        exit(EXIT_FAILURE);
    }

    if (s->top == s->btm) {
        res = *s->top;
        s->top = NULL;
    } else {
        s->top--;
        res = *s->top;
    }

    return res;
}

Ast_Node* Stack_top(Stack* s) {
    if (s->top == NULL) {
        fputs("Stack is empty", stderr);
        exit(EXIT_FAILURE);
    }

    return *s->top;
}