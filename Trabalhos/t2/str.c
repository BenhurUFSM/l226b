// includes, constantes e declarações {{{1
#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_ALLOC 8    // alocação mínima

struct str {
  // ...
};

// A memória para conter os bytes de uma string deve ser alocada e/ou
//   realocada conforme a necessidade, cuidando para que a quantidade
//   de memória alocada seja sempre:
//   - nula (não alocada) se a string for vazia, ou
//   - não inferior ao necessário para armazenar os bytes da codificação utf8;
//   - não inferior à alocação mínima;
//   - não superior ao triplo do número de bytes necessários
//     (exceto quando for o mínimo);
//   - uma potência de 2.

// funções auxiliares {{{1

// verifica se a string cad está de acordo com a especificação
// aborta o programa se não tiver
static void s_ok(Str s)
{
}

//...

// operações de criação e destruição {{{1

Str s_cria(char *strC)
{
  //...
  return NULL;
}

void s_destroi(Str s)
{
  s_ok(s);
  //...
}

// operações de acesso {{{1

int s_tam(Str s)
{
  s_ok(s);
  //...
  return 0;
}

char *s_strc(Str s)
{
  s_ok(s);
  //...
  return NULL;
}

unichar s_ch(Str s, int pos)
{
  s_ok(s);
  //...
  return UNI_INV;
}

Str s_substring(Str s, int pos, int tam)
{
  s_ok(s);
  //...
  return NULL;
}

Str s_copia(Str s)
{
  s_ok(s);
  //...
  return NULL;
}


// operações de busca e comparação {{{1

bool s_igual(Str s, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return false;
}

int s_busca_c(Str s, int pos, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_nc(Str s, int pos, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_rc(Str s, int pos, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_rnc(Str s, int pos, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_s(Str s, int pos, Str buscada)
{
  s_ok(s);
  s_ok(buscada);
  //...
  return -1;
}


// operações de alteração {{{1

void s_substitui(Str s, int pos, int tam, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
}

void s_anexa(Str s, Str sb)
{
  s_substitui(s, -1, 0, sb);
}

void s_insere(Str s, int pos, Str sb)
{
  //...
}

void s_remove(Str s, int pos, int tam)
{
  //...
}

Str s_apara(Str s, Str sobras)
{
  s_ok(s);
  s_ok(sobras);
  //...
  return NULL;
}

// operações de E/S {{{1

// imprime a string em s na saída padrão
void s_imprime(Str s)
{
  s_ok(s);
  //...
}

// Retorna uma nova string com o conteúdo do arquivo chamado nome.
// Retorna uma string vazia em caso de erro.
Str s_le_arquivo(Str nome)
{
  s_ok(nome);
  //...
  return NULL;
}

// grava o conteúdo de s em um arquivo chamado nome
void s_grava_arquivo(Str s, Str nome)
{
  s_ok(s);
  s_ok(nome);
  //...
}


// vim: foldmethod=marker shiftwidth=2

