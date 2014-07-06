/****************************************************************
EXERCICIO-PROGRAMA DE MAC0315 - PROGRAMACAO LINEAR
PRIMEIRO SEMESTRE DE 2014
ALUNO: GERVASIO PROTASIO DOS SANTOS NETO
NUSP: 7990996
PROFESSOR: WALTER MASCARENHAS

arquivo: arvore.c
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"
#include "arvore.h"
#include "simplex.h"

#define null NULL

Arvore init_tree(int n){
  Arvore t  = malloc(sizeof(*t)); /*aloca memoria*/
  Vertex v;
  t->n = n;
  t->pais = malloc(sizeof(Vertex)*t->n);
  for(v = 0; v < t->n; v++)
    t->pais[v] = v; /*a arvore ainda nao foi construida: cada vertice eh seu proprio pai*/
  t->profundidade = malloc(sizeof(int)*t->n);
  for(v = 0; v < t->n; v++)
    t->profundidade[v] = 1; /*inicialmente nao ha raiz, entao pronfudidades sao 1*/
  t->arvore = malloc(sizeof(Arc)*(t->n));
  for(v = 0; v < t->n; v++)
    t->arvore[v] = NULL; /*como a arvore ainda nao foi montada, os arcos pai sao NULL*/
  t->y = malloc(sizeof(double)*t->n);
  for(v = 0; v < t->n; v++)
    t->y[v] = 0; /*Os potenciais comecao inicializados com zero por padrao. Depois de motada a arvore muda-se isso.*/
  return t;
}

/*Verifica se u->v eh um arco da arvore. Se for, devolve u->v, caso contrario, devolve null*/
Arc is_tree_arc(Arvore g, Vertex u, Vertex v){
  int i;
  Arc x;
  for(i = 0; i < g->n; i++){
    x = g->arvore[i];
    if(x != null && x->ini == u && x->dest == v)
      return x;
  }
  return null;
}


/*Atribui u como novo pai de v na arvore*/
void set_parent(Arvore g, Vertex u, Vertex v, Arc x){
  g->pais[v] = u;
  g->profundidade[v] = 1 + g->profundidade[u];
  g->arvore[v] = x;
  x->inTree = 1;
}

/*um simples getter para o pai*/
Vertex prnt(Arvore g, Vertex v){
  return g->pais[v];
}

/*getter para a profundidade de v*/
int depth(Arvore g, Vertex v){
  return g->profundidade[v];
}

/*inverte um caminho*/
Vertex* reverse_path(Vertex *path, int size){
  int i = 0;
  Vertex* r = malloc(sizeof(Vertex)*size);
  for(i = 0; i < size; i++)
    r[i] = path[size-1-i];
  free(path);
  return r;
}

/*imprime a representacao da arvore sendo utilizada*/
void show_tree(Arvore g, FILE *output){
  Vertex v;
  Arc x;
  fputs("Pais:\n",output);
  for(v = 0; v < g->n; v++){
    if(prnt(g,v) != v)
      fprintf(output, "\t%d -> %d\t(Profundidade: %d)\n", prnt(g,v), v, depth(g,v));
    else 
      fprintf(output,"\t%d\t(raiz)\n", v);
  }
  fputs("Arcos:\n",output);
  for(v = 0; v < g->n; v++){
    if((x = g->arvore[v]) != null){
      fprintf(output,"\t%d->%d - cost: %f - fluxo: %f\n",x->ini, x->dest, x->cost, x->fluxo);
    }
  }
  fputs("\n",output);
}

/*imprime o caminho entre vertices u e v e qual aresta sai da base se colocamos o arco u->v*/
void show_path(Arvore g, Arc e, FILE *output){
  Vertex *path = null;
  Arc x;
  int size = 0;
  int i;
  x = tree_path(g,e,&path, &size);
  for(i = 0; i < size; i++)
    fprintf(output," %d ",path[i]);
  fputs("\n",output);
  fprintf(output,"sai %d->%d, de custo %f e antigo fluxo %f\n\n", x->ini, x->dest, x->cost, x->fluxo);
}

