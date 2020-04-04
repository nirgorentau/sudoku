#include <stdio.h>
#include "linked_list.h"


void printlist(LinkedList* l) {
    Node* tmp = l->curr;
    l->curr = l->head;
    if (l->head == NULL) {
        printf("EMPTY\n");
        return;
    } else {
        do {
            if (l->head == l->curr) {
                printf("H");
            }
            if (l->curr == tmp) {
                printf("C");
            }
            printf("%d ", l->curr->m[0].i);
        } while (move_forward(l) != -1);
        printf("\n");
        l->curr = tmp;
    }
    printf("Length: %d\n", length(l));
}

void main() {
    LinkedList* l = new_list();
    Move *a = new_move(1),
         *b = new_move(1),
         *c = new_move(1),
         *d = new_move(1);
    int bin = 0;
    set_move(a, 0, 0, 0, 0, 1);
    set_move(b, 0, 1, 0, 0, 1);
    set_move(c, 0, 2, 0, 0, 1);
    set_move(d, 0, 3, 0, 0, 1);
    printf("New list created successfully\n");
    printlist(l);
    append_next(l, a, 1);
    printf("Appended a\n");
    printlist(l);
    move_forward(l);
    append_next(l, b, 1);
    printf("Appended b\n");
    printlist(l);
    append_prev(l, c, 1);
    printf("Appended c\n");
    printlist(l);
    l->curr = l->head;
    append_next(l, d, 1);
    printf("Appended d\n");
    printlist(l);
    free_list(l);
    l = new_head();
    printf("Created new list\n");
    free_list(l);
    l = new_head();
    printf("Created new list\n");
    remove_curr(l, &bin);
    printf("removed first item\n");
    free_list(l);
}