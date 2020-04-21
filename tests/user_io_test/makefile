CC = gcc
OBJS = test.o board.o linked_list.o file_io.o stack.o display_board.o deterministic_solver.o lp_solver.o game.o user_io.o
IDIR = ../../src
EXEC = play_test
CFLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors -I$(IDIR) -g
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
test.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c test.c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(IDIR)/game.c $(IDIR)/user_io.c
lp_solver.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c
game.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(IDIR)/game.c
user_io.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(IDIR)/game.c $(IDIR)/user_io.c
clean:
	rm -f *.o $(EXEC) int_sol.lp sol_log.log
