// lista_ev_n.c

// implementação de lista (descrita em lista.h)
// usando encadeamento em vetores
// com índice do último elemento e número de elementos no descritor

#include "lista.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// capacidade da lista
#define N 200

struct lista {
  int primeiro;     // índice do vetor onde está o primeiro dado da lista
  int último;       // índice do vetor onde está o último dado da lista
  int tamanho;      // número de itens na lista
  int livre;        // índice do vetor onde está a primeira posição livre
  dado_t dados[N];  // vetor que contém os dados da lista
  int próximo[N];   // vetor que contém os encadeamentos
};

// prox[i] contém o índice do vetor onde está o dado que segue o
//         dado que está no índice i do vetor

// testa a saúde da lista
// aborta o programa caso detecte algum problema
static void l_ok(Lista l)
{
  assert(l != NULL);
  bool ocupada[N] = {false};
  int n = 0;
  // percorre os elementos da lista, marcando em ocupada[] as posições ocupadas
  for (int i = l->primeiro; i != -1; i = l->próximo[i]) {
    assert(i >= 0 && i < N);     // o índice deve ser válido
    assert(ocupada[i] == false); // a posição não pode ter sido marcada
    ocupada[i] = true;
    n++;
  }
  assert(n == l->tamanho);
  if (n == 0) {
    assert(l->último == -1);
  } else {
    assert(ocupada[l->último]);
    assert(l->próximo[l->último] == -1);
  }
  // percorre os elementos livres, marcando as posições ocupadas
  for (int i = l->livre; i != -1; i = l->próximo[i]) {
    assert(i >= 0 && i < N);     // o índice deve ser válido
    assert(ocupada[i] == false); // a posição não pode ter sido marcada
    ocupada[i] = true;
    n++;
  }
  // todas as posições devem estar livres ou ocupadas
  assert(n == N);
}

// adiciona o índice i na lista de livres
static void l_libera_índice(Lista l, int i)
{
  l->próximo[i] = l->livre;
  l->livre = i;
}

// encontra um índice livre, e retira da lista de livres
static int l_índice_livre(Lista l)
{
  // o teste de cheia pode ser l->livre == -1
  assert(!l_cheia(l)); // poderia realocar...
  // pega o índice do primeiro da lista de livres
  int ind = l->livre;
  // o primeiro livre passa a ser o seguinte
  l->livre = l->próximo[ind];
  return ind;
}

// inicializa a lista (já alocada)
static void l_init(Lista l)
{
  // não tem primeiro elemento, nem último
  l->primeiro = -1;
  l->último = -1;
  l->tamanho = 0;

  // todas as posições do vetor são encadeadas na lista de livres
  l->livre = -1;
  for (int i = 0; i < N - 1; i++) l_libera_índice(l, i);
}

Lista l_cria()
{
  Lista l = malloc(sizeof(struct lista));
  assert(l != NULL);
  l_init(l);
  l_ok(l);
  return l;
}

void l_destroi(Lista l)
{
  l_ok(l);
  free(l);
}

int l_tam(Lista l)
{
  l_ok(l);
  return l->tamanho;
}

bool l_cheia(Lista l)
{
  l_ok(l);
  return l->livre == -1;
}

bool l_vazia(Lista l)
{
  l_ok(l);
  return l->primeiro == -1;
}

void l_imprime(Lista l)
{
  l_ok(l);
  printf("[ ");
  int i = l->primeiro;
  while (i != -1) {
    printf("%d ", l->dados[i]);
    i = l->próximo[i];
  }
  printf("]\n");
}

// insere o dado d no início da lista l
void l_insere_inicio(Lista l, dado_t d)
{
  l_ok(l);
  // acha um índice livre
  int novo = l_índice_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // o dado que segue o novo primeiro dado é o antigo primeiro
  // se a lista estava vazia, l->prim é -1, que é o que queremos
  //   como prox
  l->próximo[novo] = l->primeiro;
  // atualiza a nova primeira posição (e a última, se for o caso)
  l->primeiro = novo;
  if (l->tamanho == 0) l->último = novo;
  l->tamanho++;
  l_ok(l);
}

// insere o dado d no final da lista l
void l_insere_fim(Lista l, dado_t d)
{
  // se a lista estiver vazia, é mais fácil inserir no início...
  if (l_vazia(l)) { l_insere_inicio(l, d); return; }
  // acha um índice livre
  int novo = l_índice_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // ele é o novo último
  l->próximo[novo] = -1;
  l->próximo[l->último] = novo;
  l->último = novo;
  l->tamanho++;
  l_ok(l);
}

// retorna o índice do vetor que contém o elemento da lista
//   que tá na posição pos
static int l_acha_indice_pos(Lista l, int pos)
{
  assert(pos > 0 && pos <= l->tamanho);
  if (pos == l->tamanho - 1) return l->último;
  int i = l->primeiro;
  int p = 0;
  while (p < pos) {
    assert(i != -1);
    i = l->próximo[i];
    p++;
  }
  assert(i != -1);
  return i;
}

// insere o dado d na lista l, de forma que ele fique na posição p
void l_insere_pos(Lista l, dado_t d, int p)
{
  // se a inserção for no início ou no fim, temos uma função pronta...
  if (p == 0) { l_insere_inicio(l, d); return; }
  if (p == l->tamanho - 1) { l_insere_inicio(l, d); return; }
  // acha o índice do elemento anterior ao inserido
  int anterior = l_acha_indice_pos(l, p - 1);
  // o índice do dado seguinte
  int seguinte = l->próximo[anterior];
  // acha um índice livre
  int novo = l_índice_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // o dado novo fica após o anterior
  l->próximo[anterior] = novo;
  // depois do novo fica o seguinte
  l->próximo[novo] = seguinte;
  // temos mais um elemento na lista
  l->tamanho++;
}

// retorna o dado no início da lista
dado_t l_dado_inicio(Lista l)
{
  assert(!l_vazia(l));
  return l->dados[l->primeiro];
}

// retorna o dado no final da lista
dado_t l_dado_fim(Lista l)
{
  assert(!l_vazia(l));
  return l->dados[l->último];
}

// retorna o dado na posição pos da lista
dado_t l_dado_pos(Lista l, int pos)
{
  l_ok(l);
  return l->dados[l_acha_indice_pos(l, pos)];
}

// remove e retorna o dado no início da lista
dado_t l_remove_inicio(Lista l)
{
  assert(!l_vazia(l));
  //...
  return 0;
}

// remove e retorna o dado no final da lista
dado_t l_remove_fim(Lista l)
{
  assert(!l_vazia(l));
  //...
  return 0;
}

// remove e retorna o dado na posição pos da lista
dado_t l_remove_pos(Lista l, int pos)
{
  assert(!l_vazia(l));
  //...
  return 0;
}

