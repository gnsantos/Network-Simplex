#ifndef H_INICIALIZESIMPLEX_DEFINE
#define H_INICIALIZESIMPLEX_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"
#include "simplex.h"

int insere_arcos_artificiais(Graph);
void remove_arcos_artificiais(Graph);
void acha_solucao_inicial(Graph);
int checa_viabilidade(Graph);

#endif
