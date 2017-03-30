CC = gcc

CFLAGS = -std=c99 -g

all: ppascal

%o: %c
	$(CC) $(CFLAGS) -c -o $@ $<

%tab.c %tab.h: %y
	bison -d $<

%.l.c: %.l
	flex -o $@ $<

clean:
	rm ppascal.tab* ppascal.l.c
	rm *.o 

tree_abs.o: tree_abs.c tree_abs.h
ppascal.l.o: ppascal.l.c ppascal.tab.h
ppascal: ppascal.tab.o ppascal.l.o tree_abs.o interp.o
	$(CC) $(CFLAGS) ppascal.tab.o ppascal.l.o tree_abs.o interp.o -o $@ -lfl
