#include "user_io.h"
int main() {
    LinkedList* lst = NULL;
    Board* board = new_board(1, 1);
    board->mode = INIT_MODE;
    printf("Input command: ");
    while(read_command(&board, &lst) != CMD_EXIT)
    {
        printf("Input command: ");
    }
    return 0;
}