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
    s->content = new_list();
}

void free_stack(Stack* s) {
    free_list(s->content);
    free(s);
}

void push(Stack* s, Move* m) {
    append_prev(s->content, m);
    s->content->curr = s->content->head;
    s->size++;
}

Move* pop(Stack* s) {
    Move* ret;
    if (s->size == 0) {
        return NULL;
    }
    s->size--;
    ret = remove_curr(s->content);
    return ret;
} 