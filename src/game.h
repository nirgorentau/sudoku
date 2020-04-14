#ifndef GAME_H
#define GAME_H

#include "linked_list.h"
#include "board.h"

/*Check whether the value of the cell is valid*/
int is_cell_value_valid(Board *b, Cell* cell);

/*Start a puzzle in solve mode*/
int solve(Board** board, char* filename, LinkedList** lst);

/*Start a puzzle in edit mode*/
int edit(Board** board, char* filename, LinkedList** lst);

/*Save the board to a file*/
int save(Board* board, char* filename);

/*Set the mark errors value of the board to val*/
void mark_errors(Board* board, int val);

/*Set the cell in column x and row y (1-based) to the value of z*/
int set(Board* board, int x, int y, int z, LinkedList* lst);

/*Validate the board and print whether or not it is solvable*/
int validate(Board* board);

/*Guess a solution to the board using LP with threshhold x*/
int guess(Board* board, float x, LinkedList* lst);

/*Generate a puzzle by filling x empty cells with random valid values,
solving the board using ILP and clearing all but y random cells*/
int generate(Board* board, int x, int y, LinkedList* lst);

/*Undo a previous move*/
int undo(Board* board, LinkedList* lst);

/*Redo a previously undone move*/
int redo(Board* board, LinkedList* lst);

/*Show the solution of cell <x,y> */
int hint(Board* board, int x, int y);

/*Show a guess of the solution of cell <x,y> */
int guess_hint(Board* board, int x, int y);

/*Print the number of solutions for the board*/
int num_solutions(Board* board);

/*Fill obvious values (single legal values) */
int autofill(Board* board, LinkedList* lst);

/*Undo all moves*/
int reset(Board* board, LinkedList* lst);

/*Terminate the program*/
int exit_program(Board* board, LinkedList* lst);

#endif