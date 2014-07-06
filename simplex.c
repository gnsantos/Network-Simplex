#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"
#include "arvore.h"

#define null NULL

/*escolhe um arco para entrar na base*/
Arc entry_arc(Graph g, Arvore t){
Vertex v;
Arc e, x = null;
list l;
double inf = 1.0/0.0;
 for(v = 0; v < g->n; v++){
   for(l = g->adj[v]; l != null; l = l->next){
     e = l->arco;
     /*Um arco (i,j) eh candidato a entrar na base se y[i] - y[j] > custo de (i,j). Essa formula eh adaptado do Chvatal.*/
     if((t->y[e->ini] -  t->y[e->dest] > e->cost) && !e->inTree && e->cost != inf){ 
       x = e;
       return x; /*devolve o candidato a entrar na base*/
     }
   }
 }
 return x; /*se nao encontrar nenhum candidato valido, retorn null e sabemo que acabamos.*/
}

/*procura o caminho entre u e v na arvore representada, onde u e v sao as extremidades do arco
  de entrada entry.
  Na verdade, essa funcao serve para determinar o arco que deve sair. Os vertices u e v sao as
  extremidades do arco que entra. Enquanto achamos o caminho, vamos mantendo regsitro do arco
  reverso com menor fluxo; ele sera o retornado pela funcao e removido da arvore.*/
Arc tree_path(Arvore g, Arc entry, Vertex** pshow, int* s){
  Arc x, resp = NULL, xv, xu;
  Vertex aux, u = entry->ini, v = entry->dest;
  /*para teste de codigo, foram mantidas variaveis para armazenar explicitamente o caminho*/
  Vertex *path; /*caminho de u a v*/
  Vertex *upath = malloc(sizeof(Vertex)*g->n);/*camino de u ate ancestral*/
  Vertex *vpath = malloc(sizeof(Vertex)*g->n);/*caminho de v ate acestral*/
  int i = 0, j = 0;
  double minflow = 1.0/0.0;
  list reversos = null; /*lista dos arcos reversos, cujo fluxo sera diminuido*/
  list diretos = null; /*lista dos arcos diretos, cujo fluxo sera aumentado*/
  list new, newv, newu;

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
      new = malloc(sizeof(list));
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
      new = malloc(sizeof(list));
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
    newv = malloc(sizeof(list));
    newu = malloc(sizeof(list));
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
  
  /*diminui o fluxo dos arcos reversos*/
  while(reversos != null){
    reversos->arco->fluxo -= minflow;
    reversos = reversos->next;
  }
  /*aumenta o fluxo dos arcos com a mesma orientacao de entry*/
  while(diretos != null){
    diretos->arco->fluxo += minflow;
    diretos = diretos->next;
  }
  entry->fluxo = minflow; /*atualiza o fluxo do arco de entrada*/

  /*remove da arvore o arco de saida*/
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

/*
  Atualiza o vetor de pais e o vetor de arcos da arvore. Recebe como entrada uma arvore antiga
  e um arco que esta sendo adicionado a ela, entao descobre o caminho entre suas extremidades,
  qual eh o arco de saida (que eh removido da arvore na funcao tree_path) e atualizamos o vetor
  de pais conforme o necessario.
*/
void update_prnt(Arvore g, Arc entry){
  Arc leaving, auxiliar;
  Vertex *path, e1, e2, f1, f2, root;
  int size = 0, i, beforeroot = 1, posroot = pow(2,30), posf2 = 0, posf1 = 0;

  leaving = tree_path(g,entry,&path,&size); /*arco de saida*/
  
  for(root = 0; root < g->n; root++) /*encontra a raiz*/
    if(prnt(g,root) == root)
      break;
  
  /*Setamos f2 para ser a ponta mais profunda do vertice de saida
   e e2 para ser a ponta mais profund do vertice de chegada.*/
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
  
  /*vamos comecar a seguir o caminho entre os pontos. Por comodidade,
   assumimos que e2 eh o primeiro vertice desse caminho. Se esse nao
  for o caso, invertemos o caminho.*/
  
  if(path[0] == e1)
    path = reverse_path(path, size);
  
  /*descobrimos a posicao de f1, f2 e da raiz no caminho (se a raiz nao
   esta no caminho, sua posicao eh "infinito".*/
  for(i = 0; i < size; i++){
    if(path[i] == root) posroot = i;
    if(path[i] == f2) posf2 = i;
    if(path[i] == f1) posf1 = i;
  }
  
  /*tratamos de formas diferentes quando f2 ocorre depois da raiz/f1.*/
  if(posf2 > posroot || posf2 > posf1) beforeroot = 0;
  
  /*se f2 ocorre antes, fazemos de e2 pai de e1, e vamos voltando no caminho 
   a partir de f2 fazendo de cada vertice, filho do anterior no caminho*/
  if(!beforeroot){
    set_parent(g, e2, e1, entry);
    for(i = posf2; path[i] != e1; i++){
      auxiliar = is_tree_arc(g,path[i], path[i+1]);
      if(auxiliar == null) 
	auxiliar = is_tree_arc(g,path[i+1],path[i]);
      set_parent(g, path[i+1],path[i], auxiliar);
    } 
  }
  /*caso contrario, fazemo de e1 pai de e2 e percorremos desde o comeco o 
    caminho de e2 para e1 fazendo de cada vertice pai do proximo.*/
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

/*
  Atualizamos o vetor de profundidades baseando-nos no fato
  de que quando o vetor esta correto temos que:
  
  profundidade[i] = 1 + profundidae[pai[i]]
*/

void update_depth(Arvore g){
  Vertex v;
  int i = 0;
  int updated = 1; /*guarda se alguma profundidade foi alterada*/

  /*Fazemos (no maximo) n passagens pelo vetor em busca de vertices
   cujas profundidades precisam ser ajustadas. Se mudarmos alguma
  profundiade setamos updated para 1. Se updated for 0, eh porque
  todas as profundidades estam corretas e podemos parar.

  Sabemos que ao final teremos todas as profundiades certas pois,
  no pior caso, a cada iteracao do loop while, atualizamos uma
  profundidade para seu valor correto.*/
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

/*atualiza o vetor de potenciais*/
void update_y(Arvore g){
  Vertex root, v;
  Arc x;
  int i;
  /*atualizado eh um vetor auxiliar. atualizado[i] = 1 se o vertice i ja teve seu
   potencial atualizado e eh 0 caso ainda precisemos atualizar y[i].*/
  int *atualizado = malloc(sizeof(int)*g->n);
  /*falta guara quantos vertices ainda precisamos atualizar*/
  int falta = g->n;
  for(i = 0; i < g->n; i++)
    atualizado[i] = 0; /*inicialmente, precisamos atualizar todos os vertices*/
  
  for(root = 0; root < g->n; root++) /*achamos a raiz da arvore*/
    if(prnt(g,root) == root)
      break;
  
  g->y[root] = 0; /*setamos arbitrariamente a raiz como tendo potencial 0*/
  atualizado[root] = 1; /*marcamos que ja atualizamos a raiz*/
  falta--; /*falta um vertice a menos para atualizar*/

  i = 0;
  
  /*
    No loop abaixo, percorremos (no maximo) n vezes o vetor em busca de ver-
    tices ainda nao atualizados, e adequamos seu potencial de acordo com o
    potencial de seu pai. Sabemos que ao final todo vertice estara atualizado,
    pois no pior caso, atualizamos pelo menos um vertice a cada iteracao do 
    loop while (que tem n iteracoes).

    Se verificamos que o loop while teve menos de n iteracoes, mas ja atuali-
    zamos todos os potenciais, encerramos o loop.

    para um arco (i,j) na arvore, temos que: y[i] - y[j] = custo de (i,j)
   */

  while(i < g->n && falta > 0){
    for(v = 0; v < g->n; v++){
      /*se o pai de um vertice foi atualizado e ele nao, precisamos atualiza-lo*/
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

/*Roda o simplex para redes. Recebemos um Graph g com a configuraca da rede e uma Arvore t,
  que contem, inicialmente uma solucao viavel. Temos que ao final da funcao, a Arvore t ar-
  mazenara a arvore associada a solucao otima do problema.
  
  Temos que a cada iteracao, procuramos na rede uma arco que eh candidato a entrar na base:
  se o retorno for null, quer dizer que nao temos candidatos e a solucao atual eh otima.

  Caso haja um arco candidato a entrar na base entao:
  
  i) chamamos update_prnt, que chama tree_path para encontrar o arco de saida e o tirar da base.
  Entao, update_prnt prossegue para atualizar a solucao, que eh mantida na forma da Arvore t.

  ii) chamamos update_y para atualizar os potenciais da rede. Isso nos permitira decidir, no
  inicio da proxima iteracao, se a Arvore t eh associada a uma solucao otima ou nao.

  iii) chamamos update_depth para atualizar a profundidade de todos na arvore.

*/
void network_simplex(Graph g, Arvore t){
  Arc x;
  while( (x = entry_arc(g,t)) != NULL ){
    update_prnt(t,x);
    update_y(t);
    update_depth(t);
  }
}

/*Devolve o custo de transporte por um rede r, com uma solucao viavel g*/
double net_cost(Arvore g, Graph r){
  Arc x;
  Vertex v;
  float cost = 0;
  for(v = 0; v < g->n; v++){
    x = g->arvore[v];
    if( x != NULL && x->fluxo != 0) cost += x->cost;
  }
  return cost*r->demanda;
}
