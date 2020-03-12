IDIR = ../../src
CC = gcc
CFLAGS=-I$(IDIR)
  
linked_list: test.c
	$(CC) -o $@ $^ $(IDIR)/linked_list.c $(CFLAGS)
	
clean:
	rm linked_list
