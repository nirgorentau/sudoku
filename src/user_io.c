#include "user_io.h"

static int load_ints(int* target, int count) {
    int iter;
    char* tmp;
    for (iter = 0; iter < count; iter++) {
        tmp = strtok(NULL, " \t\r\n");
        if (tmp == NULL) {
            printf("Missing parameters: Expected %d, got %d\n", count, iter);
            return CMD_ERR;
        }
        if ((strcmp(tmp, "0") != 0) && (atoi(tmp) == 0)) {
          printf("Invalid parameter: '%s' is not an integer\n", tmp);
          return CMD_ERR;
        }
        target[iter] = atoi(tmp);
    }
    /* Checking to see we don't have too many parameters */
    tmp = strtok(NULL, " \t\r\n");
        if (tmp != NULL) {
          printf("Too many parameters: command accepts only %d parameter(s)\n", count);
          return CMD_ERR;
        }
    return CMD_SUCCESS;
}

static int load_float(float* target)
{
  char* tmp;
  tmp = strtok(NULL, " \t\r\n");
  if(tmp == NULL)
  {
    printf("Missing parameters: Expected 1, got 0\n");
    return CMD_ERR;
  }
  if (atof(tmp) == 0)
  {
    printf("Invalid parameter: '%s' is not a number\n", tmp);
    return CMD_ERR;
  }
  *target = atof(tmp);
  /* Checking to see we have only 1 parameter */
  tmp = strtok(NULL, " \t\r\n");
  if(tmp != NULL)
  {
    printf("Too many parameters: command accepts only 1 parameter\n");
    return CMD_ERR;
  }
  return CMD_SUCCESS;
}

char* load_filename()
{
  return strtok(NULL, " \t\r\n");
}

int read_command(Board** board, LinkedList** lst)
{
    char line[258]; /* 256 characters + '\0' termination +1 */
    char* cmd;
    float x;
    int values[3]; /* Will conatin x, y, val in order */
    char* filename;
    if(fgets(line, 258, stdin) == NULL) return CMD_EXIT;
    cmd = strtok(line, " \t\r\n");
    if (cmd == NULL) 
    {
      return CMD_SUCCESS;
    }
    /* test that line length is less than 256: */
    if (strlen(line) > 256)
    {
      while (fgetc(stdin) != '\n') {} /* Ignore rest of line */
      printf("Error: input too long. inputh should be 256 characters or less\n");
      return CMD_ERR;
    }
    /* test for every valid command */
    if (strcmp(cmd, "solve") == 0)
    {
      filename = load_filename();
      if (filename == NULL)
      {
        printf("Missing parameters: Expected 1, got 0\n");
        printf("Correct usage: solve <filename>\n");
        return CMD_ERR;
      }
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts only 1 parameter(s)\n");
        printf("Correct usage: solve <filename>\n");
        return CMD_ERR;
      }
      if (!solve(board, filename, lst)) {
        display_board(*board);
      }
    }
    else if (strcmp(cmd, "edit") == 0)
    {
      filename = load_filename();
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts only 0-1 parameter(s)\n");
        printf("Correct usage: edit or edit <filename>\n");
        return CMD_ERR;
      }
      if (!edit(board, filename, lst))
      {
        display_board(*board);
      }
    }
    else if (strcmp(cmd, "save") == 0)
    {
      filename = load_filename();
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: save <filename>\n");
        return CMD_ERR;
      }
      save(*board, filename);
    }
    else if (strcmp(cmd, "mark_errors") == 0)
    {
      if (load_ints(values, 1) == CMD_ERR)
      {
        printf("Correct usage: mark_errors [0/1]\n");
        return CMD_ERR;
      }
      mark_errors(*board, values[0]);
    }
    else if (strcmp(cmd, "exit") == 0) 
    {
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: exit\n");
        return CMD_ERR;
      }
      exit_program(*board, *lst);
      return CMD_EXIT;
    } 
    else if (strcmp(cmd, "set") == 0) 
    {
      if (load_ints(values, 3) == CMD_ERR)
      {
        printf("Correct usage: set [1-%d] [1-%d] [0-%d]\n", get_N(*board), get_N(*board), get_N(*board));
        return CMD_ERR;
      }
      if (!set(*board, values[0], values[1], values[2], *lst))
      {
        display_board(*board);
      }
    }
    else if (strcmp(cmd, "hint") == 0)
    {
      if (load_ints(values, 2) == CMD_ERR)
      {
        printf("Correct usage: hint [1-%d] [1-%d]\n", get_N(*board), get_N(*board));
        return CMD_ERR;
      }
      hint(*board, values[0], values[1]);
    }
    else if (strcmp(cmd, "guess_hint") == 0)
    {
      if (load_ints(values, 2) == CMD_ERR)
      {
        printf("Correct usage: guess_hint [1-%d] [1-%d]\n", get_N(*board), get_N(*board));
        return CMD_ERR;
      }
      if (guess_hint(*board, values[0], values[1])) return CMD_ERR;
    }
    else if (strcmp(cmd, "validate") == 0)
    {
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: validate\n");
        return CMD_ERR;
      }
      validate(*board);
    }
    else if (strcmp(cmd, "generate") == 0)
    {
      if (load_ints(values, 2) == CMD_ERR)
      {
        printf("Correct usage: generate [0-%d] [1-%d]\n", get_N(*board)*get_N(*board), get_N(*board)*get_N(*board));
        return CMD_ERR;
      }
      if (!generate(*board, values[0], values[1], *lst))
      {
        display_board(*board);
      }
    }
    else if (strcmp(cmd, "num_solutions") == 0)
    {
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: num_solutions\n");
        return CMD_ERR;
      }
      num_solutions(*board);
    }
    else if (strcmp(cmd, "autofill") == 0)
    {
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: autofill\n");
        return CMD_ERR;
      }
      if (!autofill(*board, *lst))
      {
        display_board(*board);
      }
    }
    else if(strcmp(cmd, "guess") == 0)
    {
      if(load_float(&x) == CMD_ERR)
      {
        printf("Correct usage: guess [0.0-1.0]\n");
        return CMD_ERR;
      }
      if (!guess(*board, x, *lst))
      {
        display_board(*board);
      }
    }
    else if (strcmp(cmd, "undo") == 0)
    {
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: undo\n");
        return CMD_ERR;
      }
      if (!undo(*board, *lst))
      {
        display_board(*board);
      }
    }
    else if (strcmp(cmd, "redo") == 0)
    {
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: redo\n");
        return CMD_ERR;
      }
      if (!redo(*board, *lst))
      {
        display_board(*board);
      }
    } 
    else if (strcmp(cmd, "reset") == 0)
    {
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: reset\n");
        return CMD_ERR;
      }
      if (!reset(*board, *lst))
      {
        display_board(*board);
      }
    } else if (strcmp(cmd, "print_board") == 0) {
      if (load_filename() != NULL)
      {
        printf("Too many parameters: command accepts no parameters\n");
        printf("Correct usage: print_board\n");
        return CMD_ERR;
      }
      if ((*board)->mode == INIT_MODE) {
        printf("Command not available in init mode\n");
        return CMD_ERR;
      }
      display_board(*board);
    } else {
      printf("Invalid command\n");
      return CMD_ERR;
    }
    if (is_board_solved(*board)) {
      printf("Board solved!\n");
      (*board)->mode = INIT_MODE;
    }
    return CMD_SUCCESS;
}