#ifndef DICIONARIO_H
#define DICIONARIO_H

// dicionario.h
// TAD para buscar valores associados a chaves
// l226b

#include <stdbool.h>
#include <stdlib.h>

// tipos de chave e valor.
// ambos são ponteiros para 'void', que é o tipo em C para "ponteiro
//   para qualquer coisa", ou "ponteiro para algo que não sei o que é".
// o dicionário só armazena os ponteiros.
// alocação e liberação de memória associada a chaves e valores é
//   responsabilidade do usuário.
// uma chave que tenha sido colocada no dicionário não pode ser alterada
//   enquanto não for removida.
typedef void *chave_t;
typedef void *valor_t;

// tipo "ponteiro para função de comparação entre duas chaves"
typedef bool (*comparador_t)(chave_t a, chave_t b);

// valor usado para indicar um valor inexistente
#define VALOR_NÃO_EXISTE NULL

// declaração de tipo opaco do dicionário
typedef struct dicionário *Dicionário;

// funções que formam a interface de operações em um dicionário

// cria um novo dicionário, vazio
// as funções 'menor' e 'igual' serão usadas para comparar chaves
// 'menor' deve retornar true se a primeira chave for menor que a segunda
// 'igual' deve retornar true se as chaves forem iguais
Dicionário dic_cria(comparador_t menor, comparador_t igual);

// libera a memória ocupada pelo dicionário 'd'
void dic_destrói(Dicionário d);

// busca no dicionário 'd' e retorna o valor associado a 'chave'
// retorna VALOR_NÃO_EXISTE se não existir
valor_t dic_busca(Dicionário d, chave_t chave);

// insere 'valor', associado a 'chave' no dicionário 'd'
// retorna o valor anteriormente associado a 'chave', ou
//   VALOR_NÃO_EXISTE se não existia valor associado a 'chave'
valor_t dic_insere(Dicionário d, chave_t chave, valor_t valor);

// remove o valor associado a 'chave' do dicionário 'd'
// retorna o valor removido ou VALOR_NÃO_EXISTE
valor_t dic_remove(Dicionário d, chave_t chave);

// chama a função 'f' para todas as chaves e valores do dicionário 'd'
void dic_para_todos(Dicionário d, void (*f)(chave_t, valor_t));

// inicia um percurso das tuplas no dicionário 'd'
// cada tupla é obtida com dic_próximo()
// o dicionário não deve ser alterado durante um percurso
void dic_inicia_percurso(Dicionário d);

// retorna em *pchave e *pvalor a próxima tupla de um percurso iniciado
//   por uma chamada a dic_inicia_percurso (se não forem NULL)
// retorna 'true' se havia tupla ou 'false' se o percurso já terminou
bool dic_próximo(Dicionário d, chave_t *pchave, valor_t *pvalor);

#endif // DICIONARIO_H
