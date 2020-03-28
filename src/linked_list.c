#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

Move* new_move(int i, int j, int prev_val, int curr_val) {
    Move* m = (Move*) malloc(sizeof(Move));
    if (m == NULL) {
        printf("Memory allocation for move object failed\n");
        exit(-1);
    }
    m->i = i;
    m->j = j;
    m->prev_value = prev_val;
    m->curr_value = curr_val;
    return m;
}
/*
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

void append_and_clean(Node* n, Move* m) {
    Node* tmp;
    append_next(n, m);
    n = n->next;
    while(n->m != SENTINEL) {
        n->prev->next = n->next;
        n->next->prev = n->prev;
        tmp = n->next;
        free_node(n);
        n = tmp;
    }
}
*/

LinkedList* new_list() {
    LinkedList* l = (LinkedList*) malloc(sizeof(LinkedList));
    if (l == NULL) {
        printf("Memory allocation for list failed\n");
        exit(-1);
    }
    l->head = NULL;
    l->curr = NULL;
    return l;
}

void free_list(LinkedList* l) {
    Node* curr = l->head;
    Node* tmp;
    while (curr != NULL) {
        tmp = curr->next;
        free(curr->m);
        free(curr);
        curr = tmp;
    }
    free(l);
}

int move_back(LinkedList* l) {
    if (l->curr == NULL) {
        return -1;
    }
    if (l->curr->prev == NULL) {
        return -1;
    }
    l->curr = l->curr->prev;
    return 0;
}

int move_forward(LinkedList* l) {
    if (l->curr == NULL) {
        return -1;
    }
    if (l->curr->next == NULL) {
        return -1;
    }
    l->curr = l->curr->next;
    return 0;
}

int length(LinkedList* l) {
    Node* tmp = l->curr;
    int count = 0;
    l->curr = l->head;
    if (l->head == NULL) {
        return 0;
    }
    do {
        count++;
    } while (move_forward(l) != -1);
    l->curr = tmp;
    return count;
}

void append_next(LinkedList* l, Move* value) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    Node* tmp, *prev;
    if (new_node == NULL) {
        printf("Memory allocation for list object failed\n");
        exit(-1);
    }
    new_node->m = value;
    new_node->next = NULL;
    new_node->prev = NULL;
    if (l->head == NULL) {
        l->head = new_node;
        l->curr = l->head;
        return;
    } else {
        new_node->prev = l->curr;
        if (l->curr->next != NULL) {
            l->curr->next->prev = new_node;
        }
        new_node->next = l->curr->next;
        l->curr->next = new_node;
        tmp = l->curr;
        l->curr = new_node;
        while (move_forward(l) != -1){
            remove_curr(l);
        }
        l->curr = tmp;
    }
}

void append_prev(LinkedList* l, Move* value) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation for list object failed\n");
        exit(-1);
    }
    new_node->m = value;
    new_node->next = NULL;
    new_node->prev = NULL;
    if (l->head == NULL) {
        l->head = new_node;
        l->curr = l->head;
        return;
    } else {
        new_node->next = l->curr;
        if (l->curr->prev == NULL) {
            /* Meaning the current node is the head */
            l->head = new_node;
            l->curr->prev = new_node;
        } else {
            l->curr->prev->next = new_node;
            new_node->prev = l->curr->prev;
            l->curr->prev = new_node;
        }
    }
}

Move* remove_curr(LinkedList* l){
    Move* value = new_move(l->curr->m->i, l->curr->m->j, l->curr->m->prev_value, l->curr->m->curr_value);
    Node* tmp;
    if (l->curr == NULL) {
        return NULL;
    }
    if (l->curr->prev == NULL) {
        l->head = l->curr->next;
        free(l->curr->m);
        free(l->curr);
        l->curr = l->head;
        l->curr->prev = NULL;
    } else if (l->curr->next == NULL) {
        l->curr->prev->next = NULL;
        tmp = l->curr->prev;
        free(l->curr->m);
        free(l->curr);
        l->curr = tmp;
    } else {
        l->curr->prev->next = l->curr->next;
        l->curr->next->prev = l->curr->prev;
        tmp = l->curr->prev;
        free(l->curr->m);
        free(l->curr);
        l->curr = tmp;
    }
    return value;
}