#ifndef JOGO_H
#define JOGO_H

#include <stdbool.h>

typedef struct jogo *Jogo;

// aloca e inicializa os dados para um novo jogo
// retorna o jogo criado
Jogo jogo_cria(int duração);

// libera a memória usada por um jogo
void jogo_destrói(Jogo j);

// retorna true se o jogo já terminou
bool jogo_terminou(Jogo j);

// retorna o número de dados de entrada para controlar o jogo (3)
int jogo_num_entradas(Jogo j);

// realiza o processamento de dados de entrada para controlar o jogo
// o controle é feito por um vetor de 3 números, o maior diz o que fazer
// - se for o primeiro, é para continuar como está
// - se for o segundo, é para dobrar à direita
// - se for o terceiro, é para dobrar à esquerda
void jogo_processa_entradas(Jogo j, float *entradas);

// avança na execução do jogo - movimenta o que tiver que ser movimentado,
//   verifica colisões, pontua, etc
void jogo_avança(Jogo j);

// desenha no terminal uma tela que representa o estado atual do jogo
void jogo_desenha_tela(Jogo j);

// retorno o número de pontos obtidos até agora
int jogo_pontos(Jogo j);

#endif // JOGO_H
