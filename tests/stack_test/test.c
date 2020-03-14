#include <stdio.h>
#include "stack.h"


int main() {
    Stack* s = new_stack();
    Move *a = new_move(0, 0, 0, 1),
         *b = new_move(1, 0, 0, 1),
         *c;
    printf("items: %d\n", s->size);
    push(s, a);
    printf("items: %d\n", s->size);
    push(s, b);
    printf("items: %d\n", s->size);
    c = pop(s);
    printf("items: %d\n", s->size);
    if (c->i != b->i) {
        printf("Pop failed: %d\n", c->i);
    } else {
        printf("Pop succeeded\n");
    }
    c = pop(s);
    if (c->i != a->i) {
        printf("Pop failed: %d\n", c->i);
    } else {
        printf("Pop succeeded\n");
    }
    c = pop(s);
    if (c != NULL) {
        printf("Something went wrong: %d\n", c->i);
    } else {
        printf("Test complete\n");
    }
    free_stack(s);
    return 0;
}