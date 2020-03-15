IDIR = ../../src
CC = gcc
CFLAGS=-I$(IDIR)
  
det_solver: test.c
	$(CC) -o $@ $^ $(IDIR)/deterministic_solver.c $(IDIR)/board.c $(IDIR)/stack.c $(IDIR)/linked_list.c $(IDIR)/display_board.c $(CFLAGS)
	
clean:
	rm det_solver
