// lista.c

// implementação de lista (descrita em lista.h)
// usando encadeamento em vetores

#include "lista.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// capacidade da lista
#define N 200

struct lista {
  int prim;         // índice do vetor onde está o primeiro dado da lista
  int livre;        // índice do vetor onde está o primeira pos. livre
  dado_t dados[N];  // vetor que contém os dados da lista
  int prox[N];      // vetor que contém os encadeamentos
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
  // percorre os elementos da lista, marcando em t[] as posições ocupadas
  for (int i = l->prim; i != -1; i = l->prox[i]) {
    assert(i >= 0 && i < N);     // o índice deve ser válido
    assert(ocupada[i] == false); // a posição não pode ter sido marcada
    ocupada[i] = true;
    n++;
  }
  // percorre os elementos livres, marcando em t[] as posições ocupadas
  for (int i = l->livre; i != -1; i = l->prox[i]) {
    assert(i >= 0 && i < N);     // o índice deve ser válido
    assert(ocupada[i] == false); // a posição não pode ter sido marcada
    ocupada[i] = true;
    n++;
  }
  // todas as posições devem estar livres ou ocupadas
  assert(n == N);
}


// inicializa a lista (já alocada)
static void l_init(Lista l)
{
  // não tem primeiro elemento
  l->prim = -1;

  // todas as posições do vetor são encadeadas na lista de livres
  l->livre = 0;
  for (int i = 0; i < N - 1; i++) l->prox[i] = i + 1;
  l->prox[N - 1] = -1;
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
  int n = 0;
  int i = l->prim;
  while (i != -1) {
    i = l->prox[i];
    n++;
  }
  return n;
}

bool l_cheia(Lista l)
{
  l_ok(l);
  return l->livre == -1;
}

bool l_vazia(Lista l)
{
  l_ok(l);
  return l->prim == -1;
}

void l_imprime(Lista l)
{
  l_ok(l);
  printf("[ ");
  int i = l->prim;
  while (i != -1) {
    printf("%d ", l->dados[i]);
    i = l->prox[i];
  }
  printf("]\n");
}

// encontra um índice livre, e retira da lista de livres
static int l_indice_livre(Lista l)
{
  // o teste de cheia pode ser l->livre == -1
  assert(!l_cheia(l)); // poderia realocar...
  // pega o índice do primeiro da lista de livres
  int ind = l->livre;
  // o primeiro livre passa a ser o seguinte
  l->livre = l->prox[ind];
  return ind;
}

// insere o dado d no início da lista l
void l_insere_inicio(Lista l, dado_t d)
{
  l_ok(l);
  // acha um índice livre
  int novo = l_indice_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // o dado que segue o novo primeiro dado é o antigo primeiro
  // se a lista estava vazia, l->prim é -1, que é o que queremos
  //   como prox
  l->prox[novo] = l->prim;
  // atualiza a nova primeira posição
  l->prim = novo;
  l_ok(l);
}

// insere o dado d no final da lista l
void l_insere_fim(Lista l, dado_t d)
{
  // se a lista estiver vazia, é mais fácil inserir no início...
  if (l_vazia(l)) { l_insere_inicio(l, d); return; }
  // acha um índice livre
  int novo = l_indice_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // ele é o novo último
  l->prox[novo] = -1;
  // acha a posição do último (vai virar penúltimo)
  // a lista não está vazia, então l->prim é um índice válido
  // poderia poupar essa busca colocando a posição do
  //   último no descritor
  int ult;
  for (ult = l->prim; l->prox[ult] != -1; ult = l->prox[ult])
    /*nada*/;
  // o dado que segue o antigo último é o novo último
  l->prox[ult] = novo;
  l_ok(l);
}

// retorna o índice do vetor que contém o elemento da lista
//   que tá na posição pos
static int l_acha_indice_pos(Lista l, int pos)
{
  int i = l->prim;
  int p = 0;
  while (p < pos) {
    assert(i != -1);
    i = l->prox[i];
    p++;
  }
  assert(i != -1);
  return i;
}

// insere o dado d na lista l, de forma que ele fique na posição p
void l_insere_pos(Lista l, dado_t d, int p)
{
  // se a inserção for no início, temos uma função pronta...
  if (p == 0) { l_insere_inicio(l, d); return; }
  // acha o índice do elemento anterior ao inserido
  int anterior = l_acha_indice_pos(l, p - 1);
  // o índice do dado seguinte (pode ser -1)
  int seguinte = l->prox[anterior];
  // acha um índice livre
  int novo = l_indice_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // o dado novo fica após o anterior
  l->prox[anterior] = novo;
  // depois do novo fica o seguinte
  l->prox[novo] = seguinte;
}

// retorna o dado no início da lista
dado_t l_dado_inicio(Lista l)
{
  assert(!l_vazia(l));
  return l->dados[l->prim];
}

// retorna o dado no final da lista
dado_t l_dado_fim(Lista l)
{
  //...
  return 0;
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

