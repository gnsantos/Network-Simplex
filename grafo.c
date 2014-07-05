#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"

#define null NULL

Graph init_graph(int n, Vertex origem, Vertex destino, double demanda){/*INICIALIZA UM GRAFO*/
  Graph g = malloc(sizeof(*g)); /*aloca memoria para o grafo*/
  Vertex v;
  g->n = n; /*inicializa o numero de vertices*/
  g->m = 0; /*inicialmente, nao temos nenhum arco*/
  g->adj = malloc(sizeof(list)*n); /*alocamos espaco para o vetor d listas*/
  for(v = 0; v < n; v++) /*como ainda nao ha nenhum arco, setamos todas as listas para null*/
    g->adj[v] = NULL; 
  g->origem = origem;
  g->destino = destino;
  g->demanda = demanda;
  g->pais = malloc(sizeof(Vertex)*g->n);
  for(v = 0; v < g->n; v++)
    g->pais[v] = v; /*a arvore ainda nao foi construida: cada vertice eh seu proprio pai*/
  g->profundidade = malloc(sizeof(int)*g->n);
  for(v = 0; v < g->n; v++)
    g->profundidade[v] = 1;
  g->arvore = malloc(sizeof(Arc)*(g->n));
  for(v = 0; v < g->n; v++)
    g->arvore[v] = NULL;
  g->y = malloc(sizeof(double)*g->n);
  for(v = 0; v < g->n; v++)
    g->y[v] = 0;
  return g;
}

Arc new_arc(Vertex ini, Vertex dest, double cost, double fluxo){ /*inicializa um novo arco*/
  Arc new = malloc(sizeof(*new));
  new->ini = ini;
  new->dest = dest;
  new->cost = cost;
  new->fluxo = fluxo;
  new->inTree = 0;
  return new;
}

Arc is_arc(Graph g, Vertex u, Vertex v){/*CHECA SE UM ARCO u->v JA EXISTE*/
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

Arc is_tree_arc(Graph g, Vertex u, Vertex v){
  int i;
  Arc x;
  for(i = 0; i < g->n; i++){
    x = g->arvore[i];
    if(x != null && x->ini == u && x->dest == v)
      return x;
  }
  return null;
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

/*Atribui u como novo pai de v na arvore*/
void set_parent(Graph g, Vertex u, Vertex v, Arc x){
  g->pais[v] = u;
  g->profundidade[v] = 1 + g->profundidade[u];
  g->arvore[v] = x;
  x->inTree = 1;
}

/*um simples getter para o pai*/
Vertex prnt(Graph g, Vertex v){
  return g->pais[v];
}

/*getter para a profundidade de v*/
int depth(Graph g, Vertex v){
  return g->profundidade[v];
}

Vertex* reverse_path(Vertex *path, int size){
  int i = 0;
  Vertex* r = malloc(sizeof(Vertex)*size);
  for(i = 0; i < size; i++)
    r[i] = path[size-1-i];
  free(path);
  return r;
}


/*imprime os vertices adjacentes a todos os vertices, o custo da aresta entre ele e seu fluxo*/
void show_graph(Graph g){
  Vertex v;
  list l;
  printf("Origem: %d\nDestino: %d\nEscoamento: %lf\n", g->origem, g->destino, g->demanda);
  for(v = 0; v < g->n; v++){
    printf("%d:\n", v);
    for(l = g->adj[v]; l != NULL; l = l->next){
      Arc x = l->arco;
      if(v == x->ini)
	printf("\t %d - cost: %lf - fluxo: %lf\n", x->dest, x->cost, x->fluxo);
    }
    puts("");
  }
}

/*imprime a representacao da arvore sendo utilizada*/
void show_tree(Graph g){
  Vertex v;
  Arc x;
  puts("Pais:");
  for(v = 0; v < g->n; v++){
    printf("\t%d -> %d\t(%d)\n", prnt(g,v), v, depth(g,v));
  }
  puts("Arcos:");
  for(v = 0; v < g->n; v++){
    if((x = g->arvore[v]) != null){
      printf("\t%d->%d - cost: %lf - fluxo: %lf\n",x->ini, x->dest, x->cost, x->fluxo);
    }
  }
  puts("");
}

/*imprime o caminho entre vertices u e v e qual aresta sai da base se colocamos o arco u->v*/
void show_path(Graph g, Arc e){
  Vertex *path = null;
  Arc x;
  int size = 0;
  int i;
  x = tree_path(g,e,&path, &size);
  for(i = 0; i < size; i++)
    printf(" %d ",path[i]);
  puts("");
  printf("sai %d->%d, de custo %lf e antigo fluxo %lf\n\n", x->ini, x->dest, x->cost, x->fluxo);
}
