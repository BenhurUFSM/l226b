// implementação simples de dicionário, usando vetor
// l226b

#include "dicionario.h"

#include <assert.h>
#include <string.h>

// registro auxiliar para conter uma tupla (uma chave e seu valor)
typedef struct {
  chave_t chave;
  valor_t valor;
} tupla;

struct dicionário {
  int n_tuplas;           // quantas tuplas existem no dicionário
  int capacidade;         // quantas tuplas cabem no vetor
  tupla *tuplas;          // vetor de tuplas
  comparador_t menor;     // função de comparação de chaves
  comparador_t igual;     // função de comparação de chaves
  int índice_do_percurso; // índice do próximo no percurso
};

Dicionário dic_cria(comparador_t menor, comparador_t igual)
{
  Dicionário d = malloc(sizeof(*d));
  assert(d != NULL);
  d->menor = menor;
  d->igual = igual;
  d->n_tuplas = 0;
  d->capacidade = 0;
  d->tuplas = NULL;
  d->índice_do_percurso = -1;
  return d;
}

void dic_destrói(Dicionário d)
{
  if (d == NULL) return;
  if (d->tuplas != NULL) free(d->tuplas);
  free(d);
}

// funções auxiliares

// retorna o índice da tupla com 'chave' ou -1
static int dic_busca_índice(Dicionário d, chave_t chave)
{
  for (int i = 0; i < d->n_tuplas; i++) {
    if (d->igual(chave, d->tuplas[i].chave)) return i;
  }
  return -1;
}

// precisa espaço para n tuplas
// dobra de tamanho se precisar aumentar
// reduz pela metade se tiver ocupando menos que 1/3 do espaço
// aloca pelo menos espaço para 8 tuplas
static void dic_alocação(Dicionário d, int n)
{
  if (d->capacidade >= n && (d->capacidade == 8 || d->capacidade < 3*n)) return;
  int novacap = d->capacidade;
  if (novacap == 0) novacap = 8;
  while (novacap < n) novacap *= 2;
  while (novacap > 3*n) novacap /= 2;
  if (novacap < 8) novacap = 8;
  d->tuplas = realloc(d->tuplas, novacap * sizeof(tupla));
  assert(d->tuplas != NULL);
  d->capacidade = novacap;
}

// implementação das funções da interface pública

valor_t dic_busca(Dicionário d, chave_t chave)
{
  int i = dic_busca_índice(d, chave);
  if (i == -1) return VALOR_NÃO_EXISTE;
  return d->tuplas[i].valor;
}

valor_t dic_insere(Dicionário d, chave_t chave, valor_t valor)
{
  int i = dic_busca_índice(d, chave);
  if (i != -1) {
    valor_t valor_anterior = d->tuplas[i].valor;
    d->tuplas[i].valor = valor;
    return valor_anterior;
  }
  dic_alocação(d, d->n_tuplas + 1);
  d->tuplas[d->n_tuplas].chave = chave;
  d->tuplas[d->n_tuplas].valor = valor;
  d->n_tuplas++;
  return VALOR_NÃO_EXISTE;
}

valor_t dic_remove(Dicionário d, chave_t chave)
{
  int i = dic_busca_índice(d, chave);
  if (i == -1) return VALOR_NÃO_EXISTE;
  valor_t valor = d->tuplas[i].valor;
  d->n_tuplas--;
  int n = d->n_tuplas - i;
  memmove(&d->tuplas[i], &d->tuplas[i+1], n * sizeof(tupla));
  dic_alocação(d, d->n_tuplas);
  return valor;
}

void dic_para_todos(Dicionário d, void (*f)(chave_t, valor_t))
{
  for (int i = 0; i < d->n_tuplas; i++) {
    f(d->tuplas[i].chave, d->tuplas[i].valor);
  }
}

void dic_inicia_percurso(Dicionário d)
{
  d->índice_do_percurso = 0;
}

bool dic_próximo(Dicionário d, chave_t *pchave, valor_t *pvalor)
{
  if (d->índice_do_percurso < 0) return false;
  if (d->índice_do_percurso >= d->n_tuplas) {
    d->índice_do_percurso = -1;
    return false;
  }
  if (pchave != NULL) *pchave = d->tuplas[d->índice_do_percurso].chave;
  if (pvalor != NULL) *pvalor = d->tuplas[d->índice_do_percurso].valor;
  d->índice_do_percurso++;
  return true;
}
