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

/*escolhe um arco para entrar na base*/
Arc entry_arc(Graph g){
  Vertex v;
  Arc x = null;
  list l;
  double inf = 1.0/0.0;
  for(v = 0; v < g->n; v++){
    for(l = g->adj[v]; l != null; l = l->next){
      x = l->arco;
      if((g->y[x->ini] + x->cost <  g->y[x->dest]) && !x->inTree && x->cost != inf) /*candidato a entrar na base*/
	return x; /*devolve o candidato a entrar na base*/
    }
  }
  return x; /*se nao encontrar nenhum candidato valido, retorn null e sabemo que acabamos.*/
}

/*procura o caminho entre u e v na arvore representada, onde u e v sao as extremidades do arco
  de entrada.
  Na verdade, essa funcao serve para determinar o arco que deve sair. Os vertices u e v sao as
  extremidades do arco que entra. Enquanto achamos o caminho, vamos mantendo regsitro do arco
  reverso com menor fluxo; ele sera o retornado pela funcao e removido da arvore.*/
Arc tree_path(Graph g, Arc entry, Vertex** pshow, int* s){
  Arc x, resp, xv, xu;
  Vertex aux, u = entry->ini, v = entry->dest;
  /*para teste de codigo, foram mantidas variaveis para armazenar explicitamente o caminho*/
  Vertex *path; /*caminho de u a v*/
  Vertex *upath = malloc(sizeof(Vertex)*g->n);/*camino de u ate ancestral*/
  Vertex *vpath = malloc(sizeof(Vertex)*g->n);/*caminho de v ate acestral*/
  int i = 0, j = 0;
  double minflow = 1.0/0.0;
  list reversos = null; /*lista dos arcos reversos, cujo fluxo sera diminuido*/
  list diretos = null; /*lista dos arcos diretos, cujo fluxo sera aumentado*/

  for(aux = 0; aux < g->n; aux++)
    upath[aux] = vpath[aux] = -1;
  
  upath[0] = u;
  vpath[0] = v;
  
  /*se u for mais profundo que v, seguimos nosso caminho de u ate a profundidade de v*/
  if(depth(g,u) >= depth(g,v)){
    while(depth(g,upath[i]) != depth(g,v)){
      upath[i+1] = prnt(g,upath[i]);
      /*verificamos se o arco pelo qual u esta na arvore eh direto ou reverso*/
      x = g->arvore[upath[i]];
      list new = malloc(sizeof(list));
      new->arco = x;
      if(x->ini == upath[i]){ /*eh um arco reverso*/
	new->next = reversos; /*se ele for reverso, o colocamos na lista de arcos reversos*/
	reversos = new;
	if(x->fluxo < minflow){ /*se for o reverso de menor fluxo encontrado, armazenamos*/
	  minflow = x->fluxo;
	  resp = x; /*atualizamos a resposta*/
	}
      }
      else{
	new->next = diretos; /*o arco eh adicionado na lista de arcos diretos*/
	diretos = new;
      }
      i = i + 1;
    }
  }
  
  /*repetimos o mesmo procedimento acima, mas para o caso de v ser mais profundo que u*/
  else{
    while(depth(g,vpath[j]) != depth(g,u)){
      vpath[j+1] = prnt(g,vpath[j]);
      x = g->arvore[vpath[j]];
      list new = malloc(sizeof(list));
      new->arco = x;
      if(x->ini != vpath[j]){ /*eh um arco reverso*/
	new->next = reversos;
	reversos = new;
	if(x->fluxo < minflow){
	  minflow = x->fluxo;
	  resp = x;
	}
      }
      else{
	new->next = diretos;
	diretos = new;
      }
      j = j + 1;
    }
  }
  
  /*uma vez que ambos estam na mesma profundidade, seguimos simultaneamente ambos os caminhos
    ate encontramos um acestral comum.*/
  while(upath[i] != vpath[j]){
    upath[i+1] = prnt(g,upath[i]);
    vpath[j+1] = prnt(g, vpath[j]);

    /*repetimos o processo de verificar se os arcos que encontramos sao diretos ou reversos
      e de colocarmos nas respectivas listas. Se encontramos um arco reverso de fluxo menor
      do que se tinha visto antes, atualiza-se a resposta.*/
    xv = g->arvore[vpath[j]];
    xu = g->arvore[upath[i]];
    list newv = malloc(sizeof(list));
    list newu = malloc(sizeof(list));
    newv->arco = xv;
    newu->arco = xu;
    
    if(xv->ini != vpath[j]){ /*eh um arco reverso*/
      newv->next = reversos;
      reversos = newv;
      if(xv->fluxo < minflow){
	minflow = xv->fluxo;
	resp = xv;
      }
    }
    else{
      newv->next = diretos;
      diretos = newv;
    }
    
    if(xu->ini == upath[i]){ /*eh um arco reverso*/
      newu->next = reversos;
      reversos = newu;
      if(xu->fluxo < minflow){
	minflow = xu->fluxo;
	resp = xu;
      }
    }
    else{
      newu->next = diretos;
      diretos = newu;
    }

    j = j + 1;
    i = i + 1;
  }

  while(reversos != null){
    reversos->arco->fluxo -= minflow;
    reversos = reversos->next;
  }
  while(diretos != null){
    diretos->arco->fluxo += minflow;
    diretos = diretos->next;
  }
  entry->fluxo = minflow;

  /*!!!!!!!!!!!!!!!OLHA AQUI!!!!!!!!!!!!!!!!!!*/
  resp->inTree = 0;
  
  /*para fins de debug, mantemos um vetor com o caminho explicito*/
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
  *pshow = path;/*possibilita enxergar o caminho fora da funcao. Feito para fins de debug*/
  
  return resp;

}

Vertex* reverse_path(Vertex *path, int size){
  int i = 0;
  Vertex* r = malloc(sizeof(Vertex)*size);
  for(i = 0; i < size; i++)
    r[i] = path[size-1-i];
  free(path);
  return r;
}

void update_prnt(Graph g, Arc entry){
  Arc leaving, auxiliar;
  Vertex *path, e1, e2, f1, f2, root;
  int size = 0, i, beforeroot = 1, posroot = pow(2,30), posf2;
  leaving = tree_path(g,entry,&path,&size);
  for(root = 0; root < g->n; root++)
    if(prnt(g,root) == root)
      break;
  if(depth(g,leaving->ini) < depth(g,leaving->dest)){
    f1 = leaving->ini;
    f2 = leaving->dest;
  }
  else{
    f1 = leaving->dest;
    f2 = leaving->ini;
  }
  if(depth(g,entry->ini) < depth(g,entry->dest)){
    e1 = entry->ini;
    e2 = entry->dest;
  }
  else{
    e1 = entry->dest;
    e2 = entry->ini;
  }

  if(path[0] == e1)
    path = reverse_path(path, size);
  
  for(i = 0; i < size; i++){
    if(path[i] == root) posroot = i;
    if(path[i] == f2) posf2 = i;
  }

  if(posf2 > posroot) beforeroot = 0;

  if(!beforeroot){
    set_parent(g, e2, e1, entry);
    for(i = posf2; path[i] != e1; i++){
      auxiliar = is_tree_arc(g,path[i], path[i+1]);
      if(auxiliar == null) 
	auxiliar = is_tree_arc(g,path[i+1],path[i]);
      set_parent(g, path[i+1],path[i], auxiliar);
    } 
  }
  else{
    set_parent(g, e1, e2, entry);    
    for(i = 0; path[i] != f2; i++){
      auxiliar = is_tree_arc(g, path[i], path[i+1]);
      if(auxiliar == null) 
	auxiliar = is_tree_arc(g,path[i+1],path[i]);
      set_parent(g,path[i], path[i+1], auxiliar);
    }
  }

}

void update_depth(Graph g){
  Vertex v;
  int i = 0;
  int updated = 1;
  while(i < g->n && updated == 1){
    updated = 0;
    for(v = 0; v < g->n; v++){
      if((depth(g,v) != depth(g,g->pais[v]) + 1) && v != prnt(g,v)){
	g->profundidade[v] = g->profundidade[prnt(g,v)] + 1;
	updated = 1;
      }
      if(v == prnt(g,v))
	g->profundidade[v] = 1;
    }
    i++;
  }
}

void update_y(Graph g){
  Vertex root, v;
  int i;
  for(root = 0; root < g->n; root++)
    if(prnt(g,root) == root)
      break;
  
  g->y[root] = 0;

  for(i = 0; i < g->n; i++){
    
  }
}

/*
  void update_sucessor(Graph g){ }
  void update_pais(Graph g){ }
  void update_profundidade(Graph g){ }
*/

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
