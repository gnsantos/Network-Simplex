#ifndef H_INICIALIZESIMPLEX_DEFINE
#define H_INICIALIZESIMPLEX_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"
#include "simplex.h"

Arvore insere_arcos_artificiais(Graph); /*insere os arcos artificiais para comecar a fase 1*/
void remove_arcos_artificiais(Graph); /*remove os arcos inseridos na fase 1*/
Arvore acha_solucao_inicial(Graph); /*acha uma solucao viavel para o problema*/
int checa_viabilidade(Arvore); /*checa se a solucao achada contem arcos artificiais, ou seja,
				se o problema inicial tem solucao.*/

#endif
