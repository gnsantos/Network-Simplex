/****************************************************************
EXERCICIO-PROGRAMA DE MAC0315 - PROGRAMACAO LINEAR
PRIMEIRO SEMESTRE DE 2014
ALUNO: GERVASIO PROTASIO DOS SANTOS NETO
NUSP: 7990996
PROFESSOR: WALTER MASCARENHAS

arquivo: ep.c
 ****************************************************************/
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
  char *saida = argv[2];
  FILE *output;
  Graph g;
  Arvore t;
  int viavel;
  double custo;
  /*verificamos se foi passado um arquivo de entrada com a configuracao da rede.*/
  if(argc < 3){
    puts("Entre um arquivo com a configuracao da rede e um arquivo para a escrita da saida. Arquivo de saida nao criado.");
    puts("USO: ./ep <arquivo_de_entrada> <arquivo_de_saida>");
    exit(-1);
  }
  g = le_entrada(entrada); /*constroi o grafo de acordo com as especificacoes da entrada*/
  t = acha_solucao_inicial(g); /*acha uma solucao viavel resolvendo o problema auxiliar*/
  viavel = checa_viabilidade(t); /*checa se a solucao encontrada no problema auxiliar eh uma
				  solucao viavel do problema que queremos resolver*/
  if( !viavel ){
    puts("O problema entrado e inviavel e nao possui solucao. Nao foi criado o arquivo de saida.");
  }
  /*Se a solucao foi viavel, executamos o simplex de rede partindo dela, calculamos o custo do transporte
   e imprimimos o grafo, o vetor de pais, e os arcos da arvore otima.*/
  else{
    network_simplex(g,t);
    custo = net_cost(t,g);
    output = fopen(saida, "w"); /*abre o arquivo da saida pra escrita*/
    fputs("O estado final do grafo eh: \n",output);
    show_graph(g,output);
    fputs("A arvore associada a solucao otima eh:\n",output);
    show_tree(t,output);
    fprintf(output,"O custo minimo do transporte eh: %f\n", custo);
    printf("A saida do programa foi escrita com sucesso em %s.\n", saida);
    fclose(output); /*fecha o arquivo de saida*/
  }
  free(g); /*desaloca memoria do grafo*/
  free(t); /*desaloca memoria da arvore*/
  return 0;
}


