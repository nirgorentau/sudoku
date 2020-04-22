#include "deterministic_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include "display_board.h"

/* finds the next empty cell in Board b, and puts the coordinates in ret[0], ret[1].
If there are none, puts N = (b->m)*(b->n) in ret[0].*/
void next_empty(Board* b, int i, int j, int* ret) {
    int N = get_N(b);
    int col, row;
    for (col = j+1; col < N; col++) {
        if (cell_at(b, i, col)->value == 0) {
            ret[0] = i;
            ret[1] = col;
            return;
        }
    }
    for (row = i+1; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (cell_at(b, row, col)->value == 0) {
                ret[0] = row;
                ret[1] = col;
                return;
            }
        }
    }
    /* If no empty cell found */
    ret[0] = N;
}

/* Returns the first valid value for cell (i,j) that is >= min.
Returns 0 if there is no such value */
int lowest_available_option(Board* b, int i, int j, int min) {
    int N = get_N(b), k;
    for (k = min; k <= N; k++) {
        if (is_value_valid_for_cell(b, i, j, k)) {
            return k;
        }
    }
    return 0;
}

/* For debugging only */
void print_stack(Stack* s) {
    s->content->curr = s->content->head;
    if (s->content->curr == NULL) {
        printf("EMPTY\n");
        return;
    }
    do {
        printf("(%d, %d) ", s->content->curr->m->i, s->content->curr->m->j);
    } while (move_forward(s->content) != -1);
    printf("END\n");
}

/* TODO: This does not iterate over boards properly - allows 1 empty cell at solution board.
I let this slide for now since it is still 0-1 solutions per board
but it might become an issue given more testing */
int solution_count(Board* b) {
    int count = 0;
    int N = get_N(b);
    int i=0, j=0;
    int k = 1; /* The value we check */
    int check_next[2] = {0, 0}; /* An array for lowest_available_option */
    int bin; /* for pop */
    Stack* s = new_stack();
    Board* temp_board = new_board(b->m, b->n);
    Cell* curr_cell;
    Move* tmp;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            temp_board->matrix[i][j].value = cell_at(b, i, j)->value;
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            curr_cell = cell_at(temp_board, i, j);
            if (curr_cell->value == 0) {
                check_next[0] = 1; /* To break from outer loop */
                break;
            }
        }
        if (check_next[0] == 1) {
            break;
        }
    }
    if (i == N) {
        /* TODO: Check if board is solved */
        return count;
    }
    /*
    After every iteration:
    (i, j) - the current cell
    top of the stack - last move performed.
    */
    do
    {
        if (k != 0) {
            tmp = new_move(1);
            set_move(tmp, 0, i, j, 0, k);
            push(s, tmp, 1);
            next_empty(temp_board, i, j, check_next);
            if(check_next[0] != N)
            {
                i = check_next[0];
                j = check_next[1];
            }
            else /* we have a valid option for the last cell */
            {
                count++;
                /* printf("Found Solution #%d for %dx%d\n", count, temp_board->m, temp_board->n); */
                tmp = pop(s, &bin);
                i = tmp->i;
                j = tmp->j;
                k = tmp->curr_value;
                free(tmp);
            }
        } else {
            tmp = pop(s, &bin);
            i = tmp->i;
            j = tmp->j;
            k = tmp->curr_value;
            cell_at(temp_board, i, j)->value = k;
            free(tmp);
        }
        curr_cell = cell_at(temp_board, i, j);
        k = lowest_available_option(temp_board, i, j, curr_cell->value+1);
        cell_at(temp_board, i, j)->value = k;
    } while (s->size != 0);
    free_board(temp_board);
    free_stack(s);
    return count;
}