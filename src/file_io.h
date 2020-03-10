#ifndef FILEIO_H
#define FILEIO_H
#include <stdio.h>
#include <stdlib.h>
#include "board.h"

/* Save a board to <path> */
int save_board(Board* board, char* path);

/* load a board from <path> */
int load_board(Board* board, char* path);

#endif