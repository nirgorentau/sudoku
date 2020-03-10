#include <stdio.h>
#include <board.h>
int main()
{
  Board* b = new_board(3, 3);
  printf("%d\n", get_value(b, 0, 0));
  return 0;
}
