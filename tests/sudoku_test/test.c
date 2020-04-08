#include "user_io.h"
#include "display_board.h"
int main() {
    LinkedList* lst = NULL;
    Board* board = NULL;
    while(1)
    {
      read_command(&board, &lst);
      if(board != NULL) display_board(board);
    }
    return 0;
}