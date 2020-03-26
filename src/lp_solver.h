#ifndef LP_SOLVER_H
#define LP_SOLVER_H
#include "board.h"

/* Solve <board> using inetger LP, the result is stored in a newly allocated board pointed to by <*res>
Return 1 if the board is not solvable, 0 otherwise */
int integer_linear_solve(Board* board, Board** res);

/* Return 1 if the board is solvable, 0 otherwise */
int is_solvable(Board* board);

#endif