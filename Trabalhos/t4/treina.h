#ifndef _TREINA_H_
#define _TREINA_H_

#include "jogo.h"

// roda o algoritmo de evolução para treinar redes neurais para
//   jogar o jogo "jogo"
// o algoritmo é baseado no NEAT (NeuroEvolution of Augmenting Topologies)
// http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf
void treina_redes(Jogo jogo);

#endif // _TREINA_H_
