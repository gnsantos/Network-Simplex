#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"

#define null NULL

/*INICIALIZA UM GRAFO*/
Graph init_graph(int n, Vertex origem, Vertex destino, double demanda){
  Graph g = malloc(sizeof(*g)); /*aloca memoria para o grafo*/
  Vertex v;
  g->n = n; /*inicializa o numero de vertices*/
  g->m = 0; /*inicialmente, nao temos nenhum arco*/
  g->adj = malloc(sizeof(list)*n); /*alocamos espaco para o vetor d listas*/
  for(v = 0; v < n; v++) /*como ainda nao ha nenhum arco, setamos todas as listas para null*/
    g->adj[v] = NULL; 
  g->origem = origem; /*seta a origem do fluxo*/
  g->destino = destino; /*seta o destino final do fluxo*/
  g->demanda = demanda; /*seta qunto de produto passa pelo grafo*/
  
  return g;
}

/*inicializa um novo arco*/
Arc new_arc(Vertex ini, Vertex dest, double cost, double fluxo){ 
  Arc new = malloc(sizeof(*new));
  new->ini = ini;
  new->dest = dest;
  new->cost = cost;
  new->fluxo = fluxo;
  new->inTree = 0;
  return new;
}

/*CHECA SE UM ARCO u->v EXISTE*/
Arc is_arc(Graph g, Vertex u, Vertex v){/*CHECA SE UM ARCO u->v EXISTE*/
  list l;
  double inf = 1.0/0.0;
  for(l = g->adj[u]; l != NULL; l = l->next){/*percorre a lista de adjacencia de u*/
    Arc x = l->arco; /*pega o arco associado aquela posicao na lista*/
    if(x->ini == u && x->dest == v && x->cost != inf ) /*se e um arco nao artificial de u para v
							existe, ele eh retornado.*/
      return x;
  }
  return null; /*o arco nao existe*/
}

/*insere um novo arco no grafo*/
Arc add_arc(Graph g, Vertex ini, Vertex dest, double cost, double fluxo){
  Arc new = new_arc(ini, dest, cost, fluxo); /*inicializa o arco*/
  list l1 = malloc(sizeof(*l1)); /*nova entrada para lista de ini*/
  list l2 = malloc(sizeof(*l2)); /*nova entrada para lista de dest*/
  l1->arco = new;
  l2->arco = new;
  /*insere o novo arco no comeco da lista de ini*/
  l1->next = g->adj[ini];
  g->adj[ini] = l1;
  /*insere o arco no comeco da lista de dest*/
  l2->next = g->adj[dest];
  g->adj[dest] = l2;

  g->m = g->m + 1; /*atualiza o numero de arcos*/
  
  return new;
  /*Nota-se que na lista de adjacencia de um vertice guarda-se tanto os arcos que chegam nele
    quanto os arcos que saem dele*/
}

/*imprime os vertices adjacentes a todos os vertices, o custo da aresta entre ele e seu fluxo*/
void show_graph(Graph g){
  Vertex v;
  list l;
  printf("Origem: %d\nDestino: %d\nEscoamento: %f\n", g->origem, g->destino, g->demanda);
  for(v = 0; v < g->n; v++){
    printf("%d:\n", v);
    for(l = g->adj[v]; l != NULL; l = l->next){
      Arc x = l->arco;
      if(v == x->ini)
	printf("\t %d - cost: %f - fluxo: %f\n", x->dest, x->cost, x->fluxo);
    }
    puts("");
  }
}

