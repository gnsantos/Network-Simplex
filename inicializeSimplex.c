#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"
#include "lerEntrada.h"

/*O primeiro passo para achar uma solucao viavel e comecar o simplex eh assegurar que podemos
  construir uma arvore de solucao. Para tal vamos escolher um vertic v que nao eh nem a origem,
  nem o destino do escoamento e vamos inserir arcos ficticios da orgiem para v e de v para os
  demais. Contudo, como nao queremos esses arcos falsos em nossa solucao inicial 
(muito pelo contrario! sua presena indica a inviabilidade do problema que queremos resolver!)
damos a essas arestas um custo infinito, para nos assegurar de que elas seram substituidas.

Alem disso, ja conseguimos uma arvore inicial para resolver o problema auxiliar: setamos a ori-
gem como raiz. Ela sera o pai do vertice do qual emanam os arcos artificiais, e este sera o pai 
dos demais vertices.*/
int insere_arcos_artificiais(Graph g){
  Arc x;
  Vertex v,u;
  double infinito = 1.0/0.0; /*vamos definir o custo das arestas artificiais como infinito
			       dessa forma, desencorajamos seu uso e sabemos que se ao final nao
			       conseguimos nos livrar delas, o problema eh inviavel*/

  /*achamos um vertice que nao eh nem a origem, nem o destino*/
  for(v = 0; v < g->n; v++){
    if(v != g->origem && v != g->destino){
      /*adicionamos arcos de custo infinito indo da origem para o vertice e dele para os demais*/
      x = add_arc(g,g->origem,v, infinito, g->demanda);
      set_parent(g,g->origem,v, x);
      for(u = 0; u < g->n; u++){
	if(u != g->origem && v != u){
	  if(u != g->destino) 
	    x = add_arc(g,v,u,infinito,0);
	  else 
	    x = add_arc(g,v,u,infinito,g->demanda);
	  set_parent(g,v,u,x);
	}
      }
      break;
    }
  }
  return v; /*retornamos o vertice que foi utilizado*/
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

int main(int argc, char** argv){
  char * entrada = argv[1];
  Graph g = le_entrada(entrada);
  Vertex v,u;
  Arc x;
  v = insere_arcos_artificiais(g);
  show_graph(g);
  show_tree(g);
  while(scanf("%d %d", &u, &v) != EOF){
    x = is_arc(g,u,v);
    if(x == NULL){
      puts("denied!");
      continue;
    }
    show_path(g,x);
    show_graph(g);
  }
  return 0;
}

/*
void simplex(Graph g){ ----> g ja deve conter uma arvore com solucao inicial e potenciais certos
  Arc x, leaving;
  Vertex *path;
  int s;
  while( (x = entry_arc(g)) != null){
    leaving = tree_path(g,e, &path, &s);
    update_parents(g); --------------------> tem que colocar o que entra e tirar o que sai
    update_profundidades(g);
    update_y(g);
  }
}
*/
