#pragma once

#include "ast.h"
#include <stddef.h>

typedef struct Queue_Elem {
    Ast_Literal value;
    struct Queue_Elem* next;
    struct Queue_Elem* prev;
} Queue_Elem;

typedef struct {
    Queue_Elem* front;
    Queue_Elem* back;
} Queue;

void Queue_init(Queue* q);
void Queue_deinit(Queue* q);
void Queue_enqueue(Queue* q, Ast_Literal value);
Ast_Literal Queue_dequeue(Queue* q);
