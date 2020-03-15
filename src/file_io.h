#ifndef FILEIO_H
#define FILEIO_H
#include <stdio.h>
#include <stdlib.h>
#include "board.h"

/* Save a board to <path> */
int save_board(Board* board, char* path);

/* Load a board from <path> (no validity check, only file format check).
   If the load is successful, <*board> is allocated new memory and the board
   is loaded into it */
int load_board(Board** board, char* path);

#endif