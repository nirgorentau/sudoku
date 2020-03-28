#include "game.h"
#include "file_io.h"
#include "lp_solver.h"
#include "deterministic_solver.h"
int is_cell_value_valid(Board *board, int i, int j, Cell* cell) {
  Cell* temp;
  int k, block=get_block_index(board, i, j), N = get_N(board), val = cell->value;
  if (val == 0) return 1;
  for (k = 0; k < N; k++) {
    temp = cell_at(board, i, k);
    if(temp != cell && temp->value == val) return 0;
    temp = cell_at(board, k, j);
    if(temp != cell && temp->value == val) return 0;
    temp = cell_at_block(board, block, k);
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

int solve(Board** board, char* filename, LinkedList* lst)
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

int edit(Board** board, char* filename, LinkedList* lst)
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
    if(!is_solvable(board))
    {
      /* board is not solveable */
      return 1; 
    }
    /* TODO: mark all non-empty cells as fixed (maybe copy) */
    return save_board(board, filename);
  }
}

void mark_errors(Board* board, int val)
{
  board->mark_errors = val;
}

int set(Board* board, int x, int y, int z, LinkedList* lst)
{
  Cell* cell;
  Move* move;
  int i, j, val, N, prev;
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
  prev = cell->value;
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
  move = new_move(1);
  set_move(move, 0, i, j, prev, z);
  append_next(lst, move, 1);
  return 0;
}

int validate(Board* board)
{
  if(!is_board_legal(board))
  {
    printf("The board is erroneous\n");
    return 1;
  }
  if(is_solvable(board))
  {
    printf("The board is solveable\n");
  }
  else
  {
    printf("The board is not solvable\n");
  }
  return 0;
}

static int count_empty_cells(Board* board)
{
  int s;
  int i, j, N;
  s = 0;
  N = get_N(board);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if(cell_at(board, i, j)->value == 0)
      {
        s++;
      }
    }
  }
  return s;
}

/* Fill <empty_cells> with pointers to the empty cells of the board */
static int get_empty_cells(Cell** empty_cells, Board* board)
{
  int s;
  int i, j, N;
  s = 0;
  N = get_N(board);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if(cell_at(board, i, j)->value == 0)
      {
        empty_cells[s] = cell_at(board, i, j);
        s++;
      }
    }
  }
}

/* Return a pointer to a random cell from the first N-i cells in <cells> and switch its position with cells[N-i-1] to avoid repetitions */
Cell* get_random_cell(Cell** cells, int N, int i)
{
  Cell* temp;
  int j = rand()%(N-i);
  temp = cells[j];
  cells[j] = cells[N-i-1];
  cells[N-i-1] = temp;
  return temp;
}

/* Return a random valid value i (such that values[i] == 0) or 0 if there is none */
int get_random_valid_value(int* values, int N)
{
  int i, j, s, res;
  s = 0;
  for ( i = 0; i < N; i++)
  {
    if(values[i] == 0) s++;
  }
  if(s == 0) return 0;
  int* temp = malloc(sizeof(int) * s);
  if(temp == NULL)
  {
    printf("Memory allocation failed\n");
    exit(-1);
  }
  j = 0;
  for ( i = 0; i < N; i++)
  {
    if(values[i] == 0) temp[j++]=i+1;
  }
  res = temp[rand()%s];
  free(temp);
  return res;
}

/* Set <values>[i] to 1 if value i isn't valid for <cell>> */
int set_invalid_values_for_cell(Board* board, Cell* cell, int* values)
{
  int i, j;
  Cell* temp;
  int N = get_N(board);
  int k, block=get_block_index(board, i, j);
  for (k = 0; k < N; k++)
  {
    values[k] = 0;
  }
  
  /* mark invalid values */
  for (k = 0; k < N; k++) {
    temp = cell_at(board, i, k);
    if(temp->value != 0) values[(temp->value)-1] = 1;
    temp = cell_at(board, k, j);
    if(temp->value != 0) values[(temp->value)-1] = 1;
    temp = cell_at_block(board, block, k);
    if(temp->value != 0) values[(temp->value)-1] = 1;
  }
  return 0;
}

int generate(Board* board, int x, int y, LinkedList* lst)
{
  int i, j, k, n, m, N, empty_cells_count, val, failed;
  Board* temp;
  Board* solved_board;
  Cell** cells;
  Cell* cell;
  int* values;
  if(!(board->mode == EDIT_MODE)) return 1;
  if(!is_board_legal(board)) return 1;
  n = board->n;
  m = board->m;
  N = get_N(board);
  empty_cells_count = count_empty_cells(board);
  if(empty_cells_count < x) return 1;
  if(N*N - empty_cells_count + x < y) return 1;
  temp = new_board(m, n);
  cells = malloc(sizeof(Cell*)*empty_cells_count);
  if(cells == NULL)
  {
    printf("Memory allocation failed\n");
    exit(-1);
  }
  values = malloc(sizeof(int)*N);
  if(values == NULL)
  {
    printf("Memory allocation failed\n");
    exit(-1);
  }
  for (k = 0; k < 1000; k++)
  {
    failed = 0;
    copy_board(temp, board);
    get_empty_cells(cells, temp);
    for (i = 0; i < x; i++)
    {
      cell = get_random_cell(cells, empty_cells_count, i);
      values = set_invalid_values_for_cell(temp, cell, values);
      val = get_random_valid_value(values, N);
      if(val == 0)
      {
        failed = 1;
        break;
      } 
      else cell->value = val;
    }
    if(failed) continue; /* Try again in the next iteration */

    if(integer_linear_solve(temp, &solved_board)) continue;
    else break;
  }
  free(values);
  free_board(temp);
  free(cells);
  cells = malloc(sizeof(Cell*) * N*N);
  if(cells == NULL)
  {
    printf("Memory allocation failed\n");
    exit(-1);
  }
  for ( i = 0; i < N; i++)
  {
    for ( j = 0; j < N; j++)
    {
      cells[i*N + j] = cell_at(solved_board, i, j);
    }
  }

  for ( i = 0; i < y; i++)
  {
    cell = get_random_cell(cells, N, i);
    cell->value = 0;
  }
  /*TODO: fix all cells? 
  update lst*/
  free(cells);
  copy_board(board, solved_board);
  free(solved_board);
  return 0;
}

int hint(Board* board, int x, int y)
{
  Board* solved_board;
  int i, j;
  i = y-1;
  j = x-1;
  if(!(board->mode == SOLVE_MODE)) return 1; //not in solve mode
  if(!is_board_legal(board)) return 1; //erroneous
  if(cell_at(board,i, j)->fixed) return 1; //cell is fixed
  if(cell_at(board, i, j)->value != 0) return 1; //cell contains a value already
  if(integer_linear_solve(board, &solved_board) == 1) return 1; //not solvable
  printf("%d\n", cell_at(solved_board, i, j)->value);
  free_board(solved_board);
  return 0;
}

int num_solutions(Board* board)
{
  if(board->mode == INIT_MODE) return 1; //invalid mode
  if(!(is_board_legal(board))) return 1; //erroneous board
  printf("%d\n", solution_count(board));
  return 0;
}

/* Return an obvious value for the cell if there is one, or 0 otherwise */
static int obvious_value(Board* board, int i, int j)
{ 
  int N;
  int* values;
  int res;
  int k;
  Cell* temp;
  if(cell_at(board, i, j)->value != 0) return 0;
  N = get_N(board);
  values = malloc(sizeof(int)*N);
  if(values == NULL)
  {
    printf("Memory allocation failed\n");
    exit(-1);
  }
  set_invalid_values_for_cell(board, cell_at(board, i, j), values);
  /* check only one value is still available (0) and set res to it*/
  res = 0;
  for (k = 0; k < N; k++)
  {
    if(values[k] == 0)
    {
      if (res == 0) res = k+1;
      else
      {
        res = 0;
        break;
      }
      
    }
  }
  free(values);
  return res;
}

int autofill(Board* board, LinkedList* lst)
{
  Board* temp;
  int i, j, N;
  N = get_N(board);
  temp = new_board(board->m, board->n);
  copy_board(temp, board);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if(obvious_value(temp, i, j))
      {
        cell_at(board, i, j)->value = obvious_value(temp, i, j);
        /*TODO: update move list */
      }
    }
  }
  free_board(temp);
  return 0;
}

int exit_program(Board* board, LinkedList* lst)
{
  free_board(board);
  free_list(lst);
  printf("Exiting...\n");
  exit(0);
}