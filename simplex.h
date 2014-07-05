#ifndef H_SIMPLEX_DEFINE
#define H_SIMPLEX_DEFINE

#include "grafo.h"
#include <stdio.h>

Arc entry_arc(Graph); /*escolhe um arco para entrar na base*/
Arc tree_path(Graph, Arc, Vertex**,int*); /*encontra o caminho entre vertices e escolhe um arco para sair da base*/
void update_prnt(Graph, Arc); /*atualiza o vetor de pais e de arcos da arvore*/
void update_depth(Graph); /*atualiza o vetor de profundidades*/
void update_y(Graph); /*atualiza o vetor de potenciais*/
void network_simplex(Graph);
float net_cost(Graph);

#endif
