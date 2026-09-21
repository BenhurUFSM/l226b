// implementação de dicionário usando árvore binária de busca
// l226b

#include "dicionario.h"

#include "abb.h"
#include <assert.h>
#include <string.h>

// registro auxiliar para conter uma tupla (uma chave e seu valor)
typedef struct {
  chave_t chave;
  valor_t valor;
} tupla;

struct dicionário {
  comparador_t menor;     // função de comparação de chaves
  comparador_t igual;     // função de comparação de chaves
  ABB abb;
};

Dicionário dic_cria(comparador_t menor, comparador_t igual)
{
  Dicionário d = malloc(sizeof(*d));
  assert(d != NULL);
  d->menor = menor;
  d->igual = igual;
  d->abb = abb_cria();
  return d;
}

void dic_destrói(Dicionário d)
{
  if (d == NULL) return;
  abb_destrói(d->abb);
  free(d);
}

// funções auxiliares

// implementação das funções da interface pública

valor_t dic_busca(Dicionário d, chave_t chave)
{
  return abb_busca(d->abb, chave, d->menor, d->igual);
}

valor_t dic_insere(Dicionário d, chave_t chave, valor_t valor)
{
  valor_t valant;
  d->abb = abb_insere(d->abb, chave, valor, &valant, d->menor, d->igual);
  return valant;
}

valor_t dic_remove(Dicionário d, chave_t chave)
{
  valor_t valant;
  d->abb = abb_remove(d->abb, chave, &valant, d->menor, d->igual);
  return valant;
}

void dic_para_todos(Dicionário d, void (*f)(chave_t, valor_t))
{
}

void dic_inicia_percurso(Dicionário d)
{
}

bool dic_próximo(Dicionário d, chave_t *pchave, valor_t *pvalor)
{
  return false;
}
