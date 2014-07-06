ep: inicializeSimplex.o lerEntrada.o grafo.o simplex.o ep.o arvore.o
	gcc inicializeSimplex.o lerEntrada.o grafo.o simplex.o arvore.o ep.o -g -o ep
ep.o: grafo.c lerEntrada.c simplex.c inicializeSimplex.c
	gcc -Wall -ansi -pedantic -c -g ep.c
inicializeSimplex.o: inicializeSimplex.c lerEntrada.c grafo.c simplex.c
	gcc -Wall -ansi -pedantic -c -g inicializeSimplex.c
simplex.o: simplex.c grafo.c
	gcc -Wall -ansi -pedantic -c -g simplex.c
lerEntrada.o: lerEntrada.c grafo.c
	gcc -Wall -ansi -pedantic -c -g lerEntrada.c
arvore.o: arvore.c
	gcc -Wall -ansi -pedantic -c -g arvore.c
grafo.o: grafo.c
	gcc -Wall -ansi -pedantic -c -g grafo.c
clean:
	rm *.o *~
