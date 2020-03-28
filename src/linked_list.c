#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

Move* new_move(int count) {
    Move* m = (Move*) malloc(count*sizeof(Move));
    int i;
    if (m == NULL) {
        printf("Memory allocation for move object failed\n");
        exit(-1);
    }
    for(i = 0; i < count; i++) {
        m[i].i = 0;
        m[i].j = 0;
        m[i].prev_value = 0;
        m[i].curr_value = 0;
    }
    return m;
}

void set_move(Move* m, int index,  int i, int j, int prev_val, int curr_val) {
    m[index].i = i;
    m[index].j = j;
    m[index].prev_value = prev_val;
    m[index].curr_value = curr_val;
}

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

void append_next(LinkedList* l, Move* value, int move_count) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    Node* tmp, *prev;
    int bin; /* To give to remove_curr */
    if (new_node == NULL) {
        printf("Memory allocation for list object failed\n");
        exit(-1);
    }
    new_node->m = value;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->move_count = move_count;
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
            remove_curr(l, &bin);
        }
        l->curr = tmp;
    }
}

void append_prev(LinkedList* l, Move* value, int move_count) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation for list object failed\n");
        exit(-1);
    }
    new_node->m = value;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->move_count = move_count;
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

Move* remove_curr(LinkedList* l, int* count){
    int i;
    Move* value;
    Node* tmp;
    if (l->curr == NULL) {
        *count = 0;
        return NULL;
    }
    *count = l->curr->move_count;
    value = new_move(l->curr->move_count);
    for(i = 0; i < l->curr->move_count; i++) {
        set_move(value, i, l->curr->m[i].i, l->curr->m[i].j, l->curr->m[i].prev_value, l->curr->m[i].curr_value);
    }
    if (l->curr->prev == NULL) {
        l->head = l->curr->next;
        free(l->curr->m);
        free(l->curr);
        l->curr = l->head;
        if (l->curr != NULL) {
            l->curr->prev = NULL;
        }
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