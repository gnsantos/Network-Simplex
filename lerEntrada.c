/****************************************************************
EXERCICIO-PROGRAMA DE MAC0315 - PROGRAMACAO LINEAR
PRIMEIRO SEMESTRE DE 2014
ALUNO: GERVASIO PROTASIO DOS SANTOS NETO
NUSP: 7990996
PROFESSOR: WALTER MASCARENHAS

arquivo: lerEntrada.c
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

/*recebe o nome do arquivo que contem a definicao do probelma e constroi o grafo correspondente*/
Graph le_entrada(char* nome_entrada){
  FILE* input = fopen(nome_entrada, "r"); /*abre arquivo de entrada*/
  int n; /*numero de vertices*/
  Vertex origem, destino, u, v;
  Graph g;
  double cost, demanda;
  if(input == NULL){
    puts("O arquivo passado como argumento nao exite!");
    exit(-2);
  }
  
  fscanf(input, "%d", &n);/*le o numero de vertices*/
  fscanf(input, "%d", &origem); /*le o vertice de origem*/
  fscanf(input, "%d", &destino); /*le o vertice de destino*/
  fscanf(input, "%lf", &demanda); /*le a quantidade de produto escoada*/
  
/*inicializa a rede*/  
  g = init_graph(n, origem, destino, demanda);

  while(fscanf(input, "%d %d %lf", &u,&v,&cost) != EOF){
    /*equanto o arquivo nao acabar, le vertice_origem, vertice_destino e custo_aresta*/
    add_arc(g,u,v,cost,0); /*adiciona arco lido na rede*/
  }
  fclose(input);

  printf("A configuracao presente em %s foi lida com sucesso.\n", nome_entrada);

  return g; /*retona o grafo descrito pela entrada*/
}

