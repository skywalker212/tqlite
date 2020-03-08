src = ./src/
tqlite: tqlite.o
	gcc tqlite.o -o tqlite

tqlite.o:
	gcc -c $(src)tqlite.c -o tqlite.o

clean:
	rm  -f tqlite tqlite.o
