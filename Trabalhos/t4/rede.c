#include "rede.h"
#include "grafo.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct rede {
  Grafo grafo;
  Fila  ordem;
  int   num_entradas;
  int   num_saídas;
};

// função auxiliar para colocar o valor de um neurônio no intervalo (0,1)
static inline float sigmoid(float x)
{
  return 1 / (1 + expf(-x));
}

static void rede_calcula_valores(Rede rede)
{
  // calcula o valor de cada neurônio, na ordem da fila
  // o valor de um neurônio é o somatório do valor de cada neurônio conectado
  //   às suas entradas multiplicado pelo peso da sinapse que faz essa conexão.
  // esse valor depois é limitado pela função sigmoide.
  int       nó_origem, nó_destino;
  neurônio *neurônio_origem, *neurônio_destino;
  sinapse  *sina;
  f_inicia_percurso(rede->ordem, 0);
  while (f_próximo(rede->ordem, &nó_destino)) {
    // o valor das entradas é fornecido, não é calculado
    if (nó_destino < rede->num_entradas) continue;
    neurônio_destino = g_valor_do_nó(rede->grafo, nó_destino);
    float soma       = 0;
    g_arestas_que_chegam(rede->grafo, nó_destino);
    while ((sina = g_próxima_aresta(rede->grafo, &nó_origem)) != NULL) {
      if (!sina->habilitada) continue;
      neurônio_origem = g_valor_do_nó(rede->grafo, nó_origem);
      soma += neurônio_origem->valor * sina->peso;
    }
    neurônio_destino->valor = sigmoid(soma);
  }
}

void rede_calcula_ordem(Rede rede)
{
  if (rede->ordem != NULL) f_destrói(rede->ordem);
  rede->ordem = g_ordem_topológica(rede->grafo);
}

// Cálculo dos valores da rede neural (produz as saídas à partir das entradas)
void rede_calcula(Rede rede, float *entradas, float *saídas)
{
  if (rede == NULL) return;
  neurônio *neuro;
  // copia os valores de entrada para os neurônios de entrada da rede
  // o valor do último neurônio de entrada não vem da entrada, é sempre -1
  for (int entrada = 0; entrada < rede->num_entradas - 1; entrada++) {
    neuro        = rede_neurônio_entrada(rede, entrada);
    neuro->valor = entradas[entrada];
  }
  neuro        = rede_neurônio_entrada(rede, rede->num_entradas - 1);
  neuro->valor = -1;
  // zera os valores de saída da rede (dessa forma, se algum neurônio de
  //   saída estiver desconectado, não terá um valor não inicializado)
  for (int saída = 0; saída < rede->num_saídas; saída++) {
    neuro        = rede_neurônio_saída(rede, saída);
    neuro->valor = 0;
  }

  rede_calcula_valores(rede);

  // copia os valores dos neurônios de saída para o vetor de saída
  for (int saída = 0; saída < rede->num_saídas; saída++) {
    neuro         = rede_neurônio_saída(rede, saída);
    saídas[saída] = neuro->valor;
  }
}

// cria uma rede vazia
Rede rede_cria_vazia(int num_entradas, int num_saídas)
{
  Rede self = malloc(sizeof(*self));
  if (self == NULL) return NULL;
  self->grafo = g_cria(sizeof(neurônio), sizeof(sinapse));
  if (self->grafo == NULL) {
    free(self);
    return NULL;
  }
  self->ordem        = NULL;
  self->num_entradas = num_entradas;
  self->num_saídas   = num_saídas;
  return self;
}

// cria uma rede neural a partir de um arquivo
Rede rede_cria(char *nome)
{
  Rede  self = NULL;
  FILE *arq  = fopen(nome, "r");
  if (arq == NULL) goto erro;
  int n_nós, n_entradas, n_saídas;
  if (fscanf(arq, "%d%d%d", &n_nós, &n_entradas, &n_saídas) != 3) goto erro;
  self = rede_cria_vazia(n_entradas, n_saídas);
  if (self == NULL) goto erro;
  for (int i = 0; i < n_nós; i++) {
    neurônio neurônio = {.valor = 0};
    if (fscanf(arq, "%d", &neurônio.id) != 1) goto erro;
    g_insere_nó(self->grafo, &neurônio);
  }
  for (;;) {
    int nó_origem;
    if (fscanf(arq, "%d", &nó_origem) != 1) goto erro;
    if (nó_origem == -1) break;
    int   nó_destino, habilitada;
    float peso;
    if (fscanf(arq, "%d%f%d", &nó_destino, &peso, &habilitada) != 3) goto erro;
    sinapse sinapse = {.peso = peso, .habilitada = (habilitada == 1)};
    g_altera_valor_da_aresta(self->grafo, nó_origem, nó_destino, &sinapse);
  }
  rede_calcula_ordem(self);
  fclose(arq);
  return self;

erro:
  fprintf(stderr, "erro na leitura do grafo de '%s'\n", nome);
  if (arq != NULL) fclose(arq);
  if (self != NULL) {
    g_destrói(self->grafo);
    free(self);
  }
  return NULL;
}

// libera a memória ocupada por uma rede neural
void rede_destrói(Rede self)
{
  if (self == NULL) return;
  g_destrói(self->grafo);
  if (self->ordem != NULL) f_destrói(self->ordem);
  free(self);
}

int rede_num_neurônios(Rede self)
{
  return g_num_nós(self->grafo);
}

int rede_num_entradas(Rede self)
{
  return self->num_entradas;
}

int rede_num_saídas(Rede self)
{
  return self->num_saídas;
}

int rede_insere_neurônio(Rede self, neurônio neuro)
{
  return g_insere_nó(self->grafo, &neuro);
}

void rede_remove_neurônio(Rede self, int pos)
{
  g_remove_nó(self->grafo, pos);
}

neurônio *rede_neurônio(Rede self, int pos)
{
  return g_valor_do_nó(self->grafo, pos);
}

neurônio *rede_neurônio_entrada(Rede self, int pos)
{
  return g_valor_do_nó(self->grafo, pos);
}

neurônio *rede_neurônio_saída(Rede self, int pos)
{
  return g_valor_do_nó(self->grafo, pos + self->num_entradas);
}

void rede_remove_sinapse(Rede self, int pos_origem, int pos_destino)
{
  g_altera_valor_da_aresta(self->grafo, pos_origem, pos_destino, NULL);
}

bool rede_insere_sinapse(Rede self, int pos_origem, int pos_destino,
                         sinapse sina)
{
  g_altera_valor_da_aresta(self->grafo, pos_origem, pos_destino, &sina);
  if (g_tem_ciclo(self->grafo)) {
    rede_remove_sinapse(self, pos_origem, pos_destino);
    return false;
  }
  return true;
}

sinapse *rede_sinapse(Rede self, int pos_org, int pos_dest)
{
  return g_valor_da_aresta(self->grafo, pos_org, pos_dest);
}

void rede_sinapses_que_partem(Rede self, int pos_org)
{
  g_arestas_que_partem(self->grafo, pos_org);
}

sinapse *rede_próxima_sinapse(Rede self, int *pos_vizinho)
{
  return g_próxima_aresta(self->grafo, pos_vizinho);
}
