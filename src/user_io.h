#ifndef USER_IO_H
#define USER_IO_H

#define CMD_SUCCESS 0
#define CMD_ERR -1
#define CMD_EXIT 1
#define CMD_RESTART 2
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int read_command(Board** board, LinkedList** lst);

#endif