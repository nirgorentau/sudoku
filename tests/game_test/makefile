CC = gcc
OBJS = test.o board.o linked_list.o file_io.o stack.o display_board.o deterministic_solver.o lp_solver.o game.o
IDIR = ../../src
EXEC = game_test
CFLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors -I$(IDIR) -g
GUROBI_COMP = -I${GUROBI_HOME}/include
GUROBI_LIB = -L${GUROBI_HOME}/lib -lgurobi90

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
test.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c test.c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(IDIR)/game.c $(CFLAGS)
lp_solver.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(CFLAGS)
clean:
	rm -f *.o $(EXEC) int_sol.lp sol_log.log
