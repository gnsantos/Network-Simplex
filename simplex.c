#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"

#define null NULL

/*escolhe um arco para entrar na base*/
Arc entry_arc(Graph g){
  Vertex v;
  Arc e, x = null;
  list l;
  double inf = 1.0/0.0;
  for(v = 0; v < g->n; v++){
    for(l = g->adj[v]; l != null; l = l->next){
      e = l->arco;
      /*candidato a entrar na base*/
      if((g->y[e->ini] -  g->y[e->dest] > e->cost) && !e->inTree && e->cost != inf){ 
	x = e;
	return x; /*devolve o candidato a entrar na base*/
      }
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


void update_prnt(Graph g, Arc entry){
  Arc leaving, auxiliar;
  Vertex *path, e1, e2, f1, f2, root;
  int size = 0, i, beforeroot = 1, posroot = pow(2,30), posf2, posf1;
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
    if(path[i] == f1) posf1 = i;
  }

  if(posf2 > posroot || posf2 > posf1) beforeroot = 0;

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
  Arc x;
  int i;
  int *atualizado = malloc(sizeof(int)*g->n);
  int falta = g->n;
  for(i = 0; i < g->n; i++)
    atualizado[i] = 0;
  
  for(root = 0; root < g->n; root++)
    if(prnt(g,root) == root)
      break;
  
  g->y[root] = 0;
  atualizado[root] = 1;
  falta--;
  i = 0;
  while(i < g->n && falta > 0){
    for(v = 0; v < g->n; v++){
      if(atualizado[prnt(g,v)] && !atualizado[v]){
	x = g->arvore[v];
	if(x->ini == v)
	  g->y[v] = x->cost + g->y[g->pais[v]];
	else
	  g->y[v] = g->y[g->pais[v]] - x->cost;
	atualizado[v] = 1;
	falta--;
      }
    }
    i++;
  }
  free(atualizado);
}

void network_simplex(Graph g){
  Arc x;
  while( (x = entry_arc(g)) != NULL ){
    update_prnt(g,x);
    update_y(g);
    update_depth(g);
  }
}

float net_cost(Graph g){
  Arc x;
  Vertex v;
  float cost = 0;
  for(v = 0; v < g->n; v++){
    x = g->arvore[v];
    if( x != NULL && x->fluxo != 0) cost += x->cost;
  }
  return cost*g->demanda;
}
