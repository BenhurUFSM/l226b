#ifndef _FILA_H_
#define _FILA_H_

typedef struct fila *Fila;

#include <stdbool.h>

// funções que implementam as operações básicas de uma fila

// cria uma fila vazia que suporta dados do tamanho fornecido (em bytes)
Fila f_cria(int tam_do_dado);

// libera a memória ocupada pela fila
void f_destrói(Fila self);

// diz se a fila está vazia
bool f_tá_vazia(Fila self);

// remove o dado no início da fila e, se pdado não for NULL, copia o dado
//   removido para *pdado
void f_remove(Fila self, void *pdado);

// insere o dado apontado por pdado no final da fila
void f_insere(Fila self, void *pdado);

// funções que implementam operações complementares, que permitem acesso
//   a todos os elementos da fila. Durante um percurso, a fila não pode ser
//   alterada.

// inicia um percurso aos elementos da fila, a partir de uma posição inicial
// se a posição for positiva, o percurso vai dessa posição até o fim da fila
// se a posição for negativa, o percurso vai dessa posição até o início
//   0 é a posição do primeiro dado (aquele que está na fila há mais tempo)
//   1 é a posição do segundo dado, etc
//   além disso,
//   -1 é a posição do último dado (o que está na fila há menos tempo)
//   -2 é a posição do penúltimo dado, etc
// cada dado do percurso será acessado por chamadas a f_próximo()
void f_inicia_percurso(Fila self, int pos_inicial);

// caso o percurso tenha terminado, retorna false
// senão, coloca o próximo dado do percurso em *pdado (se pdado não for NULL),
//   e retorna true
bool f_próximo(Fila self, void *pdado);

#endif //_FILA_H_
