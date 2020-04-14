#ifndef LP_SOLVER_H
#define LP_SOLVER_H
#include "board.h"


typedef struct scores_matrix
{
  double** matrix;
  int N;
} Scores_matrix;


/* Return a pointer to a newly allocated scores matrix with blocks of size N
and all values initialized to 0 */
Scores_matrix* new_scores_matrix(int N);

/* Free the matrix's allocated memory */
void free_scores_matrix(Scores_matrix* scores);

/* Return the matrix size N */
int get_scores_matrix_N(Scores_matrix* scores);

/* Return the score in row i and column j */
double score_at(Scores_matrix* scores_matrix, int i, int j);

/* Solve <board> using integer LP, the result is stored in a previously allocated board <res>
Return 1 if the board is not solvable, 0 otherwise
If Gurobi encounters an error during solution, returns -1 */
int integer_linear_solve(Board* board, Board* res);

/* Return 1 if the board is solvable, 0 otherwise */
int is_solvable(Board* board);

/* Solve <board> using LP and store the results in an array of Scores matrices
such that scores_matrices[k][i][j] is the score of setting board[i][j] value to k+1 */
int linear_solve(Board* board, Scores_matrix* scores_matrices, int N);

#endif