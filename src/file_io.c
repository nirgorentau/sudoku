#include "file_io.h"

int save_board(Board* board, char* path)
{
  int i, j, N;
  N = get_N(board);
  FILE *f;
  f = fopen(path, "w");
  if (f == NULL) return 1;
  fprintf(f, "%d %d\n", board->m, board->n);
  for (i = 0; i < N; i++)
  {
    for ( j = 0; j < N; j++)
    {
      fprintf(f, "%d", cell_at(board, i, j)->value);
      if(cell_at(board, i, j)->fixed || board->mode == EDIT_MODE) fprintf(f, ".");
      fprintf(f, " ");
    }
    fprintf(f, "\n");
  }
  fclose(f);
  
}