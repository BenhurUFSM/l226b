#ifndef _INDIVIDUO_H_
#define _INDIVIDUO_H_

#include "rede.h"

#include <stdbool.h>

// Um Indivíduo é uma rede neural que pode evoluir por mutações ou por
//   mistura com outros indivíduos.
// Além da sua rede neural, o indivíduo mantém a pontuação obtida quando
//   sua rede foi usada para jogar.
// O indivíduo tem funções para mutar e para produzir novos indivíduos
//   através de mistura, e faz isso sob controle do treinador.
// O treinador controla vários indivíduos, distribuindo-os em Espécies.
// Um indivíduo sabe identificar se sua composição é próxima o suficiente
//   de outro para que sejam considerados da mesma espécie.

typedef struct indivíduo *Indivíduo;

// define o número de entradas e saídas das redes neurais de todos os
//   indivíduos
// esta função deve ser chamada antes de criar o primeiro indivíduo
void Indivíduo_inicializa(int n_entradas, int n_saídas);

// cria um indivíduo só com neurônios para entradas e saídas, sem sinapses
Indivíduo indivíduo_cria();

// cria um novo indivíduo à partir da mistura de dois outros
Indivíduo indivíduo_cria_mistura(Indivíduo ind1, Indivíduo ind2);

// libera a memória ocupada por um indivíduo
void indivíduo_destrói(Indivíduo self);

// muda a identificação da espécie do indivíduo
void indivíduo_altera_espécie(Indivíduo self, int espécie);

// retorna a identificação da espécie do indivíduo
int indivíduo_espécie(Indivíduo self);

// grava uma indivíduo em um arquivo, em formato que pode ser lido como Rede
void indivíduo_grava(Indivíduo self, char *nome);

// retorna a rede neural do indivíduo
Rede indivíduo_rede(Indivíduo self);

// registra a pontuação obtida no jogo
void indivíduo_altera_pontuação(Indivíduo self, float pontos);

// retorna a pontuação obtida no jogo
float indivíduo_pontuação(Indivíduo self);

// faz mutações variadas em um indivíduo
void indivíduo_muta(Indivíduo self);

// retorna true se 'b' pode ser considerado da mesma espécie que 'a'
bool indivíduo_mesma_espécie(Indivíduo a, Indivíduo b);

// compara dois indivíduos para ordenação por pontuação, com qsort
// pontuação maior fica antes (se p1 tem mais pontos que p2, retorna neg)
int indivíduo_compara_por_pontuação(const void *p1, const void *p2);

#endif // _INDIVIDUO_H_
