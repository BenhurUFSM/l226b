// lista.h
#ifndef LISTA_H
#define LISTA_H

// TAD de uma lista

#include <stdbool.h>

// o tipo dos dados mantidos pela lista
typedef int dado_t;

typedef struct lista *Lista;

// cria e inicializa uma lista vazia
Lista l_cria();

// libera a memória ocupada por uma lista
void l_destroi(Lista l);

// retorna o número de elementos na lista
int l_tam(Lista l);

// retorna true se a lista tiver cheia
bool l_cheia(Lista l);

// retorna true se a lista tiver vazia
bool l_vazia(Lista l);

// imprime os dados que estão na lista
void l_imprime(Lista l);

// insere o dado d no início da lista l
void l_insere_inicio(Lista l, dado_t d);

// insere o dado d no final da lista l
void l_insere_fim(Lista l, dado_t d);

// insere o dado d na lista l, de forma que ele fique na posição p
// a primeira posição é 0
void l_insere_pos(Lista l, dado_t d, int p);

// retorna o dado no início da lista
dado_t l_dado_inicio(Lista l);

// retorna o dado no final da lista
dado_t l_dado_fim(Lista l);

// retorna o dado na posição pos da lista
dado_t l_dado_pos(Lista l, int pos);

// remove e retorna o dado no início da lista
dado_t l_remove_inicio(Lista l);

// remove e retorna o dado no final da lista
dado_t l_remove_fim(Lista l);

// remove e retorna o dado na posição pos da lista
dado_t l_remove_pos(Lista l, int pos);

#endif // LISTA_H
