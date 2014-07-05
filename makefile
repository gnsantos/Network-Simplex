ep: inicializeSimplex.o lerEntrada.o grafo.o simplex.o ep.o
	gcc inicializeSimplex.o lerEntrada.o grafo.o simplex.o ep.o -o ep
ep.o: grafo.c lerEntrada.c simplex.c inicializeSimplex.c
	gcc -Wall -ansi -pedantic -c ep.c
inicializeSimplex.o: inicializeSimplex.c lerEntrada.c grafo.c simplex.c
	gcc -Wall -ansi -pedantic -c inicializeSimplex.c
simplex.o: simplex.c grafo.c
	gcc -Wall -ansi -pedantic -c simplex.c
lerEntrada.o: lerEntrada.c grafo.c
	gcc -Wall -ansi -pedantic -c lerEntrada.c
grafo.o: grafo.c
	gcc -Wall -ansi -pedantic -c grafo.c
clean:
	rm *.o *~
