#include <stdio.h>
#include "linked_list.h"

/*
int main() {
    Node* head = new_list();
    Move *a = new_move(0, 0, 0, 1),
         *b = new_move(1, 0, 0, 1),
         *c = new_move(2, 0, 0, 1);
    printf("New list created successfully\n");
    append_next(head, a);
    printf("Appended a\n");
    append_next(head->next, b);
    printf("Appended b\n");
    append_prev(head, c);
    printf("Appended c\n");
    printf("length: %d\n", length(head));
    head = get_head(head);
    do {
        if (head->m == SENTINEL) {
            printf("S ");
        } else {
            printf("%d ", head->m->i);
        }
        head = head->next;
    } while (!is_sentinel(head));
    printf("\n");
    head = get_tail(head);
    do {
        if (head->m == SENTINEL) {
            printf("S ");
        } else {
            printf("%d ", head->m->i);
        }
        head = head->prev;
    } while (!is_sentinel(head));
    printf("\n");
    head = get_head(head);
    printf("%d\n", head->m->i);
    head = removeNode(head);
    printf("%d\n", head->m->i);
    free_list(head);
    return 0;
}
*/

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
            printf("%d ", l->curr->m->i);
        } while (move_forward(l) != -1);
        printf("\n");
        l->curr = tmp;
    }
    printf("Length: %d\n", length(l));
}

void main() {
    LinkedList* l = new_list();
    Move *a = new_move(0, 0, 0, 1),
         *b = new_move(1, 0, 0, 1),
         *c = new_move(2, 0, 0, 1),
         *d = new_move(3, 0, 0, 1);
    printf("New list created successfully\n");
    printlist(l);
    append_next(l, a);
    printf("Appended a\n");
    printlist(l);
    move_forward(l);
    append_next(l, b);
    printf("Appended b\n");
    printlist(l);
    append_prev(l, c);
    printf("Appended c\n");
    printlist(l);
    l->curr = l->head;
    append_next(l, d);
    printf("Appended d\n");
    printlist(l);
}