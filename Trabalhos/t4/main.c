// programa principal para o jogo da cobrinha
// lê entrada do teclado, para controlar a cobrinha
//
// necessita jogo.[hc], fila.[hc], terminal.[hc]
//
// compila manualmente com:
//   gcc -o cobra main.c jogo.c fila.c terminal.c

#include "jogo.h"
#include "rede.h"
#include "terminal.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// declaração de funções auxiliares

// lê uma tecla e preenche as entradas para o jogo, de acordo com a tecla
// tem que ser coerente com o que é esperado por jogo_processa_entradas:
// entradas[*] tem 3 valores,
//   entradas[1] deve ser o maior para virar pra direita
//   entradas[2] deve ser o maior para virar pra esquerda
//   senão, continua reto
void processa_teclado(float *entradas);

// executa uma partida, mostrando a evolução na tela, e controlando pelo teclado
// retorna o número de pontos ao final da partida
int joga_com_tela(Jogo jogo, Rede rede);

// lê a rede neural de um arquivo, para jogar 'jogo'
// se nome for "treina", executa o treinamento de redes
// retorna a rede lida, ou NULL
Rede lê_rede(char *nome, Jogo jogo);

int main(int argc, char *argv[])
{
  srand(time(0));
  Rede rede = NULL;
  Jogo jogo = jogo_cria(0);

  if (argc > 1) {
    rede = rede_cria(argv[1]);
    if (rede != NULL && (rede_num_entradas(rede) - 1 != jogo_num_saídas(jogo) ||
                         rede_num_saídas(rede) != jogo_num_entradas(jogo))) {
      printf("rede '%s' incompatível\n", argv[1]);
      printf("%d %d %d %d\n", rede_num_entradas(rede), jogo_num_entradas(jogo),
                         rede_num_saídas(rede), jogo_num_saídas(jogo));
      rede_destrói(rede);
      rede = NULL;
    }
  }

  int pontos = joga_com_tela(jogo, rede);
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

int joga_com_tela(Jogo jogo, Rede rede)
{
  t_configura();
  float entradas[jogo_num_entradas(jogo)];
  float saídas[jogo_num_saídas(jogo)];

  while (!jogo_terminou(jogo)) {
    jogo_desenha_tela(jogo);
    if (rede != NULL) {
      jogo_preenche_saídas(jogo, saídas);
      rede_calcula(rede, saídas, entradas);
      jogo_processa_entradas(jogo, entradas);
    }
    processa_teclado(entradas);
    jogo_processa_entradas(jogo, entradas);
    jogo_avança(jogo);
  }

  int pontos = jogo_pontos(jogo);
  t_normaliza();
  return pontos;
}

