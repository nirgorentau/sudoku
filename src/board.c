#include <stdlib.h>
#include "board.h"

static void block_indices_to_global_indices(int m, int n, int block_i, int cell_i, int* l, int* k)
{
  *l = (block_i / m) * m;
  *k = (block_i % m) * n;
  *l += cell_i / n;
  *k += cell_i % n;
}

Board* new_board(int m, int n)
{
  int i, j, N;
  Board* board; 
  board = malloc(sizeof(Board));
  N = m*n;
  board->m = m;
  board->n = n;
  board->mode = 0;
  board->mark_errors = 0;
  board->matrix = malloc(sizeof(Cell*) * N);
  for (i = 0; i < N; i++)
  {
    board->matrix[i] = malloc(sizeof(Cell) * N);
    for (j = 0; j < N; j++)
    {
      board->matrix[i][j].value = 0;
      board->matrix[i][j].fixed = 0;
      board->matrix[i][j].valid = 1;
    }
  }
  return board;
}

void free_board(Board* board)
{
  int i, N;
  if(board == NULL) return;
  N = get_N(board);
  for (i = 0; i < N; i++)
  {
    free(board->matrix[i]);
  }
  free(board->matrix);
  free(board);
}

int get_N(Board* board)
{
  return board->m * board->n;
}

Cell* cell_at(Board* board, int i, int j)
{
  return &(board->matrix[i][j]);
}

int get_block_index(Board* board, int i, int j)
{
  int l, k;
  l = i / board->m;
  k = j / board->n;
  return l * board->m + k;
}

Cell* cell_at_block(Board* board, int block_i, int cell_i)
{
  int l, k;
  block_indices_to_global_indices(board->m, board->n, block_i, cell_i, &l, &k);
  return &(board->matrix[l][k]);
}

/* Copy the contents of <from> to <to> */
int copy_board(Board* to, Board* from)
{
  int i, j, N;
  N = get_N(to);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      cell_at(to, i, j)->value = cell_at(from, i, j)->value;
      cell_at(to, i, j)->fixed = cell_at(from, i, j)->fixed;
      cell_at(to, i, j)->valid = cell_at(from, i, j)->valid;
    }
  }
  to->mode = from->mode;
  to->mark_errors = from->mark_errors;
  return 0;
    
}