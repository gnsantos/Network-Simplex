#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"
#include "arvore.h"
#include "lerEntrada.h"
#include "simplex.h"
#include "inicializeSimplex.h"

int main(int argc, char** argv){
  char *entrada = argv[1];
  Graph g;
  Arvore t;
  int viavel;
  double custo;
  /*verificamos se foi passado um arquivo de entrada com a configuracao da rede.*/
  if(argc < 2){
    puts("Entre um arquivo com a configuracao da rede.");
    puts("USO: ./ep <arquivo_de_entrada>");
    exit(-1);
  }
  g = le_entrada(entrada); /*constroi o grafo de acordo com as especificacoes da entrada*/
  t = acha_solucao_inicial(g); /*acha uma solucao viavel resolvendo o problema auxiliar*/
  viavel = checa_viabilidade(t); /*checa se a solucao encontrada no problema auxiliar eh uma
				  solucao viavel do problema que queremos resolver*/
  if( !viavel ){
    puts("O problema entrado e inviavel e nao possui solucao.");
  }
  /*Se a solucao foi viavel, executamos o simplex de rede partindo dela, calculamos o custo do transporte
   e imprimimos o grafo, o vetor de pais, e os arcos da arvore otima.*/
  else{
    network_simplex(g,t);
    custo = net_cost(t,g);
    puts("O estado final do grafo eh: ");
    show_graph(g);
    puts("A arvore associada a solucao otima eh:");
    show_tree(t);
    printf("O custo minimo do transporte eh: %f\n", custo);
  }
  return 0;
}


