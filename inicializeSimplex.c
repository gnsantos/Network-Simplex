/****************************************************************
EXERCICIO-PROGRAMA DE MAC0315 - PROGRAMACAO LINEAR
PRIMEIRO SEMESTRE DE 2014
ALUNO: GERVASIO PROTASIO DOS SANTOS NETO
NUSP: 7990996
PROFESSOR: WALTER MASCARENHAS

arquivo: inicializeSimplex.c
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"
#include "arvore.h"
#include "lerEntrada.h"
#include "simplex.h"

/*O primeiro passo para achar uma solucao viavel e comecar o simplex eh assegurar que podemos
  construir uma arvore de solucao. Para tal vamos escolher um vertic v que nao eh nem a origem,
  nem o destino do escoamento e vamos inserir arcos ficticios da orgiem para v e de v para os
  demais. Contudo, como nao queremos esses arcos falsos em nossa solucao inicial 
(muito pelo contrario! sua presena indica a inviabilidade do problema que queremos resolver!)
damos a essas arestas um custo infinito, para nos assegurar de que elas seram substituidas.

Alem disso, ja conseguimos uma arvore inicial para resolver o problema auxiliar: setamos a ori-
gem como raiz. Ela sera o pai do vertice do qual emanam os arcos artificiais, e este sera o pai 
dos demais vertices.

Ao final, devolvemos a arvore construida. Ela eh uma solucao viavel para o problema auxiliar
que resolvemos nessa primeira fase do simplex.*/
Arvore insere_arcos_artificiais(Graph g){
  Arc x;
  Vertex v,u;
  double infinito = 1.0/0.0; /*vamos definir o custo das arestas artificiais como infinito
			       dessa forma, desencorajamos seu uso e sabemos que se ao final nao
			       conseguimos nos livrar delas, o problema eh inviavel*/
  Arvore t = init_tree(g->n);
  /*achamos um vertice que nao eh nem a origem, nem o destino*/
  if(g->n == 2){
    x = add_arc(g, g->origem, g->destino, infinito, g->demanda);
    set_parent(t,g->origem,g->destino,x);
    return t;
  }
  
  for(v = 0; v < g->n; v++){
    if(v != g->origem && v != g->destino){
      /*adicionamos arcos de custo infinito indo da origem para o vertice e dele para os demais*/
      x = add_arc(g,g->origem,v, infinito, g->demanda);
      set_parent(t,g->origem,v, x);
      for(u = 0; u < g->n; u++){
	if(u != g->origem && v != u){
	  if(u != g->destino) 
	    x = add_arc(g,v,u,infinito,0);
	  else 
	    x = add_arc(g,v,u,infinito,g->demanda);
	  set_parent(t,v,u,x);
	}
      }
      break;
    }
  }
  return t; /*retornamos a arvore que foi construida*/
}

/*Uma vez em posse da solucao inicial, nao precisaremos mais dos arcos artificiais e podemos
 remove-los*/
void remove_arcos_artificiais(Graph g){
  Vertex v;
  list p,q;
  float inf = 1.0/0.0;
  /*percore a lista de adjacencia de todos os vertices em busca de arcos com custo infinito - ou seja, artificiais*/
  for(v = 0; v < g->n; v++){

    /*enquanto o primeiro arco da lista for artificial, muda-se o primeiro arco, efetivamente
     eliminando o arco artificial*/
    while(g->adj[v]->arco->cost == inf){
      g->adj[v] = g->adj[v]->next;
      /*free(q);*/
    }
    
    /*para assegurar que nao ha arcos com custo infinito no meio da lista de adjacencia,
     vamos percorre-la, e se encontramos algum, fazemos com que seu antecessor aponte para seu
    sucessor, efetivamente eliminando-o.*/
    q = g->adj[v];
    p = q->next;

    while(p != NULL){
      if(p->arco->cost == inf){
	q->next = p->next;
	free(p);
      }
      q = p;
      p = p->next;
    }
  }
}

/*Insere os arcos artificiais na rede para achar uma solucao inicial viavel, entao,
 atualizamos o potencial dos vertices da arvore e rodamos o metodo do simplex de redes.

 O que se retorna eh uma arvore que eh uma solucao viavel para o problema inicial, caso
 nao contenha arcos artificiais.

 Foi verificado que, quando o problema eh viavel, a arvore devolvida corresponde a solu-
 cao otima.
*/
Arvore acha_solucao_inicial(Graph g){
  Arvore t;
  t = insere_arcos_artificiais(g); /*peg*/
  update_y(t);
  network_simplex(g,t);
  remove_arcos_artificiais(g);
  return t;
}

/*checa se uma arvore eh uma solucao viavel para o problema original. Para isso, vemos
 se ela contem alguma arco artificial.*/

int checa_viabilidade(Arvore g){
  Arc x;
  Vertex v;
  double inf = 1.0/0.0;
  for(v = 0; v < g->n; v++){
    x = g->arvore[v];
    if(x == NULL) continue;
    if(x->cost == inf)
      return 0;
  }
  return 1;
}
