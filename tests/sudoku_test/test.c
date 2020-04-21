#include "user_io.h"
#include "display_board.h"
int main() {
    LinkedList* lst = NULL;
    Board* board = NULL;
    while(read_command(&board, &lst) != CMD_EXIT)
    {
      if(board != NULL) display_board(board);
    }
    exit_program(board, lst);
    return 0;
}