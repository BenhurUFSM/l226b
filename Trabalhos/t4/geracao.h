#ifndef _GERACAO_H_
#define _GERACAO_H_

#include "jogo.h"

// a evolução acontece em gerações
// uma geração contém indivíduos, divididos em espécies
// cada indivíduo joga e consegue determinada pontuação
// depois de calculados, os resultados individuais são comparados e os
//   melhores são reproduzidos, misturados, sofrem mutações para compor
//   a geração seguinte.

typedef struct geração *Geração;

// cria uma geração inicial, com indivíduos criados aleatoriamente
// separa os indivíduos em espécies
Geração geração_cria_aleatório();

// libera a memória ocupada por uma geração e seus indivíduos
void geração_destrói(Geração geração);

// calcula a pontuação de cada indivíduo de uma geração, jogando com ele.
void geração_joga(Geração geração, Jogo jogo);

// calcula o número de descendentes para cada espécie da geração
void geração_calcula_descendentes(Geração geração);

// cria uma nova geração, à partir dos indivíduos da geração atual
Geração geração_reproduz(Geração velha);

// grava os indivíduos da geração em arquivos
void geração_grava(Geração geração);

// imprime um resumo da geração, para log
void geração_imprime(Geração geração);

#endif // _GERACAO_H_
