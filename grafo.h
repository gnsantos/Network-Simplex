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
  Vertex *pais; 
  int *profundidade;
/*Vetor de pais: eh usado para manter a estrutura de uma arvore geradora
  Vetor de profundidade: mantem quantos nos separam um vertice da raiz
  Por exemplo, a seguinte arvore:
                       1
                       |
                       |
		 ------5-----4
		 |     |      \
                 |     |       \
		 |     |        \
		 3     2         0
tem o seguinte vetor de pais:
      i: 0 1 2 3 4 5
pais[i]: 4 1 5 5 5 1
(note que a raiz eh o vertice v com pais[v] = v;
e de profundidade:
              i: 0 1 2 3 4 5
profundidade[i]: 4 1 3 3 3 1
*/
  Arc *arvore; /*Um vetor de arcos: o arco arvore[i] eh o arco liga pais[i] e i*/
  double *y; /*vetor de potenciais.*/
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
void set_parent(Graph, Vertex, Vertex, Arc); /*define o pai de um vertice na arvore e qual o arco que o liga*/
Vertex prnt(Graph, Vertex); /*devolve o pai de um vertice*/
int depth(Graph, Vertex); /*devolve a profundidade de um vertice*/
Arc entry_arc(Graph); /*escolhe um arco para entrar na base*/
Arc tree_path(Graph, Arc, Vertex**,int*); /*encontra o caminho entre vertices e escolhe um arco para sair da base*/
void update_prnt(Graph, Arc); /*atualiza o vetor de pais e de arcos da arvore*/
void update_depth(Graph); /*atualiza o vetor de profundidades*/
void update_y(Graph); /*atualiza o vetor de potenciais*/
void show_graph(Graph); /*imprime uma descricao literal do grafo*/
void show_tree(Graph); /*imprime um descricao da arvore e de seus arcos*/
void show_path(Graph, Arc); /*mostra o caminho entre as extremidades de um arco*/

#endif
