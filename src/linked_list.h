#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* The linked list is composed of nodes, each holding a value and pointers to the previous and next nodes.
The end of the list is represented by a special node with m=SENTINEL,
whose next is the head of the list and prev is the tail. This special node is referred to as "sentinel" */

#define SENTINEL NULL

typedef struct move
{
    int i;  /* The coordinates of the cell */
    int j;
    int prev_value; /* The original value in the cell */
    int curr_value; /* The new value in it */
} Move;


typedef struct node
{
    Move* m;
    struct node* prev;
    struct node* next;
} Node;

typedef struct linked_list
{
    Node* head;
    Node* curr;
} LinkedList;

LinkedList* new_list(); /* create a new linked list and returns a pointer to its head */
Move* new_move(int i, int j, int prev_val, int curr_val); /* creates a new element holding data of the move that was done */
void free_list(LinkedList* l); /* frees a linked list */
int length(LinkedList* l); /* returns number of nodes in list */
int move_back(LinkedList* l); /* Moves l->curr one step backwards.
Returns 0 on success, else -1 */
int move_forward(LinkedList* l); /* Moves l->curr one step forward.
Returns 0 on success, else -1; */ 
void append_next(LinkedList* l, Move* value); /* Adds a new list object after l->curr and removes all nodes after it */
void append_prev(LinkedList* l, Move* value); /* Adds a new list object before l->curr */
Move* remove_curr(LinkedList* l); /* removes l->curr from the list and returns its value */

#endif