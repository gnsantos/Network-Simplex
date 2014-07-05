#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"
#include "lerEntrada.h"
#include "simplex.h"
#include "inicializeSimplex.h"

int main(int argc, char** argv){
  char *entrada = argv[1];
  Graph g;
  int viavel;
  float custo;
  if(argc < 2){
    puts("Entre um arquivo com a configuracao da rede.");
    puts("USO: ./ep <arquivo_de_entrada>");
    exit(-1);
  }
  g = le_entrada(entrada);
  acha_solucao_inicial(g);
  viavel = checa_viabilidade(g);
  if( !viavel ){
    puts("O problema entrado e inviavel e nao possui solucao.");
  }
  else{
    network_simplex(g);
    custo = net_cost(g);
    puts("O estado final do grafo eh: ");
    show_graph(g);
    puts("A arvore associada a solucao otima eh:");
    show_tree(g);
    printf("O custo minimo do transporte eh: %lf\n", custo);
  }
  return 0;
}


