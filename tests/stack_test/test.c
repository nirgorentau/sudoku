#include <stdio.h>
#include "stack.h"


int main() {
    Stack* s = new_stack();
    Move *a = new_move(0, 0, 0, 1),
         *b = new_move(1, 0, 0, 1),
         *c;
    int i;
    printf("items: %d\n", s->size);
    push(s, a);
    printf("items: %d\n", s->size);
    push(s, b);
    printf("items: %d\n", s->size);
    c = pop(s);
    printf("items: %d\n", s->size);
    if (c->i != 1) {
        printf("Pop failed: %d != 1\n", c->i);
    } else {
        printf("Pop succeeded\n");
    }
    c = pop(s);
    if (c->i != 0) {
        printf("Pop failed: %d != 0\n", c->i);
    } else {
        printf("Pop succeeded\n");
    }
    c = pop(s);
    if (c != NULL) {
        printf("Something went wrong: %d\n", c->i);
    } else {
        printf("Stack Empty\n");
    }
    for (i = 0; i < 16; i++) {
        push(s, new_move(i, 0, 0, 1));
    }
    for (i = 0; i < 16; i++)
    {
        c = pop(s);
        printf("(%d) ", c->i);
    }
    c = pop(s);
    if (c != NULL) {
        printf("Something went wrong: %d\n", c->i);
    } else {
        printf("Stack Empty\n");
    }
    
    free_stack(s);
    return 0;
}