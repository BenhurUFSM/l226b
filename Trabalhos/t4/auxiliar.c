#include "auxiliar.h"

#include <math.h>
#include <stdlib.h>

// usa rand48 em vez de rand ou random, para ter valores aleatórios
//   independentes para o treinamento e para o jogo

float al01()
{
  return drand48();
  // return (random() % 1000000) / (float)1000000;
}

float gauss(float média, float desvio)
{
  float pi = acosf(-1);
  float x, y, z;
  x = al01();
  y = al01();
  z = sqrtf(-2 * logf(x)) * cosf(2 * pi * y);
  return média + desvio * z;
}

int aleatório(int limite)
{
  return lrand48() % limite;
  // return random() % limite;
}

int aleatório_p(int limite)
{
  if (limite <= 1) return 0;
  float alp01;
  do {
    alp01 = fabsf(gauss(0, 1));
  } while (alp01 >= 1);
  return alp01 * limite;
}

int aleatório_entre(int min, int max)
{
  return min + aleatório(max - min);
}

int max(int a, int b)
{
  if (a > b) return a;
  return b;
}

bool probabilidade(int percentual)
{
  return aleatório(100) < percentual;
}
