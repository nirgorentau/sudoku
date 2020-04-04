#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* The linked list is composed of nodes, each holding a value and pointers to the previous and next nodes. 
Some lists can be made with a special head Node whose value is NULL */

#define HEAD_NODE NULL

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
    int move_count; /* The number of moves in the node */
} Node;

typedef struct linked_list
{
    Node* head;
    Node* curr;
} LinkedList;

LinkedList* new_list(); /* create a new linked list and returns a pointer to its head */
Move* new_move(int count); /* creates an array of empty moves */
LinkedList* new_head(); /* Creates a new list and sets its head to be a special head Node */
void set_move(Move* m, int index, int i, int j, int prev_val, int curr_val); /* Sets a given move's data */
void free_list(LinkedList* l); /* frees a linked list */
int length(LinkedList* l); /* returns number of nodes in list */
int move_back(LinkedList* l); /* Moves l->curr one step backwards.
Returns 0 on success, else -1 */
int move_forward(LinkedList* l); /* Moves l->curr one step forward.
Returns 0 on success, else -1; */ 
void append_next(LinkedList* l, Move* value, int length); /* Adds a new list object after l->curr and removes all nodes after it */
void append_prev(LinkedList* l, Move* value, int length); /* Adds a new list object before l->curr */
Move* remove_curr(LinkedList* l, int* count); /* removes l->curr from the list and returns its value, and puts the number of moves in it in count */

#endif