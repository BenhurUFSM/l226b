// programa principal para o jogo da cobrinha
// lê entrada do teclado, para controlar a cobrinha
//
// necessita jogo.[hc], fila.[hc], terminal.[hc]
//
// compila manualmente com:
//   gcc -o cobra main.c jogo.c fila.c terminal.c

#include "jogo.h"
#include "terminal.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// declaração de funções auxiliares

// lê uma tecla e preenche as entradas para o jogo, de acordo com a tecla
// se entradas[1] for maior, vira pra direita
// se entradas[2] for maior, vira pra esquerda
void processa_teclado(float *entradas);

// executa uma partida, mostrando a evolução na tela, e controlando pelo teclado
// retorna o número de pontos ao final da partida
int joga_com_tela();

int main()
{
  srand(time(0));
  int pontos = joga_com_tela();
  printf("pontos: %d\n", pontos);
}

void processa_teclado(float *entradas)
{
  entradas[0] = entradas[1] = entradas[2] = 0;
  char c = t_lê_tecla();
  switch (c) {
    case 'j': case 'n':
      entradas[2] = 1;
      break;
    case 'k': case 'e':
      entradas[1] = 1;
      break;
  }
}

int joga_com_tela()
{
  Jogo jogo;
  t_configura();
  jogo = jogo_cria(0);
  float entradas[jogo_num_entradas(jogo)];

  while (!jogo_terminou(jogo)) {
    jogo_desenha_tela(jogo);
    processa_teclado(entradas);
    jogo_processa_entradas(jogo, entradas);
    jogo_avança(jogo);
  }

  int pontos = jogo_pontos(jogo);
  jogo_destrói(jogo);
  t_normaliza();
  return pontos;
}

