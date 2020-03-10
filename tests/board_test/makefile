IDIR = ../../src
CC = gcc
CFLAGS=-I$(IDIR)
  
board: test.c
	$(CC) -o $@ $^ $(IDIR)/board.c $(IDIR)/display_board.c $(IDIR)/file_io.c $(CFLAGS)
	
clean:
	rm board
