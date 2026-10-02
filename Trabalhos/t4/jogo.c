// implementa uma cobrinha semovimentante, com direção controlável
// ganha pontos enquanto anda e mais quando come frutas
// morre se bater em si mesma ou na parede ou num obstáculo
//

// includes {{{1

#include "jogo.h"

#include "fila.h"
#include "terminal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// configurações {{{1

#define N_OBSTÁCULOS      4 // quantos obstáculos simultâneos estão no tabuleiro
#define TAM_INI_COBRA     5 // quantas partes tem o corpo da cobra no início
#define JOGO_NUM_ENTRADAS 3 // quantos valores são usados para controlar o jogo
#define PONTOS_POR_FRUTA  123 // quantos pontos se ganha comendo a fruta
#define AUMENTO_POR_FRUTA 5   // quanto aumenta a cada alimentação
#define AUTONOMIA         200 // quantos passos a cobrinha pode dar sem comer

// geometria {{{1

// um retângulo na tela
typedef struct {
  posição inf;
  posição sup;
} retângulo;

// retorna a posição do centro do retângulo
posição retângulo_centro(retângulo r)
{
  return (posição){.linha  = (r.inf.linha + r.sup.linha) / 2,
                   .coluna = (r.inf.coluna + r.sup.coluna) / 2};
}

// retorna true se a posição p está dentro o retângulo r
bool tá_dentro(posição p, retângulo r)
{
  if (p.linha <= r.inf.linha) return false;
  if (p.coluna <= r.inf.coluna) return false;
  if (p.linha >= r.sup.linha) return false;
  if (p.coluna >= r.sup.coluna) return false;
  return true;
}

// uma direção na tela
typedef enum { direita, esquerda, cima, baixo } direção;

typedef enum { reto, pra_direita, pra_esquerda } lado_da_curva;

// retorna a nova posição de pos, quando se move na direção dir
posição avanca_pos(posição pos, direção dir)
{
  switch (dir) {
    case direita:
      pos.coluna++;
      break;
    case esquerda:
      pos.coluna--;
      break;
    case cima:
      pos.linha--;
      break;
    case baixo:
      pos.linha++;
      break;
  }
  return pos;
}

// tipos {{{1

// uma cobra
typedef struct {
  Fila    corpo;      // posições dos pedacinhos da cobra
  posição pos_cabeca; // onde está a cabeça da cobra
  direção direção;    // para que lado a cobra está indo
} cobra;

// o estado do jogo
struct jogo {
  retângulo tela;       // área onde a cobrinha passeia
  cobra     aninha;     // a cobrinha
  int       aumentando; // número de peças que faltam na cobrinha
  Fila      obstáculos; // objetos espalhados para complicar a vida da cobrinha
  posição   fruta;      // onde está o objeto de desejo da cobrinha
  int       pontos;     // quantos pontos foram obtidos até agora
  int       passos;     // quantos passos a cobrinha já deu
  int       passos_fruta; // passos quando comeu a última fruta
  int       max_passos;   // número máximo de passos nesta partida
  enum { normal, terminando, terminado } estado;
};

// criação e estado {{{1

void sorteia_obstáculos(Jogo j);
void sorteia_fruta(Jogo j);

Jogo jogo_cria(int max_passos)
{
  Jogo j = malloc(sizeof(*j));
  assert(j != NULL);
  j->tela              = (retângulo){{2, 1}, {24, 50}};
  j->aninha.corpo      = f_cria(sizeof(posição));
  j->aninha.direção    = cima;
  j->aninha.pos_cabeca = retângulo_centro(j->tela);
  f_insere(j->aninha.corpo, &j->aninha.pos_cabeca);
  j->aumentando = TAM_INI_COBRA;
  j->obstáculos = f_cria(sizeof(posição));
  sorteia_obstáculos(j);
  sorteia_fruta(j);
  j->pontos       = 0;
  j->estado       = normal;
  j->passos       = 0;
  j->passos_fruta = 0;
  j->max_passos   = max_passos;
  return j;
}

void jogo_destrói(Jogo j)
{
  f_destrói(j->aninha.corpo);
  f_destrói(j->obstáculos);
  free(j);
}

int jogo_num_entradas(Jogo j)
{
  return JOGO_NUM_ENTRADAS;
}

bool jogo_terminou(Jogo j)
{
  return j->estado == terminado;
}

int jogo_pontos(Jogo j)
{
  return j->pontos;
}


// auxiliares {{{1

// retorna true se a fila de posições f contém a posição pos
bool fila_contém(Fila f, posição pos)
{
  posição p;
  f_inicia_percurso(f, 0);
  while (f_próximo(f, &p)) {
    if (pos.linha == p.linha && pos.coluna == p.coluna) {
      return true;
    }
  }
  return false;
}

// retorna um número aleatório entre min e max, inclusivos
int aleatório_entre(int min, int max)
{
  return min + rand() % (max - min + 1);
}

// diz se posição dada está sem nada
bool posição_livre(Jogo j, posição pos)
{
  // vê se tá fora da tela
  if (!tá_dentro(pos, j->tela)) return false;
  // vê se tem um obstáculo
  if (fila_contém(j->obstáculos, pos)) return false;
  // vê se tá na cobrinha
  if (fila_contém(j->aninha.corpo, pos)) return false;

  return true;
}

// retorna uma posição aleatória dentro do retângulo, onde não tem obstáculo
//   nem cobrinha
posição sorteia_pos_livre(Jogo j)
{
  posição pos;
  int     min_lin = j->tela.inf.linha + 1;
  int     max_lin = j->tela.sup.linha - 1;
  int     min_col = j->tela.inf.coluna + 1;
  int     max_col = j->tela.sup.coluna + 1;
  do {
    pos.linha  = aleatório_entre(min_lin, max_lin);
    pos.coluna = aleatório_entre(min_col, max_col);
  } while (!posição_livre(j, pos));
  return pos;
}

// preenche a fila de obstáculos com posições aleatórias
void sorteia_obstáculos(Jogo j)
{
  while (!f_tá_vazia(j->obstáculos)) {
    f_remove(j->obstáculos, NULL);
  }
  for (int nobs = 0; nobs < N_OBSTÁCULOS; nobs++) {
    posição pos = sorteia_pos_livre(j);
    f_insere(j->obstáculos, &pos);
  }
}

// sorteia a posição onde fica a fruta
void sorteia_fruta(Jogo j)
{
  j->fruta = sorteia_pos_livre(j);
}
// desenho {{{1

// cores dos objetos na tela
cor cor_fundo          = {0, 0, 0};
cor cor_cobra_morrendo = {200, 30, 30};
cor cor_cobra_normal   = {150, 150, 0};
cor cor_obstáculo      = {200, 0, 0};
cor cor_contorno       = {255, 50, 200};
cor cor_fruta          = {55, 50, 180};

// formas dos objetos na tela
char *desenho_rabo      = "."; // \u25e6
char *desenho_corpo     = "O"; // \u25cb
char *desenho_cabeça    = ":"; // \u2687
char *desenho_fruta     = "*"; //
char *desenho_obstáculo = "X"; //

// desenha a borda do tabuleiro
void desenha_contorno(Jogo j)
{
  retângulo r = j->tela;
  t_seleciona_cor(cor_fundo, cor_contorno);
  t_posiciona(r.inf);
  fputs("╭", stdout);
  for (int c = r.inf.coluna + 1; c < r.sup.coluna; c++) {
    fputs("─", stdout);
  }
  fputs("╮", stdout);
  for (int l = r.inf.linha + 1; l < r.sup.linha; l++) {
    t_posiciona((posição){l, r.inf.coluna});
    fputs("│", stdout);
    t_posiciona((posição){l, r.sup.coluna});
    fputs("│", stdout);
  }
  t_posiciona((posição){r.sup.linha, r.inf.coluna});
  fputs("╰", stdout);
  for (int c = r.inf.coluna + 1; c < r.sup.coluna; c++) {
    fputs("─", stdout);
  }
  fputs("╯", stdout);
}

// desenha a cobrinha na tela
void desenha_cobra(Jogo j)
{
  if (j->estado == terminando) {
    t_seleciona_cor(cor_fundo, cor_cobra_morrendo);
  } else {
    t_seleciona_cor(cor_fundo, cor_cobra_normal);
  }
  posição pos;
  f_inicia_percurso(j->aninha.corpo, 0);
  f_próximo(j->aninha.corpo, &pos);
  t_posiciona(pos);
  fputs(desenho_rabo, stdout);
  while (f_próximo(j->aninha.corpo, &pos)) {
    t_posiciona(pos);
    fputs(desenho_corpo, stdout);
  }
  t_posiciona(pos);
  fputs(desenho_cabeça, stdout);
}

// desenha os obstáculos na tela
void desenha_obstáculos(Jogo j)
{
  t_seleciona_cor(cor_fundo, cor_obstáculo);
  posição pos;
  f_inicia_percurso(j->obstáculos, 0);
  while (f_próximo(j->obstáculos, &pos)) {
    t_posiciona(pos);
    fputs(desenho_obstáculo, stdout);
  }
}

// desenha a fruta na tela
void desenha_fruta(Jogo j)
{
  t_seleciona_cor(cor_fundo, cor_fruta);
  t_posiciona(j->fruta);
  fputs(desenho_fruta, stdout);
}


// desenha a tela do jogo
void jogo_desenha_tela(Jogo j)
{
  t_seleciona_cor(cor_fundo, cor_contorno);
  t_limpa();

  // desenha o contorno da janela
  desenha_contorno(j);

  // desenha os objetos
  desenha_obstáculos(j);
  desenha_fruta(j);
  desenha_cobra(j);

  // desenha a pontuação
  t_seleciona_cor_normal();
  t_posiciona((posição){1, 1});
  printf("%d", j->pontos);
}

// movimentação {{{1

void vê_se_acertou_a_fruta(Jogo j)
{
  posição pos = j->aninha.pos_cabeca;
  if (pos.linha == j->fruta.linha && pos.coluna == j->fruta.coluna) {
    j->pontos += PONTOS_POR_FRUTA;
    j->aumentando += AUMENTO_POR_FRUTA;
    sorteia_fruta(j);
    j->passos_fruta = j->passos;
  }
}

bool cobrinha_bateu(Jogo j)
{
  return !posição_livre(j, j->aninha.pos_cabeca);
}

// faz um movimento da cobrinha e vê se bateu em algo
void movimenta_cobrinha(Jogo j)
{
  // calcula a nova posição da cabeça da cobrinha
  j->aninha.pos_cabeca = avanca_pos(j->aninha.pos_cabeca, j->aninha.direção);

  // vê se essa posição é ok
  if (cobrinha_bateu(j)) {
    j->estado = terminando;
  } else {
    vê_se_acertou_a_fruta(j);
  }
  // coloca a nova cabeça na cobrinha
  f_insere(j->aninha.corpo, &j->aninha.pos_cabeca);

  // se a cobra não tiver aumentando, tira o último pedaço
  if (j->aumentando == 0) {
    f_remove(j->aninha.corpo, NULL);
  } else {
    j->aumentando--;
  }
}

// reaje à passagem do tempo
// - muda a cabeça para uma nova posição, de acordo com a direção
// - se não tiver aumentando, remove a posição do rabo
// - verifica se bateu em algo e reage de acordo
// - verifica se está há muito tempo sem pegar a fruta, ou se há muito tempo
//   jogando, para garantir que o jogo termina (importante durante o
//   treinamento)
void jogo_avança(Jogo j)
{
  if (j->estado == terminando) {
    f_remove(j->aninha.corpo, NULL);
    if (f_tá_vazia(j->aninha.corpo)) {
      j->estado = terminado;
    }
    return;
  }

  movimenta_cobrinha(j);

  if (j->estado == normal) {
    j->passos++;
    j->pontos++;
    if (j->passos - j->passos_fruta > AUTONOMIA) {
      j->estado = terminando;
    }
    if (j->max_passos != 0 && j->passos > j->max_passos) {
      j->estado = terminando;
    }
  }
}

// entrada {{{1

// muda a direção da cobra de acordo com o lado da curva
void faz_curva(Jogo j, lado_da_curva lado)
{
  // qual a próxima direção se está indo para tal direção e faz tal curva?
  direção prox_direita[] = {
      [cima]     = direita,
      [direita]  = baixo,
      [baixo]    = esquerda,
      [esquerda] = cima,
  };
  direção prox_esquerda[] = {
      [cima]     = esquerda,
      [direita]  = cima,
      [baixo]    = direita,
      [esquerda] = baixo,
  };
  if (lado == pra_esquerda) {
    j->aninha.direção = prox_esquerda[j->aninha.direção];
  } else if (lado == pra_direita) {
    j->aninha.direção = prox_direita[j->aninha.direção];
  } // se for reto, não muda
}

void jogo_processa_entradas(Jogo j, float *entradas)
{
  //  as entradas são o quanto se acha que tem que ir reto ou pra direita ou
  //  esquerda
  if (entradas[1] > entradas[0] && entradas[1] > entradas[2])
    faz_curva(j, pra_direita);
  if (entradas[2] > entradas[0] && entradas[2] > entradas[1])
    faz_curva(j, pra_esquerda);
}

// vim: foldmethod=marker shiftwidth=2
