#include <stdio.h>
#include <board.h>
#include <file_io.h>
#include <display_board.h>
int main()
{
  Board* t;
  Board* b = new_board(3, 3);
  cell_at(b, 6, 6)->value = 1;
  cell_at(b, 6, 6)->fixed = 1;
  printf("%d\n", cell_at(b, 8, 8)->value);
  printf("%d\n", cell_at_block(b, 8, 0)->value);
  printf("%d\n", cell_at_block(b, 8, 8)->value);
  display_board(b);
  save_board(b, "save_file.txt");
  if(load_board(&t, "load_file.txt") == 0)
  {
    puts("load successful");
    display_board(t);
    free_board(t);
  }
  else puts("load failed");
  free_board(b);
  return 0;
}
