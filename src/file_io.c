#include "file_io.h"
#include <string.h>

static int load_failure(Board* board)
{
  free_board(board);
  return 1;
}

int save_board(Board* board, char* path)
{
  int i, j, N;
  FILE *f;
  N = get_N(board);
  f = fopen(path, "w");
  if (f == NULL) return 1;
  fprintf(f, "%d %d\n", board->m, board->n);
  for (i = 0; i < N; i++)
  {
    for ( j = 0; j < N; j++)
    {
      fprintf(f, "%d", cell_at(board, i, j)->value);
      if(board->mode == EDIT_MODE || cell_at(board, i, j)->fixed) fprintf(f, ".");
      if(j < N-1) fprintf(f, " ");
    }
    fprintf(f, "\n");
  }
  fclose(f);
  return 0;
}

int load_board(Board** board, char* path)
{
  Board* tboard;
  int i, j, m, n, N, v;
  char t;
  char* s;
  FILE *f;
  f = fopen(path, "r");
  if (f == NULL) return 1;
  if(fscanf(f, "%d", &m) == 0) return 1;
  if(fscanf(f, "%d", &n) == 0) return 1;
  tboard = new_board(m, n);
  N = get_N(tboard);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if(fscanf(f, "%d", &v)!=1) return load_failure(tboard);
      if(v >= N || v < 0) return load_failure(tboard);
      cell_at(tboard, i, j)->value = v;
      if(fscanf(f, "%c", &t)!=0)
      {
        if (t == '.') cell_at(tboard, i, j)->fixed = 1;
        else if(t != ' ' && t != '\n' && t != '\t') return load_failure(tboard);
      }
    }
  }
  if(fscanf(f, "%s", s) == 1) return load_failure(tboard); /* too many characters */
  fclose(f);
  *board = tboard;
  return 0;
}