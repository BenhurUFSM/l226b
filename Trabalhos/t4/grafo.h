#ifndef _GRAFO_H_
#define _GRAFO_H_

typedef struct grafo *Grafo;

#include "fila.h"
#include <stdbool.h>

// funções que implementam as operações básicas de um grafo
//
// um grafo é constituido por nós e arestas que os interligam.
// existe um valor associado a cada nó e a cada aresta. Esses valores são
//   armazenados pelo grafo, mas o grafo desconhece seu tipo, só conhece
//   seu tamanho.
// os nós são identificados por um inteiro que corresponde à ordem em que
//   são inseridos no grafo, com 0 correspondendo ao primeiro.
// a identificação de um nó é sempre um número inferior ao número de nós
//   no grafo. A remoção de um nó altera a identificação dos nós com números
//   superiores.
// as arestas são direcionadas, e são identificadas pelo seu nó de origem
//   e de destino.
// os valores das arestas e dos nós são copiados para o grafo. O acesso a
//   esses valores é por meio de ponteiros. O usuário do grafo pode alterar
//   o valor armazenado no grafo após obter o ponteiro (com g_valor_do_nó
//   por exemplo). O usuário pode também alterar o valor usando uma função
//   de alteração (como g_altera_valor_do_nó).

// cria um grafo vazio que suporta dados do tamanho fornecido (em bytes)
//   nos nós e nas arestas
Grafo g_cria(int tam_nó, int tam_aresta);

// libera a memória ocupada por um grafo
void g_destrói(Grafo self);

// Nós

// insere um nó no grafo, com o dado apontado por p_dado
// retorna o número do novo nó (que é o número de nós que
//   existia antes da inserção)
int g_insere_nó(Grafo self, void *p_dado);

// remove um nó do grafo e as arestas incidentes nesse nó
// a identificação dos nós remanescentes é alterada, como se esse nó nunca
//   tivesse existido
void g_remove_nó(Grafo self, int nó);

// altera o valor associado a um nó (copia o valor apontado por p_dado para
//   uma memória gerenciada pelo grafo)
void g_altera_valor_do_nó(Grafo self, int nó, void *p_dado);

// retorna um ponteiro para o valor associado a um nó que está na memória
//   gerenciada pelo grafo.
void *g_valor_do_nó(Grafo self, int nó);

// retorna o número de nós do grafo
int g_num_nós(Grafo self);

// Arestas

// altera o valor da aresta que interliga o nó origem ao nó destino
//   (copia de *p_dado para uma memória gerenciada pelo grafo)
// caso a aresta não exista, deve ser criada
// caso p_dado seja NULL, a aresta deve ser removida
void g_altera_valor_da_aresta(Grafo self, int nó_origem, int nó_destino,
                              void *p_dado);

// retorna um ponteiro para o valor associado à aresta entre nó_origem e 
//   nó_destino, localizado em memória gerenciada pelo grafo.
// retorna NULL se tal aresta não existir
void *g_valor_da_aresta(Grafo self, int nó_origem, int nó_destino);

// inicia uma consulta a arestas que partem do nó origem
// as próximas chamadas a 'g_próxima_aresta' devem retornar os valores
//   correspondentes a cada aresta que parte desse nó
void g_arestas_que_partem(Grafo self, int nó_origem);

// inicia uma consulta a arestas que chegam ao nó destino
// as próximas chamadas a 'g_próxima_aresta' devem retornar os valores
//   correspondentes a cada aresta que chega nesse nó
void g_arestas_que_chegam(Grafo self, int nó_destino);

// retorna um ponteiro para o dado associado à próxima aresta, de acordo
//   com a última consulta iniciada por 'g_arestas_que_partem' ou 
//   'g_arestas_que_chegam'
// o valor do nó vizinho ao nó da consulta deve ser colocado em '*p_nó_vizinho'
//   (se não for NULL)
// caso não exista mais aresta que satisfaça a consulta, retorna NULL.
void *g_próxima_aresta(Grafo self, int *p_nó_vizinho);

// Algoritmos

// retorna true se grafo é cíclico, false caso contrário
bool g_tem_ciclo(Grafo self);

// retorna uma fila de inteiros contendo os números dos nós do grafo em
//   uma ordem em que, se o nó 'a' antecede 'b', não existe uma aresta
//   de 'b' para 'a' no grafo
// deve retornar uma fila vazia caso tal ordem não exista
// quem chama esta função é responsável por destruir a fila.
Fila g_ordem_topológica(Grafo self);

#endif //_GRAFO_H_
