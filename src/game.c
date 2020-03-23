#include "game.h"
#include "file_io.h"

int is_cell_value_valid(Board *b, int i, int j, Cell* cell) {
  Cell* temp;
  int k, block=get_block_index(b, i, j), N = get_N(b), val = cell->value;
  if (val == 0) return 1;
  for (k = 0; k < N; k++) {
    temp = cell_at(b, i, k);
    if(temp != cell && temp->value == val) return 0;
    temp = cell_at(b, k, j);
    if(temp != cell && temp->value == val) return 0;
    temp = cell_at_block(b, block, k);
    if(temp != cell && temp->value == val) return 0;
  }
  return 1;
}

static int set_valid_values(Board* board)
{
  int i, j, N;
  Cell* cell;
  N = get_N(board);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      cell = cell_at(board, i, j);
      cell->valid = is_cell_value_valid(board, i, j, cell);
    }
  }
  return 0;
}

static int is_board_legal(Board* board)
{
  int i, j, N;
  N = get_N(board);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if(cell_at(board, i, j)->valid == 0)
      {
        return 0;
      }
    }
  }
  return 1;
}

static int are_fixed_cells_legal(Board* board)
{
  int i, j, N, m, n;
  N = get_N(board);
  m = board->m;
  n = board->n;
  Board* temp;
  temp = new_board(m, n);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if(cell_at(board, i, j)->fixed == 1)
      {
        cell_at(temp, i, j)->value = cell_at(board, i, j)->value;
      }
    }
  }
  set_valid_values(temp);
  if(!is_board_legal(temp))
  {
    free_board(temp);
    return 0;
  }
  free_board(temp);
  return 1;
}

int solve(Board** board, char* filename, Node* lst)
{
  Board* temp;
  if(load_board(&temp, filename)!=0) return 1;
  if(are_fixed_cells_legal(temp) != 1) return 1;
  set_valid_values(temp);
  free_board(*board);
  *board = new_board(temp->m, temp->n);
  copy_board(*board, temp);
  (*board)->mode = SOLVE_MODE;
  /* TODO: mark_errors? 
  empty move list */
  free_board(temp);
  return 0;
}

int edit(Board** board, char* filename, Node* lst)
{
  Board* temp;
  if(load_board(&temp, filename)!=0) return 1;
  set_valid_values(temp);
  free_board(*board);
  *board = new_board(temp->m, temp->n);
  copy_board(*board, temp);
  (*board)->mode = EDIT_MODE;
  /* TODO: mark_errors? 
  empty move list */
  free_board(temp);
  return 0;
}

int save(Board* board, char* filename)
{
  if(board->mode == INIT_MODE)
  {
    /* command not available in init mode */
    return 1;
  }
  if(board->mode == SOLVE_MODE)
  {
    return save_board(board, filename);
  }
  else /* edit mode */
  {
    if(!is_board_legal)
    {
      return 1;
    }
    /* TODO: check if solvable
    mark all non-empty cells as fixed (maybe copy) */
    return save_board(board, filename);
  }
}

void mark_errors(Board* board, int val)
{
  board->mark_errors = val;
}

int set(Board* board, int x, int y, int z, Node* lst)
{
  Cell* cell;
  int i, j, val, N;
  N = get_N(board);
  if(x < 1 || x > N || y < 1 || y > N)
  {
  /* invalid cell cooridinates */
  return 1;
  }
  if(z < 0 || z > N)
  {
  /* invalid value */
  return 1;
  }
  i = y-1;
  j = x-1;
  cell = cell_at(board, i, j);
  if(board->mode == SOLVE_MODE)
  {
    if (cell->fixed == 1)
    {
      /* trying to set a fixed cell */ 
      return 1;
    }
  }
  cell->value = z;
  set_valid_values(board); /* can be made more efficient since only neighbors need to be updated */
  return 0;
}