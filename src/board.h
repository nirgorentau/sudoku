#ifndef BOARD
#define BOARD
typedef struct Cell
{
  int value;
  int fixed;
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
and all values initialized to 0 */
Board* new_board(int m, int n);

/* Free the board's allocated memory */
void free_board(Board* board);

/* Return the board size N = m*n */
int get_N(Board* board);

/* Return the value of the board cell in row i and column j */
int get_value(Board* board, int i, int j);

/* Set the value of the board cell in row i and column j */
void set_value(Board* board, int i, int j, int val);

/* Return whether the board cell in row i and column j is fixed (1) or not (0) */
int get_fixed(Board* board, int i, int j);

/* Set whether the board cell in row i and column j is fixed (1) or not (0) */
void set_fixed(Board* board, int i, int j, int val);

/* Return the index of the block containing 
the board cell in row i and column j */
int get_block(Board* board, int i, int j);

/* Return the value of the cell_i'th cell in the block_i'th block */
int get_block_value(Board* board, int block_i, int cell_i);

/* Set the value of the cell_i'th cell in the block_i'th block */
void set_block_value(Board* board, int block_i, int cell_i, int val);

/* Return whether the cell_i'th cell in the block_i'th block is fixed (1) or not (0) */
int get_block_fixed(Board* board, int block_i, int cell_i);

/* Set whether the cell_i'th cell in the block_i'th block is fixed (1) or not (0) */
void set_block_fixed(Board* board, int block_i, int cell_i, int val);
#endif