#ifndef STACK_H
#define STACK_H


#include "linked_list.h"

/* Implementation of stack using linked-list.
   Outside modification of struct member "size" may cause the stack to not fuction properly, and it is advised to avoid doing so.*/

typedef struct stack
{
    LinkedList* content;
    int size;
} Stack;

Stack* new_stack(); /* creates a new stack and returns a pointer to it */
void free_stack(Stack* s); /* Frees a stack's memory */
void push(Stack* s, Move* m); /* pushes a move to the stack */
Move* pop(Stack* s); /* pops a move from the stack. if stack is empty returns NULL */


#endif