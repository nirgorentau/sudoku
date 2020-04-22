#include "game.h"
#include "file_io.h"
#include "lp_solver.h"
#include "deterministic_solver.h"
#include "display_board.h"


/* Check whether the value of the cell is valid */
static int is_cell_value_valid(Board *board, Cell* cell) {
  Cell* temp;
  int i, j, N, val;
  int k, block;
  i = cell->i;
  j = cell->j;
  block=get_block_index(board, i, j);
  N = get_N(board);
  val = cell->value;
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

/* Set the valid field of the cells in the board */
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
      cell->valid = is_cell_value_valid(board, cell);
    }
  }
  return 0;
}

/* Check that the board is not erroneous */
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

/* Set the valid field of the fixed cells in the board, ignoring non-fixed cells */
static int are_fixed_cells_legal(Board* board)
{
  int i, j, N, m, n;
  Board* temp;
  N = get_N(board);
  m = board->m;
  n = board->n;
  /* Copy the fixed cells to a temporary board */
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


/* Count the number of empty cells in the board */
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
  return s;
}

/* Return a pointer to a random cell from the first N-i cells in <cells> and switch its position with cells[N-i-1] to avoid repetitions */
static Cell* get_random_cell(Cell** cells, int N, int i)
{
  Cell* temp;
  int j = rand()%(N-i);
  temp = cells[j];
  cells[j] = cells[N-i-1];
  cells[N-i-1] = temp;
  return temp;
}

/* Return a random valid value i (such that values[i] == 0) or 0 if there is none */
static int get_random_valid_value(int* values, int N)
{
  int i, j, s, res;
  int* temp;
  s = 0;
  for ( i = 0; i < N; i++)
  {
    if(values[i] == 0) s++;
  }
  if(s == 0) return 0;
  temp = malloc(sizeof(int) * s);
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

/* Set <values>[i] to 1 if value i isn't valid for <cell> */
static int set_invalid_values_for_cell(Board* board, Cell* cell, int* values)
{
  int i, j, k, block;
  Cell* temp;
  int N = get_N(board);
  i = cell->i;
  j = cell->j;
  block=get_block_index(board, i, j);
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

/* Allocate and set a new move array based on the differences between the boards to <*moves> and return the length of the array */
static int get_moves(Board* old_board, Board* new_board, Move** moves)
{
  int i, j, N, counter, k;
  N = get_N(old_board);
  counter = 0;
  for ( i = 0; i < N; i++)
  {
    for ( j = 0; j < N; j++)
    {
      if(cell_at(old_board, i, j)->value != cell_at(new_board, i, j)->value) counter++;
    }
  }
  if(counter) *moves = new_move(counter);
  k = 0;
  for ( i = 0; i < N; i++)
  {
    for ( j = 0; j < N; j++)
    {
      if(cell_at(old_board, i, j)->value != cell_at(new_board, i, j)->value)
      {
        set_move(*moves, k, i, j, cell_at(old_board, i, j)->value, cell_at(new_board, i, j)->value);
        k++;
      }
    }
  }
  return counter;
  
}

/* Return an obvious value for the cell if there is one, or 0 otherwise */
static int obvious_value(Board* board, int i, int j)
{ 
  int N;
  int* values;
  int res;
  int k;
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


/* Allocate an array of N scores matrices of size N*N */
static Scores_matrix** new_scores_matrices(int N)
{
  Scores_matrix** scores_matrices;
  int i;
  scores_matrices = malloc(sizeof(Scores_matrix*) * N);
  if(scores_matrices == NULL)
  {
    printf("Memory allocation failed\n");
    exit(-1);
  }
  for (i = 0; i < N; i++)
  {
    scores_matrices[i] = new_scores_matrix(N);
  }
  return scores_matrices;
}

/* Free an array of scores matrices */
static void free_scores_matrices(Scores_matrix** scores_matrices, int N)
{
  int i;
  for (i = 0; i < N; i++)
  {
    free_scores_matrix(scores_matrices[i]);
  }
  free(scores_matrices);
}

int is_board_full(Board* board)
{
  int i, j, N;
  N = get_N(board);
  for ( i = 0; i < N; i++)
  {
    for ( j = 0; j < N; j++)
    {
      if(cell_at(board, i, j)->value == 0) return 0;
    }
  }
  return 1;
}

/* Choose a random value from <values> with probability proportional to their score */
static int choose_random_value(int* values, double* scores, double sum, int size)
{
  int i;
  double random_double;
  random_double = ((double)(rand())/RAND_MAX) * sum;
  for ( i = 0; i < size; i++)
  {
    if(random_double < scores[i]) return values[i];
  }

  return 0;
}

int is_board_solved(Board* board)
{
  return (is_board_full(board) && is_board_legal(board));
}

int solve(Board** board, char* filename, LinkedList** lst)
{
  Board* temp;
  if(load_board(&temp, filename)!=0) 
  {
    printf("Error loading file %s\n", filename);
    return 1;
  }
  if(are_fixed_cells_legal(temp) != 1)
  {
    printf("Invalid file format: %s\n", filename);
    return 1;
  }
  set_valid_values(temp);
  free_board(*board);
  *board = new_board(temp->m, temp->n);
  copy_board(*board, temp);
  (*board)->mode = SOLVE_MODE;
  free_list(*lst);
  *lst = new_head();
  free_board(temp);
  return 0;
}

int edit_default(Board** board, LinkedList** lst)
{
  free_board(*board);
  *board = new_board(3, 3);
  (*board)->mode = EDIT_MODE;
  free_list(*lst);
  *lst = new_head();
  return 0;
}

int edit(Board** board, char* filename, LinkedList** lst)
{
  Board* temp;
  if (filename == NULL) {
    return edit_default(board, lst);
  }
  if(load_board(&temp, filename)!=0)
  {
    printf("Error loading file %s\n", filename);
    return 1;
  }
  set_valid_values(temp);
  free_board(*board);
  *board = new_board(temp->m, temp->n);
  copy_board(*board, temp);
  (*board)->mode = EDIT_MODE;
  free_list(*lst);
  *lst = new_head();
  free_board(temp);
  return 0;
}

int save(Board* board, char* filename)
{
  if(board->mode == INIT_MODE)
  {
    printf("Command not only available in edit mode or solve mode\n"); 
    return 1;
  }
  if(board->mode == SOLVE_MODE)
  {
    return save_board(board, filename);
  }
  else /* edit mode */
  {
    if(!is_board_legal(board))
    {
      printf("The board is erroneous\n");
      return 1;
    }
    if(!is_solvable(board))
    {
      printf("The board is not solveable\n");
      return 1; 
    }
    return save_board(board, filename);
  }
}

void mark_errors(Board* board, int val)
{
  if (board->mode != SOLVE_MODE) {
    printf("Command only available in solve mode\n"); 
  } else {
    board->mark_errors = val;
  }
}

int set(Board* board, int x, int y, int z, LinkedList* lst)
{
  Cell* cell;
  Move* move;
  int i, j, N, prev;
  if (board->mode == INIT_MODE) {
    printf("Command not available in init mode\n"); 
    return 1;
  }
  N = get_N(board);
  if(x < 1 || x > N || y < 1 || y > N)
  {
  /* invalid cell cooridinates */
  printf("Invalid parameters: Coordinates out of range (1-%d)\n", N);
  return 1;
  }
  if(z < 0 || z > N)
  {
  /* invalid value */
  printf("Invalid parameters: Cell value out of range (0-%d)\n", N);
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
      printf("Trying to set a fixed cell in solve mode\n"); 
      return 1;
    }
  }
  cell->value = z;
  set_valid_values(board);
  move = new_move(1);
  set_move(move, 0, i, j, prev, z);
  append_next(lst, move, 1);
  return 0;
}

int validate(Board* board)
{
  if (board->mode == INIT_MODE) {
    printf("Command not available in init mode\n"); 
    return 1;
  }
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

int generate(Board* board, int x, int y, LinkedList* lst)
{
  int i, j, k, n, m, N, empty_cells_count, val, failed;
  Board* temp;
  Board* solved_board;
  Cell** cells;
  Cell* cell;
  int* values;
  Move* moves;
  if(!(board->mode == EDIT_MODE)) 
  {
    printf("Command only available in edit mode\n");
    return 1;
  }
  n = board->n;
  m = board->m;
  N = get_N(board);
  empty_cells_count = count_empty_cells(board);
  if(empty_cells_count < x) 
  {
    printf("X is too large, not enough empty cells to fill with random values\n");
    return 1;
  }
  if(N*N < y)
  {
    printf("Y can't be larger than the total amount cells in the board\n");
    return 1;
  } 
  if(!is_board_legal(board))
  {
    printf("The board is erroneous\n");
    return 1;
  } 
  temp = new_board(m, n);
  solved_board = new_board(m, n);
  solved_board->mode = EDIT_MODE;
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
    for (i = 0; i < x; i++) /* Add random (valid) values for x random cells */
    {
      cell = get_random_cell(cells, empty_cells_count, i);
      set_invalid_values_for_cell(temp, cell, values); /* Set the valid values for the cell to choose from */
      val = get_random_valid_value(values, N);
      if(val == 0) /* There is no valid value for the cell */
      {
        failed = 1;
        break;
      } 
      else cell->value = val;
    }
    if(failed) continue; /* Couldn't generate a valid board, Try again in the next iteration */
    if(integer_linear_solve(temp, solved_board) != 0) continue; /* The generated board isn't solvable, try again */
    else break;
  }
  free(values);
  free_board(temp);
  free(cells);
  if(k == 1000) /* Failed after 1000 tries */
  {
    printf("Failed to generate board\n");
    free_board(solved_board);
    return 1;
  }
  cells = malloc(sizeof(Cell*) * N*N);
  if(cells == NULL)
  {
    printf("Memory allocation failed\n");
    exit(-1);
  }
  /* Fill the cells array with pointers to the cells of the board */
  for ( i = 0; i < N; i++)
  {
    for ( j = 0; j < N; j++)
    {
      cells[i*N + j] = cell_at(solved_board, i, j);
    }
  }

  for ( i = 0; i < N*N - y; i++)
  {
    cell = get_random_cell(cells, N*N, i); /* Choose a random cell (no repetitions) */
    cell->value = 0;
  }
  i = get_moves(board, solved_board, &moves);
  if(i != 0) append_next(lst, moves, i);
  free(cells);
  copy_board(board, solved_board);
  set_valid_values(board);
  free_board(solved_board);
  return 0;
}

int undo(Board* board, LinkedList* lst)
{
  Move* moves;
  int k, count;
  if(board->mode == INIT_MODE)
  {
    printf("Command not only available in edit mode or solve mode\n"); 
    return 1;
  }
  count = lst->curr->move_count;
  if (board->mode == INIT_MODE) {
    printf("Command not available in init mode\n"); 
    return 1;
  }
  if(count == 0) 
  {
    printf("No moves to undo\n");
    return -1;
  }
  moves = lst->curr->m;
  for (k = count-1; k >= 0; k--)
  {
    cell_at(board, moves[k].i, moves[k].j)->value = moves[k].prev_value;
    printf("Changed cell <%d,%d> from %d to %d\n", moves[k].j+1, moves[k].i+1, moves[k].curr_value, moves[k].prev_value);
  }
  set_valid_values(board);
  move_back(lst);
  return 0;
}

int redo(Board* board, LinkedList* lst)
{
  Move* moves;
  int k, count;
  if (board->mode == INIT_MODE) {
    printf("Command not available in init mode\n"); 
    return 1;
  }
  if(move_forward(lst) == -1)
  {
    printf("No moves to redo\n");
    return -1;
  }
  count = lst->curr->move_count;
  moves = lst->curr->m;
  for (k = 0; k < count; k++)
  {
    cell_at(board, moves[k].i, moves[k].j)->value = moves[k].curr_value;
    printf("Changed cell <%d,%d> from %d to %d\n",  moves[k].j+1, moves[k].i+1, moves[k].prev_value, moves[k].curr_value);
  }
  set_valid_values(board);
  return 0;
}

int reset(Board* board, LinkedList* lst)
{
  Move* moves;
  int k, count;
  if (board->mode == INIT_MODE) {
    printf("Command not available in init mode\n"); 
    return 1;
  }
  count = lst->curr->move_count;
  if(board->mode == INIT_MODE)
  {
    printf("Command not only available in edit mode or solve mode\n"); 
    return 1;
  }
  while(count != 0) 
  {
    moves = lst->curr->m;
    for (k = count-1; k >= 0; k--)
    {
      cell_at(board, moves[k].i, moves[k].j)->value = moves[k].prev_value;
    }
    move_back(lst);
    count = lst->curr->move_count;
  }
  set_valid_values(board);
  return 0;
}

int hint(Board* board, int x, int y)
{
  Board* solved_board;
  int i, j, N;
  int res;
  if(!(board->mode == SOLVE_MODE)) 
  {
    printf("Command only available in solve mode\n");
    return 1;
  }
  N = get_N(board);
  if(x < 1 || x > N || y < 1 || y > N)
  {
  /* invalid cell cooridinates */
  printf("Invalid parameters: Coordinates out of range (1-%d)\n", N);
  return 1;
  }
  i = y-1;
  j = x-1;
  if(!is_board_legal(board))
  {
    printf("The board is erroneous");
    return 1;
  } 

  if(cell_at(board, i, j)->fixed)
  {
    printf("The cell is fixed\n");
    return 1;
  } 
  if(cell_at(board, i, j)->value != 0)
  {
    printf("The cell already contains a value\n");
    return 1;
  } 
  solved_board = new_board(board->m, board->n);
  res = integer_linear_solve(board, solved_board);
  if(res == 0) printf("hint: %d\n", cell_at(solved_board, i, j)->value);
  else printf("The board is not solvable\n");
  free_board(solved_board);
  return res;
}

int num_solutions(Board* board)
{
  if (board->mode == INIT_MODE) /* invalid mode */
  {
    printf("Command not available in init mode\n"); 
    return 1;
  }
  if(!is_board_legal(board)) /* erroneous board */
  {
    printf("The board is erroneous");
    return 1;
  }
  printf("Number of solutions: %d\n", solution_count(board));
  return 0;
}

int autofill(Board* board, LinkedList* lst)
{
  Board* temp;
  int i, j, N, counter, k;
  Move* moves;
  if(board->mode != SOLVE_MODE)
  {
    printf("Command only available in solve mode\n");
    return 1;
  }
  if(!is_board_legal(board))
  {
    printf("The board is erroneous\n");
    return 1;
  }
  N = get_N(board);
  temp = new_board(board->m, board->n);
  copy_board(temp, board);
  counter = 0;
  /* Count how many cells will be filled */
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if(obvious_value(temp, i, j))
      {
        counter++;
      }
    }
  }
  if(counter)
  {
    /* Fill the cells with their forced value */
    moves = new_move(counter);
    k = 0;
    for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
      {
        if(obvious_value(temp, i, j))
        {
          set_move(moves, k, i, j, cell_at(board, i, j)->value, obvious_value(temp, i, j));
          cell_at(board, i, j)->value = obvious_value(temp, i, j);
          k++;
        }
      }
    }
    append_next(lst, moves, counter);
  }
  free_board(temp);
  set_valid_values(board);
  return 0;
}

int guess_hint(Board* board, int x, int y)
{
  int i, j, k, N;
  Board* temp;
  Scores_matrix** scores_matrices;
    N = get_N(board);
  if(board->mode != SOLVE_MODE)
  {
    printf("Command only available in solve mode\n");
    return 1;
  }
  if(x < 1 || x > N || y < 1 || y > N)
  {
    /* invalid cell cooridinates */
    printf("Invalid parameters: Coordinates out of range (1-%d)\n", N);
    return 1;
  }
  i = y-1;
  j = x-1;
  if(!is_board_legal(board))
  {
    printf("The board is erroneous\n");
    return 1;
  }
  if(cell_at(board, i, j)->fixed)
  {
    printf("The cell is fixed\n");
    return 1;
  }
  if(cell_at(board, i, j)->value != 0)
  {
    printf("The cell already contains a value\n");
    return 1;
  }

  temp = new_board(board->m, board->n);
  copy_board(temp, board);
  scores_matrices = new_scores_matrices(N); /* The values of the i, j k'th score matrix represents the score
  of the value k for the cell in the i'th row and j'th column */
  linear_solve(temp, scores_matrices, N);
  for (k = 0; k < N; k++)
  {
    if(score_at(scores_matrices[k], i, j) > 0.0)
    {
      printf("Score for value %d: %f\n", k+1, score_at(scores_matrices[k], i, j));
    }
  }
  free_board(temp);
  free_scores_matrices(scores_matrices, N);
  return 0;
}

int guess(Board* board, float x, LinkedList* lst)
{
  int i, j, k, l, N;
  double* scores_vector;
  double scores_sum;
  int* values_vector;
  Board* temp;
  Move* moves;
  int move_count;
  Cell* cell;
  int candidate;
  Scores_matrix** scores_matrices;
  if (board->mode != SOLVE_MODE) {
    printf("Command only available in solve mode\n"); 
    return 1;
  }
  if(x < 0.0 || x > 1.0)
  {
    printf("Invalid parameter: Threshold out of range (0.0-1.0)\n");
    return 1;
  }
  N = get_N(board);
  if(!is_board_legal(board))
  {
    printf("The board is erroneous\n");
    return 1;
  }
  temp = new_board(board->m, board->n);
  copy_board(temp, board);
  scores_matrices = new_scores_matrices(N); /* Would store the scores for the different values for the cell */
  linear_solve(temp, scores_matrices, N);
  for ( i = 0; i < N; i++)
  {
    for ( j = 0; j < N; j++)
    {
      l = 0;
      scores_sum = 0.0;
      scores_vector = calloc(N, sizeof(double));
      values_vector = calloc(N, sizeof(int));
      if(scores_vector == NULL || values_vector == NULL)
      {
        printf("Memory allocation failed\n");
        return 1;
      }
      /* fill the values and scores vector with values that have a score greater than x */
      for ( k = 0; k < N; k++)
      {
        if(score_at(scores_matrices[k], i, j) > x)
        {
            values_vector[l] = k+1;
            scores_sum += score_at(scores_matrices[k], i, j);
            scores_vector[l] = scores_sum;
            l++;
        }
      }
      if(l>0)
      {
        /* Choose a random value (distribution proportional to the score) and the the cell to that value, if it is legal */
        candidate = choose_random_value(values_vector, scores_vector, scores_sum, l);
        cell = cell_at(board, i, j);
        if(is_value_valid_for_cell(board, i, j, candidate)) cell->value = candidate;
      }
      free(scores_vector);
      free(values_vector);
    }
    
  }
  set_valid_values(board);
  move_count = get_moves(temp, board, &moves);
  if(move_count)
  {
    append_next(lst, moves, move_count);
  }
  free_board(temp);
  free_scores_matrices(scores_matrices, N);
  return 0;
}

int exit_program(Board* board, LinkedList* lst)
{
  free_board(board);
  free_list(lst);
  printf("Exiting...\n");
  return 0;
}
