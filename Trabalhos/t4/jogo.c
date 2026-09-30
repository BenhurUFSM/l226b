// implementa uma cobrinha semovimentante, com direção controlável
// ganha pontos enquanto anda e mais quando come frutas
// morre se bater em si mesma ou na parede ou num obstáculo
//

#include "jogo.h"

#include "fila.h"
#include "terminal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_OBSTÁCULOS 4 // quantos obstáculos simultâneos estão no tabuleiro
#define TAM_INI_COBRA 5 // quantas partes tem o corpo da cobra no início
#define JOGO_NUM_ENTRADAS 3 // quantos valores são usados para controlar o jogo
#define PONTOS_POR_FRUTA 123 // quantos pontos se ganha comendo a fruta
#define AUMENTO_POR_FRUTA 5 // quanto aumenta a cada alimentação
#define AUTONOMIA 200 // quantos passos a cobrinha pode dar sem comer

// um retângulo na tela
typedef struct {
    posição inf;
    posição sup;
} retângulo;

// retorna a posição do centro do retângulo
posição retângulo_centro(retângulo r)
{
    return (posição) {
        .linha =  (r.inf.linha  + r.sup.linha ) / 2,
        .coluna = (r.inf.coluna + r.sup.coluna) / 2
    };
}

// retorna true se a posição p está dentro o retângulo r
bool tá_dentro(posição p, retângulo r)
{
    if (p.linha <= r.inf.linha) return false;
    if (p.coluna <= r.inf.coluna) return false;
    if (p.linha >= r.sup.linha) return false;
    if (p.coluna >= r.sup.coluna) return false;
    return true;
}

// uma direção na tela
typedef enum { direita,
    esquerda,
    cima,
    baixo } direção;

typedef enum { reto,
    pra_direita,
    pra_esquerda } lado_da_curva;

// uma cobra
typedef struct {
    Fila corpo; // posições dos pedacinhos da cobra
    posição pos_cabeca; // onde está a cabeça da cobra
    direção direção; // para que lado a cobra está indo
} cobra;

// o estado do jogo
struct jogo {
    retângulo tela; // área onde a cobrinha passeia
    cobra aninha; // a cobrinha
    int aumentando; // número de peças que faltam na cobrinha
    Fila obstáculos; // objetos espalhados para complicar a vida da cobrinha
    posição fruta; // onde está o objeto de desejo da cobrinha
    int pontos; // quantos pontos foram obtidos até agora
    enum { normal,
        terminando,
        terminado } estado;
    int passos; // quantos passos a cobrinha já deu
    int passo_última_fruta; // quantos passos ela tinha dado quando comeu a última fruta
    int max_passos; // número máximo de passos nesta partida
};

// cores dos objetos na tela
cor fundo = { 0, 0, 0 };
cor cobra_morrendo = { 200, 30, 30 };
cor cobra_normal = { 150, 150, 0 };
cor cor_obstáculo = { 200, 0, 0 };
cor cor_contorno = { 255, 50, 200 };

// formas dos objetos na tela
char* desenho_rabo = "."; // \u25e6
char* desenho_corpo = "O"; // \u25cb
char* desenho_cabeça = ":"; // \u2687
char* desenho_fruta = "*"; //
char* desenho_obstáculo = "X"; //

// desenha os pedaços da cobrinha na tela
void desenha_cobra(Fila f, bool terminando)
{
    if (terminando) {
        t_seleciona_cor(fundo, cobra_morrendo);
    } else {
        t_seleciona_cor(fundo, cobra_normal);
    }
    posição pos;
    f_inicia_percurso(f, 0);
    f_próximo(f, &pos);
    t_posiciona(pos);
    fputs(desenho_rabo, stdout);
    while (f_próximo(f, &pos)) {
        t_posiciona(pos);
        fputs(desenho_corpo, stdout);
    }
    t_posiciona(pos);
    fputs(desenho_cabeça, stdout);
}

// desenha os obstáculos na tela
void desenha_obstáculos(Fila f)
{
    t_seleciona_cor(fundo, cor_obstáculo);
    f_inicia_percurso(f, 0);
    posição pos;
    while (f_próximo(f, &pos)) {
        t_posiciona(pos);
        fputs(desenho_obstáculo, stdout);
    }
    t_seleciona_cor_normal();
}

// desenha a fruta na tela
void desenha_fruta(posição pos)
{
    t_posiciona(pos);
    fputs(desenho_fruta, stdout);
}

// retorna a nova posição de por, quando se move na direção dada
posição avanca_pos(posição pos, direção dir)
{
    switch (dir) {
    case direita:
        pos.coluna++;
        break;
    case esquerda:
        pos.coluna--;
        break;
    case cima:
        pos.linha--;
        break;
    case baixo:
        pos.linha++;
        break;
    }
    return pos;
}

// retorna true se a fila de posições f contém a posição pos
bool fila_contém(Fila f, posição pos)
{
    posição p;
    f_inicia_percurso(f, 0);
    while (f_próximo(f, &p)) {
        if (pos.linha == p.linha && pos.coluna == p.coluna) {
            return true;
        }
    }
    return false;
}

// desenha a borda de um retângulo na tela
void desenha_retângulo(retângulo r)
{
    t_posiciona(r.inf);
    fputs("╭", stdout);
    for (int c = r.inf.coluna + 1; c < r.sup.coluna; c++) {
        fputs("─", stdout);
    }
    fputs("╮", stdout);
    for (int l = r.inf.linha + 1; l < r.sup.coluna; l++) {
        t_posiciona((posição) { l, r.inf.coluna });
        fputs("│", stdout);
        t_posiciona((posição) { l, r.sup.coluna });
        fputs("│", stdout);
    }
    t_posiciona((posição) { r.sup.linha, r.inf.coluna });
    fputs("╰", stdout);
    for (int c = r.inf.coluna + 1; c < r.sup.coluna; c++) {
        fputs("─", stdout);
    }
    fputs("╯", stdout);
}

// retorna uma posição aleatória dentro do retângulo (fora das margens)
posição sorteia_pos(retângulo r)
{
    posição pos;
    int nl = r.sup.linha - r.inf.linha - 1;
    int nc = r.sup.coluna - r.inf.coluna - 1;
    pos.linha = rand() % nl + r.inf.linha + 1;
    pos.coluna = rand() % nc + r.inf.coluna + 1;
    return pos;
}

// preenche a fila de obstáculos com posições aleatórias
void sorteia_obstáculos(Jogo j)
{
    while (!f_tá_vazia(j->obstáculos)) {
        f_remove(j->obstáculos, NULL);
    }
    int nobs = 0;
    while (nobs < N_OBSTÁCULOS) {
        posição pos = sorteia_pos(j->tela);
        if (!fila_contém(j->aninha.corpo, pos)
                && !fila_contém(j->obstáculos, pos)) {
            f_insere(j->obstáculos, &pos);
            nobs++;
        }
    }
}

// sorteia a posição onde fica a fruta
void sorteia_fruta(Jogo j)
{
    posição pos;
    do {
        pos = sorteia_pos(j->tela);
    } while (fila_contém(j->aninha.corpo, pos) || fila_contém(j->obstáculos, pos));
    j->fruta = pos;
}

Jogo jogo_cria(int max_passos)
{
    Jogo j = malloc(sizeof(*j));
    assert(j != NULL);
    j->tela = (retângulo) { { 2, 1 }, { 24, 50 } };
    j->aninha.corpo = f_cria(sizeof(posição));
    j->aninha.direção = cima;
    j->aninha.pos_cabeca = retângulo_centro(j->tela);
    f_insere(j->aninha.corpo, &j->aninha.pos_cabeca);
    j->aumentando = TAM_INI_COBRA;
    j->obstáculos = f_cria(sizeof(posição));
    sorteia_obstáculos(j);
    sorteia_fruta(j);
    j->pontos = 0;
    j->estado = normal;
    j->passos = 0;
    j->passo_última_fruta = 0;
    j->max_passos = max_passos;
    return j;
}

void jogo_destrói(Jogo j)
{
    f_destrói(j->aninha.corpo);
    f_destrói(j->obstáculos);
    free(j);
}

int jogo_num_entradas(Jogo j)
{
    return JOGO_NUM_ENTRADAS;
}

void jogo_desenha_tela(Jogo j)
{
    t_limpa();

    // desenha o contorno da janela
    t_seleciona_cor(fundo, cor_contorno);
    desenha_retângulo(j->tela);

    // desenha os objetos
    desenha_obstáculos(j->obstáculos);
    desenha_fruta(j->fruta);
    desenha_cobra(j->aninha.corpo, j->estado == terminando);

    // desenha a pontuação
    t_posiciona((posição) { 1, 1 });
    printf("%d", j->pontos);
}

// muda a direção da cobra de acordo com o lado da curva
void faz_curva(Jogo j, lado_da_curva lado)
{
    // qual a próxima direção se está indo para tal direção e faz tal curva?
    direção prox_direita[] = {
        [cima] = direita,
        [direita] = baixo,
        [baixo] = esquerda,
        [esquerda] = cima,
    };
    direção prox_esquerda[] = {
        [cima] = esquerda,
        [direita] = cima,
        [baixo] = direita,
        [esquerda] = baixo,
    };
    if (lado == pra_esquerda) {
        j->aninha.direção = prox_esquerda[j->aninha.direção];
    } else if (lado == pra_direita) {
        j->aninha.direção = prox_direita[j->aninha.direção];
    } // se for reto, não muda
}

posição movimenta_cobrinha(Jogo j)
{
    // calcula a nova posição da cabeça
    posição pos = avanca_pos(j->aninha.pos_cabeca, j->aninha.direção);
    f_insere(j->aninha.corpo, &j->aninha.pos_cabeca);

    // se a cobra não tiver aumentando, tira o último pedaço
    if (j->aumentando == 0) {
        f_remove(j->aninha.corpo, NULL);
    } else {
        j->aumentando--;
    }

    j->passos++;
    return pos;
}

bool cobrinha_bateu(Jogo j, posição pos)
{
    // vê se bateu na borda
    if (!tá_dentro(pos, j->tela)) return true;
    // vê se bateu em um obstáculo
    if (fila_contém(j->obstáculos, pos)) return true;
    // vê se bateu nela mesma
    if (fila_contém(j->aninha.corpo, pos)) return true;

    return false;
}

void vê_se_acertou_a_fruta(Jogo j, posição pos)
{
    if (pos.linha == j->fruta.linha && pos.coluna == j->fruta.coluna) {
        j->pontos += PONTOS_POR_FRUTA;
        j->aumentando += AUMENTO_POR_FRUTA;
        sorteia_fruta(j);
        j->passo_última_fruta = j->passos;
    }
}

// movimenta a cobrinha
// - muda a cabeça para uma nova posição, de acordo com a direção
// - se não tiver aumentando, remove a posição do rabo
// - verifica se bateu em algo e reage de acordo
// - verifica se está há muito tempo sem pegar a fruta, ou se há muito tempo
//   jogando, para garantir que o jogo termina (importante durante o
//   treinamento)
void jogo_avança(Jogo j)
{
    if (j->estado == terminando) {
        f_remove(j->aninha.corpo, NULL);
        if (f_tá_vazia(j->aninha.corpo)) {
            j->estado = terminado;
        }
        return;
    }
    posição pos = movimenta_cobrinha(j);

    if (cobrinha_bateu(j, pos)) j->estado = terminando;
    else vê_se_acertou_a_fruta(j, pos);
    j->aninha.pos_cabeca = pos;

    j->pontos++;
    if (j->passos - j->passo_última_fruta > AUTONOMIA) {
        j->estado = terminando;
    }
    if (j->max_passos != 0 && j->passos > j->max_passos) {
        j->estado = terminando;
    }
}

void jogo_processa_entradas(Jogo j, float entradas[JOGO_NUM_ENTRADAS])
{
    //  as entradas são o quanto se acha que tem que ir reto ou pra direita ou
    //  esquerda
    if (entradas[1] > entradas[0] && entradas[1] > entradas[2])
        faz_curva(j, pra_direita);
    if (entradas[2] > entradas[0] && entradas[2] > entradas[1])
        faz_curva(j, pra_esquerda);
}

bool jogo_terminou(Jogo j)
{
    return j->estado == terminado;
}

int jogo_pontos(Jogo j)
{
    return j->pontos;
}
