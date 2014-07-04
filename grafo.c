#include <stdio.h>
#include <stdlib.h>
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
  return g;
}

Arc new_arc(Vertex ini, Vertex dest, double cost, double fluxo){ /*inicializa um novo arco*/
  Arc new = malloc(sizeof(*new));
  new->ini = ini;
  new->dest = dest;
  new->cost = cost;
  new->fluxo = fluxo;
  return new;
}

int is_arc(Graph g, Vertex u, Vertex v){/*CHECA SE UM ARCO u->v JA EXISTE*/
  list l;
  for(l = g->adj[u]; l != NULL; l = l->next){/*percorre a lista de adjacencia de u*/
    Arc x = l->arco; /*pega o arco associado aquela posicao na lista*/
    if(x->ini == u && x->dest == v) /*se e um arco de u para v, o arco ja existe e retorna-se 1*/
      return 1;
  }
  return 0; /*o arco nao existe*/
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
}

/*um simples getter para o pai*/
Vertex prnt(Graph g, Vertex v){
  return g->pais[v];
}

/*getter para a profundidade de v*/
int depth(Graph g, Vertex v){
  return g->profundidade[v];
}

/*procura o caminho entre u e v na arvore representada. Assume que u eh mai profundo que v.*/
Vertex *tree_path(Graph g, Vertex u, Vertex v, int* s){
  Vertex aux;
  Vertex *upath = malloc(sizeof(Vertex)*g->n);
  Vertex *vpath = malloc(sizeof(Vertex)*g->n);
  Vertex *path;
  int i = 0, j = 0;

  /*assegura-se que u eh pelo menos tao profundo quanto v. Se nao for, troca-se os nomes.*/
  if(depth(g,u) < depth(g,v)){
    aux = u;
    u = v;
    v = aux;
  }

  for(aux = 0; aux < g->n; aux++)
    upath[aux] = vpath[aux] = -1;
  
  upath[0] = u;
  vpath[0] = v;

  while(depth(g,upath[i]) != depth(g,v)){
    upath[i+1] = prnt(g,upath[i]);
    i = i + 1;
  }
  
  while(upath[i] != vpath[j]){
    upath[i+1] = prnt(g,upath[i]);
    vpath[j+1] = prnt(g, vpath[j]);
    j = j + 1;
    i = i + 1;
  }
  
  path = malloc(sizeof(Vertex)*(i + j + 1));
  
  for(aux = 0; aux <= i; aux++){
    path[aux] = upath[aux];
    *s = *s + 1;
  }
  
  for(aux = i+1; j > 0; aux++){
    path[aux] = vpath[--j];
    *s = *s + 1;
  }

  free(upath);
  free(vpath);
  
  return path;

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

void show_path(Graph g, Vertex u, Vertex v){
  int size = 0;
  Vertex *path = tree_path(g,u,v,&size);
  int i;
  for(i = 0; i < size; i++)
    printf(" %d ",path[i]);
  puts("");
}
