/****************************************************************
EXERCICIO-PROGRAMA DE MAC0315 - PROGRAMACAO LINEAR
PRIMEIRO SEMESTRE DE 2014
ALUNO: GERVASIO PROTASIO DOS SANTOS NETO
NUSP: 7990996
PROFESSOR: WALTER MASCARENHAS

arquivo: arvore.h
 ****************************************************************/

#ifndef H_TREE_DEFINE
#define H_TREE_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

struct tree{
  int n; /*numero de nos na arvore*/
  Vertex *pais; 
  int *profundidade;
/*Vetor de pais: eh usado para manter a estrutura de uma arvore geradora
  Vetor de profundidade: mantem quantos nos separam um vertice da raiz
  Por exemplo, a seguinte arvore:
                       1
                       |
                       |
		 ------5-----4
		 |     |      \
                 |     |       \
		 |     |        \
		 3     2         0
tem o seguinte vetor de pais:
      i: 0 1 2 3 4 5
pais[i]: 4 1 5 5 5 1
(note que a raiz eh o vertice v com pais[v] = v;)
e de profundidade:
              i: 0 1 2 3 4 5
profundidade[i]: 4 1 3 3 3 1
*/
  Arc *arvore; /*Um vetor de arcos: o arco arvore[i] eh o arco liga pais[i] e i*/
  double *y; /*vetor de potenciais.*/
};

typedef struct tree* Arvore;

Arvore init_tree(int n); /*inicializa uma arvore alocando a memoria necessaria*/
Arc is_tree_arc(Arvore, Vertex, Vertex); /*verifica se ha na arvore um arco entre os verices entrados*/
void set_parent(Arvore, Vertex, Vertex, Arc); /*define o pai de um vertice na arvore e qual o arco que o liga*/
Vertex prnt(Arvore, Vertex); /*devolve o pai de um vertice*/
int depth(Arvore, Vertex); /*devolve a profundidade de um vertice*/
Vertex *reverse_path(Vertex*, int); /*inverte um caminho*/
void show_tree(Arvore); /*imprime um descricao da arvore e de seus arcos*/
void show_path(Arvore, Arc); /*mostra o caminho entre as extremidades de um arco. Usado para debug.*/

#endif
