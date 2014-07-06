/****************************************************************
EXERCICIO-PROGRAMA DE MAC0315 - PROGRAMACAO LINEAR
PRIMEIRO SEMESTRE DE 2014
ALUNO: GERVASIO PROTASIO DOS SANTOS NETO
NUSP: 7990996
PROFESSOR: WALTER MASCARENHAS

arquivo: grafo.h
 ****************************************************************/

#ifndef H_GRAFO_DEFINE
#define H_GRAFO_DEFINE

/************************ DEFINICOES DE ESTRUTURAS ************************************/

typedef int Vertex; /*vertices sao representados com inteiros*/

struct arc{ /*representacao de um arco*/
  Vertex ini;       /*vertice de inicio do arco*/
  Vertex dest;      /*vertice de chegada do arco*/ 
  double cost;      /*custo do arco*/
  double fluxo;     /*fluxo passando pelo arco*/
  int inTree;       /*valor booleano: 1 se o arco esta na arvore (base), 0 caso contrario*/
};

typedef struct arc *Arc; /*Arc e um ponteiro para um arco*/

struct list_member{ /*lista de adjacencia de um vertice*/
  Arc arco; /*cada membro da lista e um arco*/
  struct list_member *next; /*e tem um ponteiro para o proximo arco*/
};

typedef struct list_member *list; /*definindo list como um ponteiro para list_member*/

struct grafo{
  int m; /*numero de arcos*/
  int n; /*numero de vertices*/
  list* adj; /*vetor de listas de adjacencia*/
  Vertex origem; /*guarda qual eh o no de origem na rede*/
  Vertex destino; /*guarda qual eh o no de desino na rede*/
  double demanda; /*armazena a quantidade de produto escoado pela rede*/
};

typedef struct grafo* Graph;

/******************* FUNCOES PARA INICIALIZACAO E MANIPULACAO DE ESTRUTURAS ********************/


Graph init_graph(int,Vertex,Vertex,double); /*inicializa um grafo, alocando o necessario*/
Arc new_arc(Vertex,Vertex,double,double); /*cria um novo arco*/
Arc is_arc(Graph, Vertex, Vertex); /*verifica se um arco (nao artificial) existe*/
Arc add_arc(Graph, Vertex, Vertex, double, double); /*adiciona um novo arco ao grafo*/
void show_graph(Graph,FILE*); /*imprime uma descricao literal do grafo*/

#endif
