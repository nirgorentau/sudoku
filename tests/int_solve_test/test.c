#include <stdio.h>
/* Sorry for the weird include - the makefile had to use the gurobi folder for includes */
#include "../../src/lp_solver.h"
#include "../../src/board.h"
#include "../../src/display_board.h"

int main() {
    Board* b = new_board(2, 3);
    Board* res = new_board(2, 3);
    cell_at(b, 0, 0)->value = 1;
    cell_at(b, 0, 1)->value = 2;
    cell_at(b, 0, 2)->value = 3;
    cell_at(b, 1, 1)->value = 4;
    cell_at(b, 1, 2)->value = 5;
    cell_at(b, 1, 3)->value = 6;
    if (!integer_linear_solve(b, &res)) {
        printf("Test error - solved unsolvable board:\n");
    }
    display_board(res);
    return 0;
}