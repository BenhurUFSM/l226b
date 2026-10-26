#ifndef _AUXILIAR_H_
#define _AUXILIAR_H_

#include <stdbool.h>

// funções auxiliares para números aleatórios

// retorna um aleatório >= 0 e < 1
float al01();

// retorna um inteiro aleatório, que pode ser 0, mas não chega a ser limite
int aleatório(int limite);

// retorna um inteiro aleatório, que pode ser min, mas não chega a ser max
int aleatório_entre(int min, int max);

// retorna verdadeiro com uma certa probabilidade
bool probabilidade(int percentual);

// retorna um número aleatório com distribuição normal, com média e
//   desvio padrão dados
float gauss(float média, float desvio);

// retorna um inteiro aleatório, que pode ser 0, mas não chega a ser limite
// quanto mais próximo de 0, maior a chance
int aleatório_p(int limite);

// retorna o maior
int max(int a, int b);

#endif // _AUXILIAR_H_
