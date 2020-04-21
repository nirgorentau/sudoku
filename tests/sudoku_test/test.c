#include "user_io.h"
int main() {
    LinkedList* lst = NULL;
    Board* board = new_board(1, 1);
    board->mode = INIT_MODE;
    while(read_command(&board, &lst) != CMD_EXIT)
    {
    }
    return 0;
}