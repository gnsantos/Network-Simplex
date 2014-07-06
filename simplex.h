#ifndef H_SIMPLEX_DEFINE
#define H_SIMPLEX_DEFINE

#include "grafo.h"
#include "arvore.h"
#include <stdio.h>

Arc entry_arc(Graph, Arvore); /*escolhe um arco para entrar na base*/
Arc tree_path(Arvore, Arc, Vertex**,int*); /*encontra o caminho entre vertices e escolhe um arco para sair da base*/
void update_prnt(Arvore, Arc); /*atualiza o vetor de pais e de arcos da arvore*/
void update_depth(Arvore); /*atualiza o vetor de profundidades*/
void update_y(Arvore); /*atualiza o vetor de potenciais*/
void network_simplex(Graph, Arvore); /*Roda o simplex de redes, tendo como entrada um Graph - a configuracao da rede e uma Arvore, com uma solucao viavel*/
double net_cost(Arvore, Graph); /*Devolve o custo de transporte dada uma Arvore contendo uma solucao*/

#endif
