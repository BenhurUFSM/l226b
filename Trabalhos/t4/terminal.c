#include "terminal.h"

#include <stdio.h>
#include <stdlib.h>


// salva ou recupera o conteúdo da tela
static void t_seleciona_tela_alternativa(bool alt)
{
  if (alt) {
    printf("\e[?1049h");
  } else {
    printf("\e[?1049l");
  }
}

void t_configura()
{
  // salva o conteúdo da tela
  t_seleciona_tela_alternativa(true);
  // coloca o terminal em modo cru
  if (system("stty raw opost -echo min 0 time 1") != 0) {
    perror("erro na execução de system(\"stty\")");
    fprintf(stderr, "você tem o programa stty instalado?\n");
    exit(1);
  }
  // configura leitura não bufferizada
  if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
    perror("erro na execução de setvbuf()");
    exit(1);
  }
  t_mostra_cursor(false);
  t_limpa();
}

void t_normaliza()
{
  system("stty sane");
  // recupera o conteúdo da tela
  t_seleciona_tela_alternativa(false);
  t_mostra_cursor(true);
}

void t_mostra_cursor(bool mostra)
{
  if (mostra) {
    printf("\x1b[?25h");
  } else {
    printf("\x1b[?25l");
  }
}

void t_limpa()
{
  printf("\e[2J");
}

void t_posiciona(posição pos)
{
  printf("\e[%d;%dH", pos.linha, pos.coluna);
}

void t_seleciona_cor_normal()
{
  printf("\e[m");
}

void t_seleciona_cor(cor fundo, cor letra)
{
  printf("\e[48;2;%d;%d;%dm", fundo.vermelho, fundo.verde, fundo.azul);
  printf("\e[38;2;%d;%d;%dm", letra.vermelho, letra.verde, letra.azul);
}

char t_lê_tecla()
{
  // força o envio de tudo que foi escrito na tela ao SO
  fflush(stdout);
  // lê um caractere e retorna
  char c;
  if (fread(&c, 1, 1, stdin) == 1)
    return c;
  // se a leitura não deu certo, retorna 0
  return 0;
}
