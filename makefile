ep: inicializeSimplex.o lerEntrada.o grafo.o simplex.o ep.o arvore.o
	gcc inicializeSimplex.o lerEntrada.o grafo.o simplex.o arvore.o ep.o -g -o ep -lm
ep.o: grafo.c lerEntrada.c simplex.c inicializeSimplex.c ep.c
	gcc -Wall -ansi -pedantic -c -O2 ep.c -lm
inicializeSimplex.o: inicializeSimplex.c lerEntrada.c grafo.c simplex.c
	gcc -Wall -ansi -pedantic -c -O2 inicializeSimplex.c -lm
simplex.o: simplex.c grafo.c
	gcc -Wall -ansi -pedantic -c -O2 simplex.c -lm
lerEntrada.o: lerEntrada.c grafo.c
	gcc -Wall -ansi -pedantic -c -O2 lerEntrada.c -lm
arvore.o: arvore.c
	gcc -Wall -ansi -pedantic -c -O2 arvore.c -lm
grafo.o: grafo.c
	gcc -Wall -ansi -pedantic -c -O2 grafo.c -lm
clean:
	rm *.o *~
