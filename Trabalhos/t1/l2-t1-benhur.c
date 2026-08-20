// l2 2026b t1 -- space invaders 1D
// implementação do Benhur

// includes {{{1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// terminal {{{1

// Configura o terminal para o modo "cru", para permitir a leitura
//   de cada caractere digitado sem esperar pelo "enter".
void configura_terminal()
{
    if (system("stty raw opost -echo min 0 time 1") != 0) {
        perror("erro na execução de system(\"stty\")");
        fprintf(stderr, "você tem o programa stty instalado?\n");
        exit(1);
    };
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        perror("erro na execução de setvbuf()");
        exit(1);
    }
}

// Configura o terminal para o modo normal, com bufferização por linha.
void normaliza_terminal()
{
    system("stty sane");
}

// Lê um caractere do teclado.
// Retorna o código do caractere lido ou 0 se nada tenha sido digitado.
// Só funciona corretamente se o terminal estiver em modo "cru".
char lechar()
{
    // força o envio de tudo que foi escrito na tela ao SO
    fflush(stdout);
    // lê um caractere e retorna
    char c;
    if (fread(&c, 1, 1, stdin) == 1) return c;
    // se a leitura não deu certo, retorna 0
    return 0;
}

// crono {{{1
// implementação de um cronômetro
typedef struct timespec crono;

// Inicializa um cronômetro com a hora atual.
void crono_inicia(crono *c)
{
    clock_gettime(CLOCK_MONOTONIC, c);
}

// Retorna o tempo passado desde que o cronômetro *c foi iniciado, em segundos.
double crono_parcial(crono *c)
{
    crono agora;
    clock_gettime(CLOCK_MONOTONIC, &agora);

    double segundos = agora.tv_sec - c->tv_sec;
    double nanosegundos = agora.tv_nsec - c->tv_nsec;
    return segundos + 1e-9 * nanosegundos;
}

// teclas {{{1
// teclas para controle do programa
#define T_DESISTE 'q'
#define T_TIRO    '\r'
#define T_ARMA    '\t'
#define T_SOM     ' '

// constantes {{{1

// número de escudos que o jogador pode ter
#define N_ESCUDO 3
// número máximo de ataques simultâneos
#define N_ATAQUE 10
// número de posições na tela em pode existir algum ataque ativo
#define N_POS (N_ESCUDO + N_ATAQUE)
// número de pontuações a guardar para a posteridade
#define N_RECORDES 3
// nome do arquivo de recordes
#define NOME_RECORDES "recordes"

// tipos {{{1

// tipo que representa um escudo ou nada ou ataque, pode ser ') 0-9Nn'
typedef char ataque;

// estrutura principal, que contém todo o estado do jogo
typedef struct {
    // quantos pontos o jogador já fez na partida
    int pontos;
    // quantos tiros ainda restam na onda corrente
    int tiros;
    // qual a arma atual do jogador
    ataque arma;
    // os ataques ativos (e os escudos)
    ataque objetos[N_POS + 1];
    // posição no vetor objetos onde os novos ataques aparecem
    int pos_nascimento;
    // quantos ataques inativos restam na onda atual
    int n_ataque;
    // quantos intervalos de tempo já passaram na onda atual
    int idade;
    // quantos segundos dura um intervalo de tempo
    double intervalo;
    // número da onda atual
    int n_onda;
    // true quando terminar a onda atual
    bool terminou_onda;
    // true se a partida terminou (jogador foi morto ou desistiu)
    bool terminou_partida;
    // true se a onda atual é noturna
    bool noite;
    // data de início da onda atual
    crono inicio;
    // pontuações recordistas
    int recordes[3];
} estado;

// som {{{1

// Acrescenta à string s o nome do arquivo de som para o ataque a.
void acrescenta_som(char *s, ataque a)
{
    char ataques[] = "0123456789nN) ";
    char *som[] = {
        "0.3.wav ",  "1.3.wav ",  "2.3.wav ",  "3.3.wav ", "4.3.wav ",
        "5.3.wav ",  "6.3.wav ",  "7.3.wav ",  "8.3.wav ", "9.3.wav ",
        "11.3.wav ", "11.3.wav ", "12.2.wav ", "x.2.wav ",
    };
    for (int i = 0; ataques[i] != '\0'; i++) {
        if (a == ataques[i]) {
            strcat(s, "Sons/");
            strcat(s, som[i]);
            break;
        }
    }
}

// Chama um programa para tocar sons correspondentes aos ataques em 'som'.
// Espera terminar se pausa for true.
void toca_som(char *som, bool pausa)
{
    // usa o programa "aplay"
    // a opção "-q" serve para ele não escrever nada na tela
    // 250 char deve ser suficiente para até 13 sons
    char cmd[250] = "aplay -q ";

    for (char *s = som; *s != '\0'; s++) {
        acrescenta_som(cmd, *s);
    }
    if (!pausa) strcat(cmd, "&");
    system(cmd);
}

// Toca som correspondente à arma do jogador.
void sonoriza_arma(ataque a)
{
    char s[] = {a};
    toca_som(s, false);
}

// Toca som correspondente à arma quando acerta.
void sonoriza_tiro_certo(ataque a)
{
    char s[] = {a};
    toca_som(s, false);
}

// Toca som correspondente ao espaço quando erra.
void sonoriza_tiro_errado(ataque a)
{
    char s[] = " ";
    toca_som(s, false);
}

void sonoriza_novo_ataque(ataque a)
{
    char s[2] = {a};
    toca_som(s, false);
}

// Toca um som para marcar o fim de uma onda
void sonoriza_fim_de_onda()
{
    toca_som(" 56789", true);
}

// tela {{{1

// Desenha a tela do jogo.
void imprime_estado(estado *e)
{
    if (e->noite) {
        printf(" %d %-20s\r", e->pontos, "noite");
    } else {
        printf(" %d %d %c%-20s\r", e->pontos, e->tiros, e->arma, e->objetos);
    }
}

// processa tempo {{{1

// Verifica se um movimento irá destruir um escudo.
// Se for o caso, destroi o escudo e o ataque.
void vê_se_destroi_escudo(estado *e)
{
    // procura de tras pra diante, só o último escudo pode ser destruído
    for (int i = N_ESCUDO - 1; i >= 0; i--) {
        if (e->objetos[i] == ')') {
            if (e->objetos[i + 1] != ' ') {
                e->objetos[i] = e->objetos[i + 1] = ' ';
            }
            break;
        }
    }
}

// Move os ataques uma posição para a esquerda.
void move_inimigos(estado *e)
{
    vê_se_destroi_escudo(e);

    // move o que não for escudo
    for (int pos = 0; pos < e->pos_nascimento; pos++) {
        if (e->objetos[pos] != ')') {
            e->objetos[pos] = e->objetos[pos + 1];
        }
    }
    // garante que tem um espaço no final
    e->objetos[e->pos_nascimento] = ' ';
}

// Coloca um ataque aleatório na posição final.
void cria_inimigo(estado *e)
{
    ataque a;
    // gera um ataque aleatório
    int i = rand() % 11;
    if (i == 10) {
        a = 'N';
    } else {
        // de noite, só tem ataques pares
        if (e->noite && (i % 2 == 1)) i--;
        a = i + '0';
    }

    // coloca o ataque na posição de nascimento
    e->objetos[e->pos_nascimento] = a;

    // resta um ataque a menos
    e->n_ataque--;

    // toca o som correspondente ao ataque
    sonoriza_novo_ataque(a);
}

// Avança o inimigo, se passou mais um período de tempo.
void verifica_tempo(estado *e)
{
    double t_onda = crono_parcial(&e->inicio);
    int período = t_onda / e->intervalo;
    if (período <= e->idade) return;
    // passou mais um período!
    e->idade++;
    // se tem um ataque na posição inicial, já era
    if (e->objetos[0] != ')' && e->objetos[0] != ' ') {
        e->terminou_onda = true;
        e->terminou_partida = true;
        return;
    }
    // move os ataques
    move_inimigos(e);
    if (e->n_ataque > 0) cria_inimigo(e);
}

// processa teclado {{{1

// troca a arma do jogador
void incrementa_arma(estado *e)
{
    if (e->arma == 'n') {
        e->arma = '0';
    } else {
        e->arma += e->noite ? 2 : 1;
        if (e->arma == '9' + 1) e->arma = 'n';
    }
    sonoriza_arma(e->arma);
}

// verifica se todos os ataques foram destruídos
void vê_se_terminou_onda(estado *e)
{
    // se ainda tem ataque não ativo, não terminou
    if (e->n_ataque > 0) return;
    // procura algum ataque ativo
    for (int i = 0; i <= e->pos_nascimento; i++) {
        ataque a = e->objetos[i];
        if (a >= '0' && a <= '9') return;
        if (a == 'n' || a == 'N') return;
    }

    // acabaram os ataques, terminou a onda.
    e->terminou_onda = true;
    // pontua os tiros e escudos restantes
    e->pontos += e->tiros * 2;
    for (int i = 0; i < N_ESCUDO; i++) {
        if (e->objetos[i] == ')') e->pontos += 10;
    }
}

// verifica se o tiro acerta algo na posição i
bool vê_se_acertou(estado *e, int i)
{
    if (e->arma == 'n' && e->objetos[i] == 'N') {
        // o primeiro tiro em uma nave não mata
        e->objetos[i] = 'n';
        return true;
    }
    if (e->objetos[i] == e->arma) {
        // acertou!  destroi o ataque
        e->objetos[i] = ' ';
        // calcula os pontos
        int pt = e->pos_nascimento - i + 1;
        if (e->arma == 'n') {
            pt *= 2;
        }
        e->pontos += pt;
        return true;
    }
    return false;
}

// processa um tiro do jogador
void atira(estado *e)
{
    // não dá para atirar se não tem mais tiros
    if (e->tiros == 0) return;
    // um tiro a menos
    e->tiros--;
    // procura um ataque correspondente à arma atual
    bool acertou = false;
    for (int i = 0; i <= e->pos_nascimento; i++) {
        if (vê_se_acertou(e, i)) {
            acertou = true;
            break;
        }
    }
    if (acertou) {
        vê_se_terminou_onda(e);
        sonoriza_tiro_certo(e->arma);
    } else {
        sonoriza_tiro_errado(e->arma);
    }
}

// jogador desistiu!
void desiste(estado *e)
{
    e->terminou_onda = true;
    e->terminou_partida = true;
}

// Toca sons correpondentes aos escudos e ataques.
// Espera terminar se for noturno.
void toca_sonar(estado *e)
{
    toca_som(e->objetos, e->noite);
}


// vê se o jogador teclou algo válido, e reaje de acordo
void verifica_teclado(estado *e)
{
    switch (lechar()) {
        case T_SOM:
            toca_sonar(e);
            break;
        case T_ARMA:
            incrementa_arma(e);
            break;
        case T_TIRO:
            atira(e);
            break;
        case T_DESISTE:
            desiste(e);
            break;
        default:
            break;
    }
}

// lê o teclado até que seja digitado q ou r
char q_ou_r()
{
    while (true) {
        char c = lechar();
        if (c == 'q' || c == 'r') return c;
    }
}

// inicialização {{{1

// decide se a onda vai ser diurna ou noturna
bool sorteia_noite(int onda)
{
    int probabilidade_diurno = 100 - 20 * (onda - 1);
    if (probabilidade_diurno < 20) probabilidade_diurno = 20;
    return rand() % 100 > probabilidade_diurno;
}

// Inicializa uma nova onda de ataques inimigos.
void inicia_onda(estado *e)
{
    e->noite = sorteia_noite(e->n_onda);
    if (e->noite) {
        e->n_ataque = 15;
        e->pos_nascimento = N_ESCUDO + N_ATAQUE / 2 - 1;
    } else {
        e->n_ataque = 20;
        e->pos_nascimento = N_ESCUDO + N_ATAQUE - 1;
    }
    for (int i = N_ESCUDO; i < N_POS; i++) {
        e->objetos[i] = ' ';
    }
    e->objetos[e->pos_nascimento + 1] = '\0';
    e->arma = '0';
    e->tiros = 30;
    e->intervalo = 2 * pow(0.9, e->n_onda);
    if (e->noite) e->intervalo *= 3;
    e->idade = 0;
    e->terminou_onda = false;
    e->n_onda++;
    crono_inicia(&e->inicio);
}

// Inicializa o estado para uma nova partida.
void inicia_partida(estado *e)
{
    e->pontos = 0;
    for (int i = 0; i < N_ESCUDO; i++) {
        e->objetos[i] = ')';
    }
    e->terminou_partida = false;
    e->n_onda = 0;
    e->terminou_partida = false;
}

// recordes {{{1

// lê o arquivo de recordes
void le_recordes(estado *e)
{
    FILE *arq = fopen(NOME_RECORDES, "r");
    int nlidos = 0;
    if (arq != NULL) {
        for (int i = 0; i < N_RECORDES; i++) {
            if (fscanf(arq, "%d", &e->recordes[i]) != 1) break;
            nlidos++;
        }
        fclose(arq);
    }
    if (nlidos != N_RECORDES) {
        for (int i = 0; i < N_RECORDES; i++) {
            e->recordes[i] = 0;
        }
    }
}

// grava os recordes no arquivo
void grava_recordes(estado *e)
{
    FILE *arq = fopen(NOME_RECORDES, "w");
    if (arq != NULL) {
        for (int i = 0; i < N_RECORDES; i++) {
            fprintf(arq, "%d\n", e->recordes[i]);
        }
        fclose(arq);
    }
}

// insere a pontuação atual nos recordes, se for o caso
// mantém os recordes em ordem
void verifica_recordes(estado *e)
{
    int menor = N_RECORDES;
    while (menor > 0 && e->recordes[menor - 1] < e->pontos) {
        menor--;
        e->recordes[menor + 1] = e->recordes[menor];
    }
    if (menor == N_RECORDES) return;
    e->recordes[menor] = e->pontos;
    grava_recordes(e);
}

// laços do jogo {{{1

// executa uma onda de ataques
void executa_onda(estado *e)
{
    inicia_onda(e);
    printf("\n");
    while (!e->terminou_onda) {
        verifica_teclado(e);
        verifica_tempo(e);
        imprime_estado(e);
    }
    printf("\n\nFim da onda %d\n", e->n_onda);
    printf("Pontos: %d\n", e->pontos);
    sonoriza_fim_de_onda();
}

void confirma_nova_onda(estado *e)
{
    if (e->terminou_partida) return;
    printf("Digite 'r' para a nova onda, 'q' para fim ");
    if (q_ou_r() == 'q') {
        e->terminou_partida = true;
    }
}

// joga uma partida
void executa_partida(estado *e)
{
    inicia_partida(e);
    while (!e->terminou_partida) {
        executa_onda(e);
        confirma_nova_onda(e);
    }
    verifica_recordes(e);
    printf("\n\nFim da partida\nTotal de pontos: %d\n", e->pontos);
    printf("Recordes:");
    for (int i = 0; i < N_RECORDES; i++) {
        printf(" %d", e->recordes[i]);
    }
    printf("\n");
}

// joga várias partidas
void joga(estado *e)
{
    do {
        executa_partida(e);
        printf("\nDigite 'r' para novo jogo, 'q' para fim ");
    } while (q_ou_r() != 'q');
}

// main {{{1

int main()
{
    srand(time(0));
    configura_terminal();

    estado e;
    le_recordes(&e);
    joga(&e);

    normaliza_terminal();
}

// vim: foldmethod=marker
