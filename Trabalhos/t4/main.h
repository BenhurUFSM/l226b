#ifndef _MAIN_H_
#define _MAIN_H_

// funções exportadas pelo main.c

#include "jogo.h"
#include "rede.h"

// executa uma partida, mostrando a evolução na tela, e controlando pelo teclado
// ou pela rede, se não for NULL
// retorna o número de pontos ao final da partida
int joga_com_tela(Jogo jogo, Rede rede);

// executa uma partida, controlada pela rede, sem mostrar nada
// retorna o número de pontos ao final da partida
// para treinar a rede
int joga_sem_tela(Jogo jogo, Rede rede);

#endif // _MAIN_H_
