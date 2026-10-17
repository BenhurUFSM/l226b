#ifndef _REDE_H_
#define _REDE_H_

#include <stdbool.h>

// Uma rede neural simples
//
// A rede é um grafo de nós (neurônios) interligados por arestas (sinapses).
// Alguns desses nós são nós de entrada, outros são nós de saída; os demais
//   são nós intermediários.
// Cada nó tem um valor associado, que pode ser calculado pela rede ou ser
//   um valor de entrada (no caso dos nós de entrada).
// O último nó de entrada é especial, e tem sempre o valor -1. Logo, o número
//   de entradas da rede deve ser um além do número de entradas fornecidas pelo
//   modelo.
// Cada aresta tem um valor associado, chamado peso. As arestas são direcionais,
//   ligam um nó de origem a um nó de destino (e não vice-versa).
// O valor de um nó é calculado pelo somatório do valor de cada nó antecessor
//   multiplicado pelo peso da aresta que o liga a esse antecessor.
// Para uniformizar esses valores, é usada uma função "sigmoide", que limita
//   o valor calculado ao intervalo (0,1).
// Os valores calculados nos nós de saída são os valores de saída da rede, que
//   são usados como valores de entrada no modelo.
//
// O cálculo do valor de um nó é dependente dos valores dos nós que o precedem,
//   logo necessitamos de uma ordem topológica para realizar esses cálculos.
// Para facilitar a identificação dos nós de entrada e saída do grafo, os nós
//   de entrada são os com menores números de identificação, seguidos pelos
//   nós de saída e então pelos nós intermediários.
// Os valores dos primeiros nós de entrada são fornecidos pelo modelo, a última
//   entrada sempre vale -1.
//
// Tem duas formas de uso das redes.
// A primeira é para comandar a execução do jogo, à partir de uma rede
//   pré-existente. Nesse caso, os nós e arestas da rede, assim como seus
//   pesos estão em um arquivo, que é lido na inicialização. Essa é a parte II
//   do trabalho.
// A segunda é para gerar essas ligações e pesos, em um processo evolutivo
//   de treinamento, em que várias redes são usadas para executar várias
//   partidas, e as que conseguem pontuação maior sobrevivem e se tenta
//   aprimorá-las de forma aleatória. As melhores são registradas em arquivos.
//   Essa é a parte III do trabalho.

typedef struct rede *Rede;

// as estruturas de neurônio e sinapse estão abertas aqui para simplificar
//   a implementação do treinador de rede. Essas estruturas não devem ser
//   acessadas diretamente a não ser pelo treinador.

// Um neurônio tem uma identificação e um valor.
typedef struct {
  int id;
  float valor;
} neurônio;

// Uma sinapse contém um peso usado no cálculo do valor do neurônio de chegada,
//   e pode ser desabilitada
typedef struct {
  float peso;
  bool habilitada;
} sinapse;


// cria uma rede neural a partir de um arquivo
Rede rede_cria(char *nome);

// cria uma rede vazia
Rede rede_cria_vazia(int num_entradas, int num_saídas);

// libera a memória ocupada por uma rede neural
void rede_destrói(Rede self);

// retorna o número de neurônios na rede
int rede_num_neurônios(Rede self);

// retorna o número de entradas na rede
int rede_num_entradas(Rede self);

// retorna o número de saídas na rede
int rede_num_saídas(Rede self);

// insere um novo neurônio na rede, retorna sua posição
// o conteúdo do neurônio é copiado para a rede
int rede_insere_neurônio(Rede self, neurônio neuro);

// retorna ponteiro para o neurônio na posição 'pos'
neurônio *rede_neurônio(Rede self, int pos);

// retorna ponteiro para o 'pos'-ésimo neurônio de entrada
// (é o próprio neurônio na posição 'pos' da rede)
neurônio *rede_neurônio_entrada(Rede self, int pos);

// retorna ponteiro para o 'pos'-ésimo neurônio de saída
// (é o neurônio na posição 'pos+n_entradas' da rede)
neurônio *rede_neurônio_saída(Rede self, int pos);

// remove o neurônio na posição 'pos'
// remove também as sinapses conectadas à esse neurônio
// a posição dos neurônios seguintes é alterada, de forma a não ter
//   posição vazia
void rede_remove_neurônio(Rede self, int pos);

// retorna ponteiro para a sinapse entre os neurônios nas posições 'pos_org' e 'pos_dest'
sinapse *rede_sinapse(Rede self, int pos_org, int pos_dest);

// insere a sinapse 'sina' entre os neurônios em 'pos_origem' e 'pos_destino'
// não insere se for formar ciclo
// retorna true se inseriu
bool rede_insere_sinapse(Rede self, int pos_origem, int pos_destino, sinapse sina);

// remove a sinapse entre os neurônios em 'pos_origem' e 'pos_destino'
void rede_remove_sinapse(Rede self, int pos_origem, int pos_destino);

// recalcula a ordem topológica de cálculo dos nós
void rede_calcula_ordem(Rede rede);

// recalcula os valores dos nós da rede, à partir dos valores fornecidos
//   para as entradas; copia os valores dos nós de saída para o vetor 'saídas'
// a ordem de cálculo deve obedecer a última ordem obtida por 'rede_calcula_ordem'
void rede_calcula(Rede rede, float *entradas, float *saídas);

// inicia um percurso pelas sinapses que partem do neurônio em pos_org
void rede_sinapses_que_partem(Rede self, int pos_org);

// retorna um ponteiro para a próxima sinapse do percurso
// coloca em '*pos_vizinho' a posição do nó vizinho
// retorna NULL e não altera '*pos_vizinho' se o percurso já terminou
sinapse *rede_próxima_sinapse(Rede self, int *pos_vizinho);

#endif // _REDE_H_
