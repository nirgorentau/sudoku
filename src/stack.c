#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

Stack* new_stack() {
    Stack* s = (Stack*) malloc(sizeof(Stack));
    if (s == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    s->size = 0;
    s->content = new_list();
    return s;
}

void free_stack(Stack* s) {
    free_list(s->content);
    free(s);
}

void push(Stack* s, Move* m, int count) {
    append_prev(s->content, m, count);
    s->content->curr = s->content->head;
    s->size++;
}

Move* pop(Stack* s, int* count) {
    Move* ret;
    if (s->size == 0) {
        return NULL;
    }
    s->size--;
    ret = remove_curr(s->content, count);
    return ret;
} 