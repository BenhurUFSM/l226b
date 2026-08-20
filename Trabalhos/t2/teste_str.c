
// teste_str.c
// programa com testes do TAD str

#include "str.h"
#include <stdio.h>

int main()
{
  char *a = "Rábica";
  Str s = s_cria(a);
  printf("Deve escrever [%s] ", a);
  s_imprime(s);
  Str ss = s_cria_substring(s, 3, 2);
  printf("\nDeve escrever [ic] ");
  s_imprime(ss);
  s_substring(ss, s, 1, 2);
  printf("\nDeve escrever [áb] ");
  s_imprime(ss);
  if (s_tam(ss) != 2) printf("\ntamanho de 'áb' (%d) deveria ser 2\n", s_tam(ss));
  s_substitui(ss, -2, 1, NULL);
  s_substitui(s, -7, 3, ss);
  printf("Deve escrever [Rica] ");
  s_imprime(s);
  s_destroi(ss);
  s_destroi(s);
}
