// programa principal para o jogo da cobrinha
// lê entrada do teclado, para controlar a cobrinha
//
// necessita jogo.[hc], fila.[hc], terminal.[hc]
//
// compila manualmente com:
//   gcc -o cobra main.c jogo.c fila.c terminal.c

#include "main.h"
#include "jogo.h"
#include "rede.h"
#include "terminal.h"
#include "treina.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int joga_sem_tela(Jogo jogo, Rede rede);

// lê a rede neural de um arquivo, para jogar 'jogo'
// se nome for "treina", executa o treinamento de redes
// retorna a rede lida, ou NULL
Rede lê_rede(char *nome, Jogo jogo);

// processa os argumentos da linha de comando:
// - o primeiro é o nome do arquivo com a rede neural, ou "treina"
// - o segundo é um inicializador para a geração de números aleatórios
// inicializa a rede e o jogo, e retorna o que deve ser feito
enum que_fazer { nada, treinar, jogar };
enum que_fazer processa_argumentos(int argc, char *argv[], Rede *rede,
                                   Jogo *jogo);

int main(int argc, char *argv[])
{
  Rede rede;
  Jogo jogo;
  int pontos;
  switch (processa_argumentos(argc, argv, &rede, &jogo)) {
    case treinar:
      treina_redes(jogo);
      break;
    case jogar:
      pontos = joga_com_tela(jogo, rede);
      printf("pontos: %d\n", pontos);
      break;
    case nada:
      break;
  }
  rede_destrói(rede);
  jogo_destrói(jogo);
}

enum que_fazer processa_argumentos(int argc, char *argv[], Rede *rede,
                                   Jogo *jogo)
{
  int ini = time(0);
  if (argc > 2) ini = atoi(argv[2]);
  srand48(ini);
  srand(ini);

  *jogo = jogo_cria();
  *rede = NULL;
  if (argc > 1) {
    if (strcmp(argv[1], "treina") == 0) return treinar;
    *rede = lê_rede(argv[1], *jogo);
    if (*rede == NULL) return nada;
  }
  return jogar;
}

Rede lê_rede(char *nome, Jogo jogo)
{
  Rede rede = rede_cria(nome);
    if (rede != NULL && (rede_num_entradas(rede) - 1 != jogo_num_saídas(jogo) ||
                         rede_num_saídas(rede) != jogo_num_entradas(jogo))) {
    printf("rede '%s' incompatível\n", nome);
      rede_destrói(rede);
      rede = NULL;
    }
  return rede;
}

void processa_teclado(float *entradas)
{
  for (int i = 0; i < 3; i++) entradas[i] = 0;
  char tecla = t_lê_tecla();
  switch (tecla) {
    case 'j':
    case 'n':
      entradas[2] = 1;
      break;
    case 'k':
    case 'e':
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

int joga_sem_tela(Jogo jogo, Rede rede)
{
  // return joga_com_tela(jogo, rede);
  float entradas[jogo_num_entradas(jogo)];
  float saídas[jogo_num_saídas(jogo)];

  while (!jogo_terminou(jogo)) {
    jogo_preenche_saídas(jogo, saídas);
    rede_calcula(rede, saídas, entradas);
    jogo_processa_entradas(jogo, entradas);
    jogo_avança(jogo);
  }

  int pontos = jogo_pontos(jogo);
  return pontos;
}
