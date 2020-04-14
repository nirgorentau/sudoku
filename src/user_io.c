#include "user_io.h"

static int load_ints(int* target, int count) {
    int iter;
    char* tmp;
    for (iter = 0; iter < count; iter++) {
        tmp = strtok(NULL, " \t\r\n");
        if (tmp == NULL) {
            return CMD_ERR;
        }
        target[iter] = atoi(tmp);
    }
    return CMD_SUCCESS;
}

char* load_filename()
{
  return strtok(NULL, " \t\r\n");
}

int read_command(Board** board, LinkedList** lst)
{
    char line[256];
    char* cmd;
    int values[3]; /* Will conatin x, y, val in order */
    char* filename;
    if(fgets(line, 256, stdin) == NULL) return CMD_EXIT;
    cmd = strtok(line, " \t\r\n");
    if (cmd == NULL) 
    {
      return CMD_SUCCESS;
    }
    /* test for every valid command */
    if (strcmp(cmd, "solve") == 0)
    {
      filename = load_filename();
      solve(board, filename, lst);
    }
    if (strcmp(cmd, "edit") == 0)
    {
      filename = load_filename();
      edit(board, filename, lst);
    }
    if (strcmp(cmd, "save") == 0)
    {
      filename = load_filename();
      save(*board, filename);
    }
    if (strcmp(cmd, "mark_errors") == 0)
    {
      if (load_ints(values, 1) == CMD_ERR)
      {
        return CMD_ERR;
      }
      mark_errors(*board, values[0]);
    }
    else if (strcmp(cmd, "exit") == 0) 
    {
      exit_program(*board, *lst);
    } 
    else if (strcmp(cmd, "set") == 0) 
    {
      if (load_ints(values, 3) == CMD_ERR) 
      {
        return CMD_ERR;
      }
      set(*board, values[0], values[1], values[2], *lst);
    }
    else if (strcmp(cmd, "hint") == 0)
    {
      if (load_ints(values, 2) == CMD_ERR) 
      {
        return CMD_ERR;
      }
      hint(*board, values[0], values[1]);
    }
    else if (strcmp(cmd, "validate") == 0)
    {
      validate(*board);
    }
    else if (strcmp(cmd, "generate") == 0)
    {
      if (load_ints(values, 2) == CMD_ERR) 
      {
        return CMD_ERR;
      }
      generate(*board, values[0], values[1], *lst);
    }
    else if (strcmp(cmd, "num_solutions") == 0)
    {
      num_solutions(*board);
    }
    else if (strcmp(cmd, "autofill") == 0)
    {
      autofill(*board, *lst);
    }
    else if (strcmp(cmd, "undo") == 0)
    {
      undo(*board, *lst);
    }
    else if (strcmp(cmd, "redo") == 0)
    {
      redo(*board, *lst);
    }
    else if (strcmp(cmd, "reset") == 0)
    {
      reset(*board, *lst);
    }
    return CMD_SUCCESS;
}