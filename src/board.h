#ifndef BOARD_H
#define BOARD_H

#define INIT_MODE 0
#define EDIT_MODE 1
#define SOLVE_MODE 2

typedef struct Cell
{
  int i;
  int j;
  int value;
  int fixed;
  int valid;
} Cell;

typedef struct Board
{
  Cell** matrix;
  int m; /* block height */
  int n; /* block width */
  int mode;
  int mark_errors;
} Board;

/* Return a pointer to a newly allocated board with blocks of size m*n
and all values initialized to 0 (valid field set to 1)*/
Board* new_board(int m, int n);

/* Free the board's allocated memory */
void free_board(Board* board);

/* Return the board size N = m*n */
int get_N(Board* board);

/* Return a pointer to the board cell in row i and column j */
Cell* cell_at(Board* board, int i, int j);

/* Return the index of the block containing 
the board cell in row i and column j */
int get_block_index(Board* board, int i, int j);

/* Return the a pointer to the cell_i'th cell in the block_i'th block */
Cell* cell_at_block(Board* board, int block_i, int cell_i);

/* Copy the contents of <from> to <to> */
int copy_board(Board* to, Board* from);

#endif