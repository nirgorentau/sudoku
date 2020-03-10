#include <stdlib.h>
#include "board.h"

static void block_indices_to_global_indices(int m, int n, int i, int j, int* l, int* k)
{
  *l = (i / m) * m;
  *k = (i % m) * n;
  *l += j / n;
  *k += j % n;
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
    }
  }
  return board;
}

void free_board(Board* board)
{
  int i, N;
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

int get_value(Board* board, int i, int j)
{
  return (board->matrix)[i][j].value;
}

void set_value(Board* board, int i, int j, int val)
{
  board->matrix[i][j].value = val;
}

int get_fixed(Board* board, int i, int j)
{
  return board->matrix[i][j].fixed;
}

void set_fixed(Board* board, int i, int j, int val)
{
  board->matrix[i][j].fixed = val;
}

int get_block(Board* board, int i, int j)
{
  int l, k;
  l = i / board->m;
  k = j / board->n;
  return l * board->m + k;
}

int get_block_value(Board* board, int i, int j)
{
  int l, k;
  block_indices_to_global_indices(board->m, board->n, i, j, &l, &k);
  return board->matrix[l][k].value;
}

void set_block_value(Board* board, int i, int j, int val)
{
  int l, k;
  block_indices_to_global_indices(board->m, board->n, i, j, &l, &k);
  board->matrix[l][k].value = val;
}

int get_block_fixed(Board* board, int i, int j)
{
  int l, k;
  block_indices_to_global_indices(board->m, board->n, i, j, &l, &k);
  return board->matrix[l][k].fixed;
}

void set_block_fixed(Board* board, int i, int j, int val)
{
  int l, k;
  block_indices_to_global_indices(board->m, board->n, i, j, &l, &k);
  board->matrix[l][k].fixed = val;
}