#include "display_board.h"

void print_seperator_row(Board* board)
{
  int i, size;
  size = 4*get_N(board) + board->m + 1;
  for (i = 0; i < size; i++)
  {
    printf("-");
  }
  printf("\n");
}

void print_cell(Board* board, int i, int j)
{
  int val;
  Cell* cell;
  printf(" ");
  cell = cell_at(board, i, j);
  val = cell->value;
  if(val != 0) printf("%2d", val);
  else printf("  ");
  if(board->mode == SOLVE_MODE && cell->fixed) printf(".");
  else
  {
    /*if erroneous and board is in edit more or mark errors set to 1 */
    if(!cell->valid && (board->mode == EDIT_MODE || board->mark_errors == 1)) printf("*");
    else printf(" ");
  }
}

void display_board(Board* board)
{
  int i, j, N;
  N = get_N(board);
  for (i = 0; i < N; i++)
  {
    if(i%(board->m) == 0) print_seperator_row(board);
    for ( j = 0; j < N; j++)
    {
      if(j%(board->n) == 0) printf("|");
      print_cell(board, i, j);
    }
    printf("|\n");
  }
  print_seperator_row(board);
  
}