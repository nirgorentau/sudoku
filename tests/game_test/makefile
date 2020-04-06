IDIR = ../../src
CC = gcc
CFLAGS=-I$(IDIR)
  
game_test: test.c
	$(CC) -o $@ $^ $(IDIR)/board.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(IDIR)/game.c $(IDIR)/deterministic_solver.c $(IDIR)/lp_solver.c $(IDIR)/linked_list.c $(IDIR)/stack.c $(CFLAGS)
	
clean:
	rm game_test
