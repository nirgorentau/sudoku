CC = gcc
OBJS = test.o board.o linked_list.o file_io.o stack.o display_board.o deterministic_solver.o lp_solver.o
IDIR = ../../src
EXEC = lp_solver
CFLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
test.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c test.c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(CFLAGS)
lp_solver.o:
	$(CC) $(CFLAGS) $(GUROBI_COMP) -c $(IDIR)/lp_solver.c $(IDIR)/deterministic_solver.c  $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(CFLAGS)
clean:
	rm -f *.o $(EXEC) sol_log.log