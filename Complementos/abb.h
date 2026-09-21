#ifndef ABB_H
#define ABB_H

// abb.h
// árvore binária de busca
// l226b

#include <stdbool.h>

// tipos de chave e valor.
// ambos são ponteiros para 'void', que é o tipo em C para "ponteiro
//   para qualquer coisa", ou "ponteiro para algo que não sei o que é".
// a árvore só armazena os ponteiros.
// alocação e liberação de memória associada a chaves e valores é
//   responsabilidade do usuário.
// uma chave que tenha sido colocada na árvore não pode ser alterada
//   enquanto não for removida.
typedef void *chave_t;
typedef void *valor_t;

// tipo "ponteiro para função de comparação entre duas chaves"
typedef bool (*comparador_t)(chave_t a, chave_t b);

// valor usado para indicar um valor inexistente
#define VALOR_NÃO_EXISTE NULL

// declaração de tipo opaco da árvore
typedef struct abb *ABB;

// funções que formam a interface de operações em uma ABB

// cria uma nova árvore, vazia
ABB abb_cria();

// libera a memória ocupada pela árvore 'a'
void abb_destrói(ABB a);

// retorna true se a árvore 'a' for vazia
bool abb_é_vazia(ABB a);

// busca na árvore 'a' e retorna o valor associado a 'chave'
// retorna VALOR_NÃO_EXISTE se não existir
valor_t abb_busca(ABB a, chave_t chave, comparador_t menor, comparador_t igual);

// insere 'valor' associado a 'chave' na árvore 'a'
// retorna a nova árvore
// se já existir um valor associado a essa chave na árvore,
//   será substituído pelo novo valor
// se 'panterior' não for NULL, *panterior receberá o valor anterior
//   ou VALOR_NÃO_EXISTE
ABB abb_insere(ABB a, chave_t chave, valor_t valor, valor_t *panterior,
	       comparador_t menor, comparador_t igual);

// remove o nó contendo 'chave' da árvore 'a'
// retorna a nova árvore
// se 'pvalor' não for NULL, *pvalor receberá o valor removido
//   ou VALOR_NÃO_EXISTE
ABB abb_remove(ABB a, chave_t chave, valor_t *pvalor,
	       comparador_t menor, comparador_t igual);

#endif // ABB_H
