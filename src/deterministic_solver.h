#ifndef DET_SOLVER
#define DET_SOLVER

#include "stack.h"
#include "board.h"

int solution_count(Board* b); /* Returns the number of available solutions for given board */
/*TODO: Move to appropriate module*/int is_valid(Board *b, int i, int j, int val); /* returns 1 if putiing val in cell (i, j) is a valid move, 0 otherwise */

#endif