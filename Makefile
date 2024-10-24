CC=clang -g

all: exec_jeu

jeu_processus.o: test2.c joueur.h
	$(CC) -c test2.c -o jeu_processus.o

logique_jeu.o: joueur.c joueur.h
	$(CC) -c joueur.c -o logique_jeu.o

exec_jeu: jeu_processus.o logique_jeu.o
	$(CC) jeu_processus.o logique_jeu.o -o exec_jeu

clean:
	rm -f exec_jeu *.o
