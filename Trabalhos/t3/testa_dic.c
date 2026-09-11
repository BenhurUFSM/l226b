#include "dicionario.h"
#include <stdio.h>
#include <stdlib.h>

bool chaves_são_iguais(chave_t a, chave_t b)
{
  int *pa = a;
  int *pb = b;
  return *pa == *pb;
}

bool chave_é_menor(chave_t a, chave_t b)
{
  int *pa = a;
  int *pb = b;
  return *pa < *pb;
}

// vetor global, para poder ser grande
#define NDADOS (65536)
int valores[NDADOS];

int main()
{
  Dicionário d;

  d = dic_cria(chave_é_menor, chaves_são_iguais);

  // preenche o vetor e o dicionário com valores aleatórios não repetidos
  for (int k = 0; k < NDADOS; k++) {
    do {
      valores[k] = rand();
    } while (dic_busca(d, &valores[k]) != VALOR_NÃO_EXISTE);
    dic_insere(d, &valores[k], &valores[k]);
  }

  for (int i = 0; i < 4; i++) {
    printf("%d\n", i);
    // busca todos os valores do vetor no dicionário
    for (int k = 0; k < NDADOS; k++) {
      int *p = dic_busca(d, &valores[k]);
      if (p == VALOR_NÃO_EXISTE)
        printf("erro na busca i%d, k%d, v%d\n", i, k, valores[k]);
      else if (*p != valores[k])
        printf("busca retornou valor errado\n");
    }
    // troca os valores no vetor e no dicionário
    for (int k = 0; k < NDADOS; k++) {
      dic_remove(d, &valores[k]);
      do {
        valores[k] = rand();
      } while (dic_busca(d, &valores[k]) != VALOR_NÃO_EXISTE);
      dic_insere(d, &valores[k], &valores[k]);
    }
  }

  dic_destrói(d);
}
