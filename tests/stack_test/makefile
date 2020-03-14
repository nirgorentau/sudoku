IDIR = ../../src
CC = gcc
CFLAGS=-I$(IDIR)
  
stack: test.c
	$(CC) -o $@ $^ $(IDIR)/stack.c $(IDIR)/linked_list.c $(CFLAGS)
	
clean:
	rm stack
