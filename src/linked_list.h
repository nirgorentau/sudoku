#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* The linked list is composed of nodes, each holding a value and pointers to the previous and next nodes.
The "head" and "tail" of the list are represented by a special node with m=SENTINEL,
whose next is the first item in the list and prev is the last. This special node is referred to as "sentinel" */

#define SENTINEL NULL

typedef struct move
{
    int i;  /* The coordinates of the cell */
    int j;
    int prev_value; /* The original value in the cell */
    int curr_falue; /* The new value in it */
} Move;


typedef struct node
{
    Move* m;
    struct node* prev;
    struct node* next;
} Node;

Node* new_list(); /* create a new linked list and returns a pointer to its head */
Move* new_move(int i, int j, int prev_val, int curr_val); /* creates a new element holding data of the move that was done */
Node* get_head(Node* n); /* returns the head of the list containing n */
Node* get_tail(Node* n); /* returns the tail of the list containing n */
int is_sentinel(Node* n); /* returns 1 if n is the sentinel, 0 otherwise */
void free_list(Node* n); /* frees a linked list */
int length(Node* n); /* returns number of nodes in list */
void append_next(Node* n, Move* value); /* adds a new node to the end of the list with the given value. */
void append_prev(Node* n, Move* value); /* adds a new node to the beginning of the list with the given value. Updates the head to point to the new node*/
Node* removeNode(Node* n); /* removes node n from the list and returns the pointer to the node after it. Does not remove the sentinel */


#endif