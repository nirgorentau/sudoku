#include "user_io.h"
#include "display_board.h"
int main() {
    LinkedList* lst = NULL;
    Board* board = NULL;
    while(read_command(&board, &lst) != CMD_EXIT)
    {
    }
    return 0;
}