#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

Stack* new_stack() {
    Stack* s = (Stack*) malloc(sizeof(Stack));
    if (s == NULL) {
        printf("Memory allocation for stack failed\n");
        exit(-1);
    }
    s->size = 0;
    s->top = new_list();
}

void free_stack(Stack* s) {
    free_list(s->top);
    free(s);
}

void push(Stack* s, Move* m) {
    append_prev(s->top, m);
    s->top = s->top->prev;
    s->size++;
}

Move* pop(Stack* s) {
    if (s->size == 0) {
        return NULL;
    }
    Move* ret = s->top->m;
    s->top = removeNode(s->top);
    s->size--;
    return ret;
} 