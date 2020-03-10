IDIR = ../../src
CC = gcc
CFLAGS=-I$(IDIR)
  
board: test.c
	$(CC) -o $@ $^ $(IDIR)/board.c $(CFLAGS)
	
clean:
	rm board
