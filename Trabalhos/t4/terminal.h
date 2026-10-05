// terminal.h
// ----------
// controle do terminal
// l226b

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>

// tipo de dados para representar uma cor
// os componentes da cor são valores entre 0 e 255
typedef struct {
  unsigned char vermelho;
  unsigned char verde;
  unsigned char azul;
} cor;

// tipo de dados para representar uma posição na tela
typedef struct {
  unsigned char linha;
  unsigned char coluna;
} posição;

// operações

// configura o terminal
// coloca-o no modo "cru", para permitir a leitura de cada caractere digitado
//   sem esperar pelo "enter".
// deve ser chamada antes de qualquer outra função deste arquivo (no início
//   do programa)
void t_configura();

// devolve o terminal para o estado normal
// deve ser chamada quando após encerrar o uso do terminal (no final do
//   programa)
void t_normaliza();

// limpa a tela
void t_limpa();

// posiciona o cursor (0,0 é o canto superior esquerdo)
void t_posiciona(posição pos);

// seleciona a cor normal (como configurado no terminal) para as próximas
//   impressões
void t_seleciona_cor_normal(void);

// seleciona a cor do fundo e das letras para as próximas impressões
void t_seleciona_cor(cor fundo, cor letra);

// seleciona se o cursor aparece ou não
void t_mostra_cursor(bool mostra);

// retorna o próximo caractere lido do teclado
// caso não exista caractere digitado, retorna 0
char t_lê_tecla();

#endif // TERMINAL_H
