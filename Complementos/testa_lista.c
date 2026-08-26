// testa_lista.c
#include "lista.h"
#include <stdio.h>

int main()
{
  int d;
  Lista l = l_cria();
  printf("[]: ");
  l_imprime(l);
  l_insere_fim(l, 5);
  printf("[ 5 ]: ");
  l_imprime(l);
  l_insere_inicio(l, 7);
  printf("[ 7 5 ]: ");
  l_imprime(l);
  l_insere_fim(l, 8);
  l_insere_pos(l, 4, 2);
  printf("[ 7 5 4 8 ]: ");
  l_imprime(l);
  d = l_remove_inicio(l);
  printf("7: %d\n", d);
  printf("[ 5 4 8 ]: ");
  l_imprime(l);
  d = l_remove_fim(l);
  printf("8: %d\n", d);
  printf("[ 5 4 ]: ");
  l_imprime(l);
  d = l_remove_pos(l, 1);
  printf("4: %d\n", d);
  printf("[ 5 ]: ");
  l_imprime(l);
}
