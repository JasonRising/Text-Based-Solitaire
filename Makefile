CC=gcc
CCFLAGS=-std=c89 -o solitaire -g

all: solitaire
	
solitaire: cards.c cards.h
	$(CC) $(CCFLAGS) solitaire.c cards.c
clean:
	rm -f solitaire
