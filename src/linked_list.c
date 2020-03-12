#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

Node* new_list() {
    Node* head = (Node*) malloc(sizeof(Node));
    if (head == NULL) {
        printf("Memory allocation for list object failed\n");
        exit(-1);
    }
    head->next = head;
    head->prev = head;
    head->m = SENTINEL;
    return head;
}

Move* new_move(int i, int j, int prev_val, int curr_val) {
    Move* m = (Move*) malloc(sizeof(Move));
    if (m == NULL) {
        printf("Memory allocation for move object failed\n");
        exit(-1);
    }
    m->i = i;
    m->j = j;
    m->prev_value = prev_val;
    m->curr_falue = curr_val;
    return m;
}

int is_sentinel(Node* n) {
    return n->m == SENTINEL;
}

Node* get_head(Node* n) {
    Node* tmp = n;
    while (!is_sentinel(tmp->prev)) {
        tmp = tmp->prev;
    }
    return tmp;
}

Node* get_tail(Node* n) {
    Node* tmp = n;
    while (!is_sentinel(tmp->next)) {
        tmp = tmp->next;
    }
    return tmp;
}

void free_list(Node* n) {
    Node* head = get_head(n), *tmp;
    while (!is_sentinel(head)) {
        tmp = head->next;
        free(head->m);
        free(head);
        head = tmp;
    }
    free(head->m);
    free(head);
}

int length(Node* n) {
    int count = 0;
    Node* tmp = get_head(n);
    while (!is_sentinel(tmp)) {
        count++;
        tmp = tmp->next;
    }
    return count;
}

void append_next(Node* n, Move* value) {
    Node* new = (Node*) malloc(sizeof(Node));
    Node* tmp = n;
    if (new == NULL) {
        printf("Memory allocation for list object failed\n");
        exit(-1);
    }
    new->m = value;
    new->next = n->next;
    new->prev = n;
    n->next->prev = new;
    n->next = new;
}

void append_prev(Node* n, Move* value) {
    Node* new = (Node*) malloc(sizeof(Node));
    Node* tmp = n;
    if (new == NULL) {
        printf("Memory allocation for list object failed\n");
        exit(-1);
    }
    new->m = value;
    new->prev = n->prev;
    new->next = n;
    n->prev->next = new;
    n->prev = new;
}

Node* removeNode(Node* n) {
    Node* ret = n->next;
    if (n->m == SENTINEL) {
        return n;
    }
    n->next->prev = n->prev;
    n->prev->next = n->next;
    free(n->m);
    free(n);
    return ret;
}

