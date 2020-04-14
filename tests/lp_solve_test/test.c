#include <stdio.h>
/* Sorry for the weird include - the makefile had to use the gurobi folder for includes */
#include "../../src/lp_solver.h"
#include "../../src/board.h"
#include "../../src/display_board.h"
#include "../../src/file_io.h"

int main() {
    Board* b = new_board(2, 3);
    Board* res = new_board(2, 3);
    Scores_matrix* mat_arr[9];
    Scores_matrix* m;
    int i, j, k;
    cell_at(b, 0, 0)->value = 1;
    cell_at(b, 0, 1)->value = 2;
    cell_at(b, 0, 2)->value = 3;
    cell_at(b, 1, 1)->value = 4;
    cell_at(b, 1, 2)->value = 5;
    cell_at(b, 1, 3)->value = 6;
    if (!integer_linear_solve(b, res)) {
        printf("Test error - solved unsolvable board:\n");
    }
    display_board(res);
    free_board(res);
    free_board(b);
    load_board(&b, "err_board.txt");
    for (i = 0; i < 9; i++) {
        mat_arr[i] = new_scores_matrix(9);
    }
    linear_solve(b, mat_arr, 9);
    for (k = 0; k < 9; k++) {
        m = mat_arr[k];
        printf("Scores for value %d:\n", k+1);
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 9; j++) {
                printf("%f ", score_at(m, i, j));
            }
            printf("\n");
        }
    }
    return 0;
}