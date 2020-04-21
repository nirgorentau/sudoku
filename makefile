CC = gcc
OBJS = main.o board.o linked_list.o file_io.o stack.o display_board.o deterministic_solver.o lp_solver.o game.o user_io.o
IDIR = src
EXEC = sudoku-console
CFLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors -I$(IDIR) -g
GUROBI_COMP = -I${GUROBI_HOME}/include
GUROBI_LIB = -L${GUROBI_HOME}/lib -lgurobi56

all: sudoku-console

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c $(IDIR)/main.c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(IDIR)/game.c $(IDIR)/user_io.c $(CFLAGS)
clean:
	rm -f *.o $(EXEC)
