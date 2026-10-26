#include "treina.h"
#include "config.h"
#include "geracao.h"
#include "individuo.h"

#include <assert.h>

void treina_redes(Jogo jogo)
{
  // inicializa o número de entradas e saídas das redes de acordo com o jogo
  // as saídas do jogo são as entradas da rede e vice-versa
  Indivíduo_inicializa(jogo_num_saídas(jogo) + 1, jogo_num_entradas(jogo));
  // começa com um povo aleatório
  Geração atual = geração_cria_aleatório();
  // evolui cada geração
  for (int i = 0; i < N_GERAÇÕES; i++) {
    // executa o jogo, controlado por cada indivíduo da geração, e pontua-os
    geração_joga(atual, jogo);
    // grava o melhor deles em disco, e põe uma mensagem na tela
    geração_grava(atual);
    geração_imprime(atual);
    // calcula quantos descendentes cada espécie terá, conforme a pontuação
    geração_calcula_descendentes(atual);
    // gera uma nova geração, com os descendentes da geração atual
    Geração próxima = geração_reproduz(atual);
    // substitui a geração antiga pela nova
    geração_destrói(atual);
    atual = próxima;
  }
  geração_destrói(atual);
}
