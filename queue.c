
#include "queue.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

static Queue_Elem* Queue_Elem_init(Ast_Literal value) {
    Queue_Elem* res = malloc(sizeof(Queue_Elem));

    if (res == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    res->value = value;
    res->next = res->prev = NULL;
    return res;
}

void Queue_init(Queue* q) { q->front = q->back = NULL; }

void Queue_deinit(Queue* q) {
    while (q->front != NULL)
        Queue_dequeue(q);
}

void Queue_enqueue(Queue* q, Ast_Literal value) {
    Queue_Elem* newelem = Queue_Elem_init(value);

    if (q->front == NULL && q->back == NULL) {
        q->front = q->back = newelem;
    } else {
        newelem->next = q->back;
        q->back->prev = newelem;
        q->back = newelem;
    }
}

Ast_Literal Queue_dequeue(Queue* q) {
    Ast_Literal res = q->front->value;
    Queue_Elem* prev = q->front->prev;

    if (q->front != NULL) {
        free(q->front);
        q->front = prev;
    }

    return res;
}