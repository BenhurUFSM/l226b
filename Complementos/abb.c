#include "abb.h"

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

// declaração do registro que contém um nó da árvore
struct abb {
  chave_t chave;
  valor_t valor;
  ABB esq;
  ABB dir;
};

// funções auxiliares (não exportadas aos clientes)

// aloca e retorna um nó folha com 'chave' e 'valor'
static ABB abb_cria_folha(chave_t chave, valor_t valor)
{
  ABB nova = malloc(sizeof(*nova));
  assert(nova != NULL);
  nova->chave = chave;
  nova->valor = valor;
  nova->esq = NULL;
  nova->dir = NULL;
  return nova;
}

static bool abb_é_folha(ABB a)
{
  return abb_é_vazia(a->esq) && abb_é_vazia(a->dir);
}

// libera a memória usada pelo nó 'a'
static void abb_destrói_nó(ABB a)
{
  // se algum dado fosse alocado, seria liberado aqui
  free(a);
}

// implementação das funções da interface
ABB abb_cria()
{
  return NULL; // a árvore vazia é representada por NULL
}

void abb_destrói(ABB a)
{
  if (abb_é_vazia(a)) return;
  abb_destrói(a->esq);
  abb_destrói(a->dir);
  abb_destrói_nó(a);
}

bool abb_é_vazia(ABB a)
{
  return a == NULL;
}


valor_t abb_busca(ABB a, chave_t chave,
                  comparador_t menor, comparador_t igual)
{
  return VALOR_NÃO_EXISTE;
}

ABB abb_insere(ABB a, chave_t chave, valor_t valor, valor_t *panterior,
               comparador_t menor, comparador_t igual)
{
  return a;
}

ABB abb_remove(ABB a, chave_t chave, valor_t *pvalor,
               comparador_t menor, comparador_t igual)
{
  return a;
}

