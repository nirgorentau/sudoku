#include <stdio.h>
#include <board.h>
#include <file_io.h>
#include <display_board.h>
int main()
{
  Board* b = new_board(3, 3);
  cell_at(b, 6, 6)->value = 1;
  printf("%d\n", cell_at(b, 8, 8)->value);
  printf("%d\n", cell_at_block(b, 8, 0)->value);
  printf("%d\n", cell_at_block(b, 8, 8)->value);
  display_board(b);
  save_board(b, "save_file.txt");
  free_board(b);
  return 0;
}
