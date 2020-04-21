#include <stdio.h>
/* Sorry for the weird include - the makefile had to use the gurobi folder for includes */
#include "../../src/lp_solver.h"
#include "../../src/board.h"
#include "../../src/display_board.h"
#include "../../src/file_io.h"

int main() {
    Board* b = new_board(2, 2);
    Board* res = new_board(2, 2);
    cell_at(b, 1, 1)->value = 4;
    if (!integer_linear_solve(b, res)) {
        printf("Test error - solved unsolvable board:\n");
    }
    display_board(res);
    free_board(res);
    free_board(b);
    return 0;
}