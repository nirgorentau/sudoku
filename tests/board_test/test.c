#include <stdio.h>
#include <board.h>
int main()
{
  Board* b = new_board(3, 3);
  set_value(b, 6, 6, 1);
  printf("%d\n", get_value(b, 8, 8));
  printf("%d\n", get_block_value(b, 8, 0));
  printf("%d\n", get_block(b, 8, 8));
  return 0;
}
