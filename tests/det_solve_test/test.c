#include "deterministic_solver.h"
#include <stdio.h>

/* TODO: Generate more test cases */
void main() {
    Board* b = new_board(2, 2);
    int count = solution_count(b);
    if (count == 288) {
        printf("Success for 2x2\n");
    } else {
        printf("Failure for 2x2: %d/288\n", count);
    }
    free_board(b);
}