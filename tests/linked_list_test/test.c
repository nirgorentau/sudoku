#include <stdio.h>
#include "linked_list.h"

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
    head = get_head(head);
    printf("%d\n", head->m->i);
    head = removeNode(head);
    printf("%d\n", head->m->i);
    free_list(head);
    return 0;
}