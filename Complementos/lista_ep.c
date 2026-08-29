// lista_ep.c

// implementação de lista (descrita em lista.h)
// usando encadeamento de nós com alocação individual

#include "lista.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// um nó da lista, contendo o dado e o índice do próximo nó
typedef struct nó nó;
struct nó {
  dado_t dado;      // o dado armazenado neste nó
  nó *próximo;      // ponteiro para o nó que segue este
};

struct lista {
  nó *primeiro;     // ponteiro para o nó onde está o primeiro dado da lista
  nó *último;       // ponteiro para o nó onde está o último dado da lista
  int tamanho;      // número de itens na lista
};

// testa a saúde da lista
// aborta o programa caso detecte algum problema
static void l_ok(Lista l)
{
  assert(l != NULL);
  // percorre os elementos da lista, contando os elementos
  int tam = 0;
  for (nó *n = l->primeiro; n != NULL; n = n->próximo) {
    tam++;
  }
  assert(tam == l->tamanho);
  if (tam == 0) assert(l->último == NULL);
}

// recicla o nó
static void l_libera_nó(Lista l, nó *n)
{
  // se houver algo a fazer com o dado antes de detonar com ele..
  free(n);
}

// encontra memória para um novo nó
static nó *l_aloca_nó(Lista l)
{
  nó *n = malloc(sizeof(*n));
  assert(n != NULL);  // poderia tentar tratar esse problema, ou devolver NULL
  return n;
}

// inicializa a lista (já alocada)
static void l_init(Lista l)
{
  // não tem primeiro elemento, nem último
  l->primeiro = NULL;
  l->último = NULL;
  l->tamanho = 0;
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
  // libera todos os nós da lista
  nó *n = l->primeiro;
  while (n != NULL) {
    nó *próximo = n->próximo; // n vai ser liberado, não se pode acessar depois
    l_libera_nó(l, n);
    n = próximo;
  }
  free(l);
}

int l_tam(Lista l)
{
  l_ok(l);
  return l->tamanho;
}

bool l_cheia(Lista l)
{
  // não tem lista cheia! se encher a memória, mata o programa...
  l_ok(l);
  return false;
}

bool l_vazia(Lista l)
{
  l_ok(l);
  return l->primeiro == NULL; // poderia ser l->tamanho == 0
}

void l_imprime(Lista l)
{
  l_ok(l);
  printf("[ ");
  for (nó *n = l->primeiro; n != NULL; n = n->próximo) {
    printf("%d ", n->dado);
  }
  printf("]\n");
}

// insere o dado d no início da lista l
void l_insere_inicio(Lista l, dado_t d)
{
  l_ok(l);
  // acha um índice livre
  nó *novo = l_aloca_nó(l);
  // coloca o dado no vetor
  novo->dado = d;
  // o dado que segue o novo primeiro dado é o antigo primeiro
  // se a lista estava vazia, l->prim é NULL, que é o que queremos
  //   como próximo
  novo->próximo = l->primeiro;
  // atualiza o novo primeiro nó (e o último, se for o caso)
  l->primeiro = novo;
  if (l->tamanho == 0) l->último = novo;
  l->tamanho++;
  l_ok(l);
}

// insere o dado d no final da lista l
void l_insere_fim(Lista l, dado_t d)
{
  // se a lista estiver vazia, é mais fácil inserir no início...
  if (l_vazia(l)) {
    l_insere_inicio(l, d);
    return;
  }
  // acha um índice livre
  nó *novo = l_aloca_nó(l);
  // coloca o dado no vetor
  novo->dado = d;
  // ele é o novo último
  novo->próximo = NULL;
  l->último->próximo = novo;
  l->último = novo;
  l->tamanho++;
  l_ok(l);
}

// retorna o nó que contém o elemento da lista
//   que está na posição pos
static nó *l_nó_na_posição(Lista l, int pos)
{
  assert(pos > 0 && pos <= l->tamanho);
  if (pos == l->tamanho - 1) return l->último;
  nó *n = l->primeiro;
  int p = 0;
  while (p < pos) {
    assert(n != NULL);
    n = n->próximo;
    p++;
  }
  assert(n != NULL);
  return n;
}

// insere o dado d na lista l, de forma que ele fique na posição p
void l_insere_pos(Lista l, dado_t d, int p)
{
  // se a inserção for no início ou no fim, temos uma função pronta...
  if (p == 0) { l_insere_inicio(l, d); return; }
  if (p == l->tamanho - 1) { l_insere_inicio(l, d); return; }
  // acha o índice do elemento anterior ao inserido
  nó *anterior = l_nó_na_posição(l, p - 1);
  // o índice do dado seguinte
  nó *seguinte = anterior->próximo;
  // acha um índice livre
  nó *novo = l_aloca_nó(l);
  // coloca o dado no vetor
  novo->dado = d;
  // o dado novo fica após o anterior
  anterior->próximo = novo;
  // depois do novo fica o seguinte
  novo->próximo = seguinte;
  // temos mais um elemento na lista
  l->tamanho++;
}

// retorna o dado no início da lista
dado_t l_dado_inicio(Lista l)
{
  assert(!l_vazia(l));
  return l->primeiro->dado;
}

// retorna o dado no final da lista
dado_t l_dado_fim(Lista l)
{
  assert(!l_vazia(l));
  return l->último->dado;
}

// retorna o dado na posição pos da lista
dado_t l_dado_pos(Lista l, int pos)
{
  l_ok(l);
  return l_nó_na_posição(l, pos)->dado;
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

