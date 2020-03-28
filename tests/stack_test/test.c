#include <stdio.h>
#include "stack.h"


int main() {
    Stack* s = new_stack();
    Move *a = new_move(1),
         *b = new_move(1),
         *c,
         *tmp;
    set_move(a, 0, 0, 0, 0, 1);
    set_move(b, 0, 1, 0, 0, 1);
    int i, bin;
    printf("items: %d\n", s->size);
    push(s, a, 1);
    printf("items: %d\n", s->size);
    push(s, b, 1);
    printf("items: %d\n", s->size);
    c = pop(s, &bin);
    printf("items: %d\n", s->size);
    if (c->i != 1) {
        printf("Pop failed: %d != 1\n", c->i);
    } else {
        printf("Pop succeeded\n");
    }
    c = pop(s, &bin);
    if (c->i != 0) {
        printf("Pop failed: %d != 0\n", c->i);
    } else {
        printf("Pop succeeded\n");
    }
    c = pop(s, &bin);
    if (c != NULL) {
        printf("Something went wrong: %d\n", c->i);
    } else {
        printf("Stack Empty\n");
    }
    for (i = 0; i < 16; i++) {
        tmp = new_move(1);
        set_move(tmp, 0, i, 0, 0, 1);
        push(s, tmp, 1);
    }
    for (i = 0; i < 16; i++)
    {
        c = pop(s, &bin);
        printf("(%d) ", c->i);
    }
    c = pop(s, &bin);
    if (c != NULL) {
        printf("Something went wrong: %d\n", c->i);
    } else {
        printf("Stack Empty\n");
    }
    
    free_stack(s);
    return 0;
}