#include "dicionario.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// implementação de um cronômetro
typedef struct timespec crono;

// inicializa um cronômetro com a hora atual
void crono_inicia(crono *c)
{
    clock_gettime(CLOCK_MONOTONIC, c);
}

// retorna o tempo passado desde que o cronômetro *c foi iniciado, em segundos
double crono_parcial(crono *c)
{
    crono agora;
    clock_gettime(CLOCK_MONOTONIC, &agora);

    double segundos = agora.tv_sec - c->tv_sec;
    double nanosegundos = agora.tv_nsec - c->tv_nsec;
    return segundos + 1e-9 * nanosegundos;
}

// funções de comparação de chaves
bool chaves_são_iguais(chave_t a, chave_t b)
{
  int *pa = a;
  int *pb = b;
  return *pa == *pb;
}

bool chave_é_menor(chave_t a, chave_t b)
{
  int *pa = a;
  int *pb = b;
  return *pa < *pb;
}

// aloca e preenche o vetor com 'n' valores aleatórios ordenados, sem repetição
int *cria_vetor(int n)
{
  int *vet = malloc(n * sizeof(*vet));
  int v = 0;
  int dmax = RAND_MAX / (n / 2 + 1);
  for (int i = 0; i < n; i++) {
    v += rand() % dmax + 1;
    vet[i] = v;
  }
  return vet;
}

// altera a ordem dos valores em vet, aleatoriamente
void embaralha_vetor(int n, int vet[n])
{
  for (int i = 0; i < n; i++) {
    int j = rand() % n;
    int t = vet[i];
    vet[i] = vet[j];
    vet[j] = t;
  }
}

void cronometra(Dicionário d, int n, int vet[n],
                double *t_ins, double *t_bus1, double *t_bus2, double *t_rem)
{
  // tempo para inserir n valores de vet em d
  crono c;
  crono_inicia(&c);
  for (int i = 0; i < n; i++) {
    dic_insere(d, &vet[i], &vet[i]);
  }
  *t_ins = crono_parcial(&c);

  // tempo para buscar todos os valores inseridos
  crono_inicia(&c);
  for (int i = 0; i < n; i++) {
    int *p = dic_busca(d, &vet[i]);
    if (p == VALOR_NÃO_EXISTE) {
      printf("erro na busca, não encontrou i%d, v%d\n", i, vet[i]);
    } else if (*p != vet[i]) {
      printf("erro na busca, retornou valor errado i%d, v%d r%d\n",
             i, vet[i], *p);
    }
  }
  *t_bus1 = crono_parcial(&c);

  // tempo para buscar n valores aleatórios em d (maioria não será encontrada)
  crono_inicia(&c);
  for (int i = 0; i < n; i++) {
    int r = rand();
    dic_busca(d, &r);
  }
  *t_bus2 = crono_parcial(&c);

  // tempo para remover todos os valores em d, um por um
  crono_inicia(&c);
  for (int i = 0; i < n; i++) {
    int *p = dic_remove(d, &vet[i]);
    if (p == VALOR_NÃO_EXISTE) {
      printf("erro na remoção, não encontrou i%d, v%d\n", i, vet[i]);
    } else if (*p != vet[i]) {
      printf("erro na remoção, retornou valor errado i%d, v%d r%d\n",
             i, vet[i], *p);
    }
  }
  *t_rem = crono_parcial(&c);
}

int main(int argc, char *argv[argc])
{
  int tam_vet = 10000;
  bool embaralha = true;
  if (argc > 1) sscanf(argv[1], "%d", &tam_vet);
  if (argc > 2) embaralha = (argv[2][0] != 'o');

  int *vet = cria_vetor(tam_vet);
  if (embaralha) embaralha_vetor(tam_vet, vet);

  Dicionário d = dic_cria(chave_é_menor, chaves_são_iguais);
  double ti, tre, trr, tr;

  printf("%s %s ", argv[0], embaralha ? "emb" : "ord");
  fflush(stdout);
  cronometra(d, tam_vet, vet, &ti, &tre, &trr, &tr);
  printf("ins %f, bex %f, bale %f, rem %f\n", ti, tre, trr, tr);

  free(vet);
  dic_destrói(d);
}
