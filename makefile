ep: inicializeSimplex.o lerEntrada.o grafo.o
	gcc inicializeSimplex.o lerEntrada.o grafo.o -o ep
inicializeSimplex.o: inicializeSimplex.c lerEntrada.c grafo.c
	gcc -Wall -ansi -pedantic -c -g inicializeSimplex.c
lerEntrada.o: lerEntrada.c grafo.c
	gcc -Wall -ansi -pedantic -c -g lerEntrada.c
grafo.o: grafo.c
	gcc -Wall -ansi -pedantic -c -g grafo.c
clean:
	rm *.o *~
