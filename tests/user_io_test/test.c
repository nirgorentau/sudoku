#include <stdio.h>
/* Sorry for the weird include - the makefile had to use the gurobi folder for includes */
#include "../../src/lp_solver.h"
#include "../../src/board.h"
#include "../../src/display_board.h"
#include "../../src/file_io.h"
#include "../../src/user_io.h"

int main() {
    Board* b = new_board(2, 3);
    LinkedList* undo_redo_list = new_list();
    int state = read_command(&b, &undo_redo_list);
    while (state != CMD_EXIT) {
        display_board(b);
        if (state == CMD_SUCCESS) {
            printf("SUCCESS\n");
        } else if (state == CMD_ERR) {
            printf("ERROR\n");
        } else if (state == CMD_RESTART) {
            printf("RESTART\n");
        } else {
            printf("Wrong state code: %d\n", state);
        }
        state = read_command(&b, &undo_redo_list);
    }
    printf("EXIT\n");
    return 0;
}