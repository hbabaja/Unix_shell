
CC = gcc
CFLAGS = -Wall
TARGETS= hbshell
default: hbshell
all: $(TARGETS)

hbshell: hbshell.o
	$(CC) $(CFLAGS) hbshell.o -o hbshell

clean:
	rm -f *.o *~ a.out $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $<	