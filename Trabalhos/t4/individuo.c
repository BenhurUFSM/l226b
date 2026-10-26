#include "individuo.h"
#include "auxiliar.h"
#include "config.h"
#include "rede.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Um indivíduo contém uma rede neural e mais informações necessárias
//   para executar o algoritmo de evolução.
struct indivíduo {
  Rede rede;       // a rede neural da criatura
  int geração;     // em que geração ele foi criado
  int espécie;     // espécie a que ele pertence
  float pontuação; // quantos pontos ele fez no jogo
};

// globais, com o número de entradas e saídas, e a identificação do
//   próximo neurônio (inovação)
static int num_entradas = -1, num_saídas = -1;
static int próxima_id_neurônio = -1;

extern void Indivíduo_inicializa(int n_entradas, int n_saídas)
{
  num_entradas = n_entradas;
  num_saídas = n_saídas;
  próxima_id_neurônio = num_entradas + num_saídas;
}

// cria um indivíduo vazio
// para poder usá-lo, deve-se inserir neurônios
static Indivíduo indivíduo_cria_vazio()
{
  assert(num_entradas != -1);
  Indivíduo self = calloc(1, sizeof(*self));
  if (self == NULL) return self;
  self->rede = rede_cria_vazia(num_entradas, num_saídas);
  if (self->rede == NULL) {
    free(self);
    return NULL;
  }
  return self;
}

// cria um indivíduo só com neurônios para entradas e saídas, sem sinapses
extern Indivíduo indivíduo_cria()
{
  Indivíduo self = indivíduo_cria_vazio();
  if (self == NULL) {
    return NULL;
  }
  self->geração = 0;
  for (int pos = 0; pos < num_entradas + num_saídas; pos++) {
    neurônio neurônio = {.id = pos, .valor = 0};
    rede_insere_neurônio(self->rede, neurônio);
  }
  rede_calcula_ordem(self->rede);
  return self;
}

extern void indivíduo_destrói(Indivíduo self)
{
  rede_destrói(self->rede);
  free(self);
}

void indivíduo_altera_espécie(Indivíduo self, int espécie)
{
  self->espécie = espécie;
}

int indivíduo_espécie(Indivíduo self)
{
  return self->espécie;
}

extern Rede indivíduo_rede(Indivíduo self)
{
  return self->rede;
}

extern float indivíduo_pontuação(Indivíduo self)
{
  return self->pontuação;
}

extern void indivíduo_altera_pontuação(Indivíduo self, float pontos)
{
  self->pontuação = pontos;
}

// grava uma indivíduo em um arquivo, em formato que pode ser lido como Rede
extern void indivíduo_grava(Indivíduo self, char *nome)
{
  Rede rede = self->rede;
  FILE *arq = fopen(nome, "w");
  if (arq == NULL) return;

  int num_neurônios = rede_num_neurônios(rede);
  fprintf(arq, "%d %d %d\n", num_neurônios, rede_num_entradas(rede),
          rede_num_saídas(rede));
  for (int pos = 0; pos < num_neurônios; pos++) {
    neurônio *neuro = rede_neurônio(rede, pos);
    fprintf(arq, "%d\n", neuro->id);
  }
  for (int pos_origem = 0; pos_origem < num_neurônios; pos_origem++) {
    int pos_destino;
    sinapse *sina;
    rede_sinapses_que_partem(rede, pos_origem);
    while ((sina = rede_próxima_sinapse(rede, &pos_destino)) != NULL) {
      fprintf(arq, "%d %d %f %d\n", pos_origem, pos_destino, sina->peso,
              sina->habilitada ? 1 : 0);
    }
  }
  fprintf(arq, "-1 -1 -1 -1\n");
  fprintf(arq, "%d\n", self->geração);
  fprintf(arq, "%f\n", self->pontuação);
  fclose(arq);
}

// retorna a posição de um neurônio qualquer do indivíduo,
//   que não seja neurônio de saída
static int pos_neurônio_não_saída(Indivíduo self)
{
  int num_saídas = rede_num_saídas(self->rede);
  int num_entradas = rede_num_entradas(self->rede);
  int num_neurônios = rede_num_neurônios(self->rede);

  int pos = aleatório(num_neurônios - num_saídas);
  if (pos >= num_entradas) pos += num_saídas;

  return pos;
}

// retorna a posição de um neurônio qualquer do indivíduo,
//   que não seja neurônio de entrada
static int pos_neurônio_não_entrada(Indivíduo self)
{
  int num_entradas = rede_num_entradas(self->rede);
  int num_neurônios = rede_num_neurônios(self->rede);

  return aleatório_entre(num_entradas, num_neurônios);
}

static neurônio *indivíduo_neurônio_com_id(Indivíduo self, int neuro_id)
{
  int num_neurônios = rede_num_neurônios(self->rede);
  for (int pos = 0; pos < num_neurônios; pos++) {
    neurônio *neuro = rede_neurônio(self->rede, pos);
    if (neuro->id == neuro_id) {
      return neuro;
    }
  }
  return NULL;
}

static sinapse *indivíduo_sinapse_com_ids(Indivíduo self, int id_neuro_origem,
                                          int id_neuro_destino)
{
  int pos_origem = -1, pos_destino = -1;
  int num_neurônios = rede_num_neurônios(self->rede);
  for (int pos = 0; pos < num_neurônios; pos++) {
    neurônio *neuro = rede_neurônio(self->rede, pos);
    if (neuro->id == id_neuro_origem) {
      pos_origem = pos;
      if (pos_destino != -1) break;
    } else if (neuro->id == id_neuro_destino) {
      pos_destino = pos;
      if (pos_origem != -1) break;
    }
  }
  if (pos_origem == -1 || pos_destino == -1) return NULL;
  return rede_sinapse(self->rede, pos_origem, pos_destino);
}

static void indivíduo_copia_neurônios(Indivíduo origem, Indivíduo destino)
{
  int num_neurônios = rede_num_neurônios(origem->rede);
  for (int pos = 0; pos < num_neurônios; pos++) {
    neurônio *neuro = rede_neurônio(origem->rede, pos);
    rede_insere_neurônio(destino->rede, *neuro);
  }
}

// compara as redes neuronais de dois indivíduos e diz se são parecidas o
//   suficiente para serem considerados da mesma espécie
// verifica as sinapses de a, procura em b sinapses com mesmos neurônios
//   de origem e destino.
// conta quantas não encontra e soma a diferença dos pesos das que encontra.
// calcula o grau de compatibilidade a partir desses dois valores.
// quanto menor o grau, mais compatíveis
static float compatibilidade(Indivíduo a, Indivíduo b)
{
  float soma_diffs_pesos = 0;
  int npesos = 0;
  int ndisjuntos = 0;

  int num_neurônios = rede_num_neurônios(a->rede);
  for (int pos_origem = 0; pos_origem < num_neurônios; pos_origem++) {
    neurônio *neuro_origem = rede_neurônio(a->rede, pos_origem);
    int pos_destino;
    sinapse *sinapse_a;
    rede_sinapses_que_partem(a->rede, pos_origem);
    while ((sinapse_a = rede_próxima_sinapse(a->rede, &pos_destino)) != NULL) {
      neurônio *neuro_destino = rede_neurônio(a->rede, pos_destino);
      sinapse *sinapse_b = indivíduo_sinapse_com_ids(b, neuro_origem->id,
                                                     neuro_destino->id);
      if (sinapse_b != NULL) {
        soma_diffs_pesos += fabsf(sinapse_a->peso - sinapse_b->peso);
        npesos++;
      } else {
        ndisjuntos++;
      }
    }
  }
  // deveria fazer de b para a também...
  float compatibilidade = ndisjuntos + 2 * soma_diffs_pesos / npesos;
  return compatibilidade;
}

extern bool indivíduo_mesma_espécie(Indivíduo a, Indivíduo b)
{
  return (compatibilidade(a, b) < 7);
}

// Produção de sinapses novas ou mutadas {{{2

// produz uma nova sinapse misturando outras 2
static sinapse mistura_sinapses(sinapse s1, sinapse s2)
{
  sinapse mistura;

  // escolhe aleatoriamente a mistura entre os pesos dos antepassados
  float alfa = al01();
  mistura.peso = alfa * s1.peso + (1 - alfa) * s2.peso;
  // se algum dos antepassados é inativo, tem 75% de chance de ser inativo
  if ((!s1.habilitada || !s2.habilitada) && probabilidade(75)) {
    mistura.habilitada = false;
  } else {
    mistura.habilitada = true;
  }

  return mistura;
}

// produz sinapses em 'filhote', misturando as sinapses de 'recessivo' que
//   correspondem às de 'dominante'
// se não houver sinapse correspondente no 'recessivo', insere uma igual
//   à de 'dominante'
// sinapses que existem em 'recessivo' sem correspondente em 'dominante'
//   não são copiadas para 'filhote'
static void indivíduo_mistura_sinapses(Indivíduo dominante, Indivíduo recessivo,
                                       Indivíduo filhote)
{
  // 'Rede' não tem uma função que percorra todas as sinapses :o(
  // faz um laço para percorrer cada neurônio e outro para percorrer
  //   as sinapses que partem dele, para obter todas as sinapses do
  //   indivíduo dominante
  int num_neurônios = rede_num_neurônios(dominante->rede);
  for (int pos_origem = 0; pos_origem < num_neurônios; pos_origem++) {
    int pos_destino;
    sinapse *sinadom, *sinarec, sinafilho;
    neurônio *origem, *destino;
    origem = rede_neurônio(dominante->rede, pos_origem);
    rede_sinapses_que_partem(dominante->rede, pos_origem);
    while (true) {
      sinadom = rede_próxima_sinapse(dominante->rede, &pos_destino);
      if (sinadom == NULL) break;
      destino = rede_neurônio(dominante->rede, pos_destino);
      // obtém a sinapse correspondente no 'recessivo', mistura elas e põe
      //   no 'filhote'
      sinarec = indivíduo_sinapse_com_ids(recessivo, origem->id, destino->id);
      if (sinarec != NULL) {
        sinafilho = mistura_sinapses(*sinadom, *sinarec);
      } else {
        sinafilho = *sinadom;
      }
      rede_insere_sinapse(filhote->rede, pos_origem, pos_destino, sinafilho);
    }
  }
}

// cria um indivíduo que é resultado da mistura de dois outros
extern Indivíduo indivíduo_cria_mistura(Indivíduo ind1, Indivíduo ind2)
{
  // para simplificar a mistura, o indivíduo de maior pontuação é dominante
  // os neurônios e sinapses do recessivo que não existam no dominante não
  //   são herdados
  Indivíduo dominante = ind1, recessivo = ind2;
  if (ind1->pontuação < ind2->pontuação) {
    dominante = ind2;
    recessivo = ind1;
  }

  Indivíduo filhote = indivíduo_cria_vazio();
  assert(filhote != NULL);
  // se ind1 e ind2 forem o mesmo indivíduo, o filhote será só uma cópia;
  // senão, se considera que o filhote é de uma nova geração
  if (dominante == recessivo) {
    filhote->geração = dominante->geração;
  } else {
    filhote->geração = max(dominante->geração, recessivo->geração) + 1;
  }

  indivíduo_copia_neurônios(dominante, filhote);
  indivíduo_mistura_sinapses(dominante, recessivo, filhote);
  // a ordem deve ser a mesma do dominante, poderia copiar
  rede_calcula_ordem(filhote->rede);

  return filhote;
}

// Mutações {{{1

// inovações

// para permite comparar redes que evoluiram independentemente, os neurônios
//   recebem uma identificação única global.
// esta função serve para numerar globalmente os neurônios (um neurônio é
//   criado no meio de dois outros, as id desses dois outros definem a id
//   do novo).
// um neurônio novo é chamado de "inovação"
// No NEAT original, isso é feito para as sinapses e não para os neurônios,
//   mas é meio caro fazer dessa forma sem complicar o grafo (ou usar outra
//   estrutura)
#define MAX_INOVAÇÕES 10000
struct inov {
  int id_origem;
  int id_destino;
  int id_meio;
} inovações[MAX_INOVAÇÕES];
int n_inovações = 0;

static int id_neurônio_entre(int id_origem, int id_destino)
{
  // procura a identificação de um neurônio com a mesma origem e destino
  // faz uma busca linear
  for (int i = 0; i < n_inovações; i++) {
    if (inovações[i].id_origem == id_origem &&
        inovações[i].id_destino == id_destino) {
      return inovações[i].id_meio;
    }
  }
  // se não achou, cria uma nova id
  if (n_inovações < MAX_INOVAÇÕES) {
    struct inov inovação = {id_origem, id_destino, próxima_id_neurônio};
    inovações[n_inovações++] = inovação;
  } else {
    fprintf(stderr, "Número máximo de inovações excedido\n");
  }
  return próxima_id_neurônio++;
}

// limita o valor do peso de uma sinapse a valores razoáveis
static float limita_peso(float valor)
{
  if (valor < -20) return -20;
  if (valor > +20) return +20;
  return valor;
}

// retorna um valor aleatório para ser usado como peso de uma sinapse
static float peso_aleatório(bool bagual)
{
  if (bagual) {
    return limita_peso(gauss(0, DP_PESO_BAGUAL));
  } else {
    return limita_peso(gauss(0, DP_PESO));
  }
}

// altera um peso somando um valor aleatório
static float altera_peso(float valor, bool bagual)
{
  if (bagual) {
    return limita_peso(valor + gauss(0, DP_VAR_PESO_BAGUAL));
  } else {
    return limita_peso(valor + gauss(0, DP_VAR_PESO));
  }
}

// cria uma nova sinapse entre 2 neurônios, sem criar ciclo na rede
// retorna true se conseguiu criar a sinapse
static bool mutação_cria_sinapse(Indivíduo self)
{
  int pos_origem = pos_neurônio_não_saída(self);
  int pos_destino = pos_neurônio_não_entrada(self);

  // vê se já existe sinapse entre esses neurônios
  sinapse *sina = rede_sinapse(self->rede, pos_origem, pos_destino);
  if (sina != NULL) {
    // já existe a sinapse -- ativa se estiver inativa
    if (!sina->habilitada) {
      sina->habilitada = true;
    }
    return false;
  }

  // sinapse não existe - cria (mas não vai ser inserida se formar ciclo)
  sinapse nova = {.peso = peso_aleatório(false), .habilitada = true};
  return rede_insere_sinapse(self->rede, pos_origem, pos_destino, nova);
}

/* remoção de neurônio e sinapse não estão sendo usadas

// retorna a posição de um neurônio qualquer do indivíduo,
//   que seja neurônio intermediário
// retorna -1 se não tem neurônio intermediário
static int pos_neurônio_intermediário(Indivíduo self)
{
  int num_saídas    = rede_num_saídas(self->rede);
  int num_entradas  = rede_num_entradas(self->rede);
  int num_neurônios = rede_num_neurônios(self->rede);

  int primeiro_nó_intermediário = num_entradas + num_saídas;
  if (num_neurônios <= primeiro_nó_intermediário) return -1;
  return aleatório_entre(primeiro_nó_intermediário, num_neurônios);
}

// remove um neurônio intermediário aleatório
static void mutação_remove_neurônio(Indivíduo self)
{
  int pos = pos_neurônio_intermediário(self);
  if (pos != -1) rede_remove_neurônio(self->rede, pos);
}

// remove uma sinapse qualquer
static void mutação_remove_sinapse(Indivíduo self)
{
  // tenta achar uma aresta aleatoriamente e remove
  // se não achar uma depois de algumas tentativas, desiste
  for (int tentativa = 0; tentativa < 50; tentativa++) {
    int pos_origem  = pos_neurônio_não_saída(self);
    int pos_destino = pos_neurônio_não_entrada(self);
    if (rede_sinapse(self->rede, pos_origem, pos_destino) != NULL) {
      rede_remove_sinapse(self->rede, pos_origem, pos_destino);
      return;
    }
  }
}
*/

// insere um novo neurônio no meio de uma sinapse
// a sinapse antiga continua existindo, mas é desativada
static bool mutação_divide_sinapse(Indivíduo self)
{
  // tenta aleatoriamente achar uma aresta para dividir
  // se não achar uma depois de algumas tentativas, desiste
  for (int tentativa = 0; tentativa < 50; tentativa++) {
    int pos_origem = pos_neurônio_não_saída(self);
    int pos_destino = pos_neurônio_não_entrada(self);

    sinapse *sina = rede_sinapse(self->rede, pos_origem, pos_destino);
    if (sina == NULL) continue;

    neurônio *neuro_origem, *neuro_destino;
    neuro_origem = rede_neurônio(self->rede, pos_origem);
    neuro_destino = rede_neurônio(self->rede, pos_destino);
    int id_do_meio = id_neurônio_entre(neuro_origem->id, neuro_destino->id);
    if (indivíduo_neurônio_com_id(self, id_do_meio) != NULL) {
      // já tem essa divisão, tenta outra
      continue;
    }
    // desativa a sinapse original
    if (sina->habilitada) {
      sina->habilitada = false;
    }
    // insere uma sinapse, um neurônio e outra sinapse interligando
    //   os 2 neurônios
    neurônio neuro_meio = {.id = id_do_meio, .valor = 0};
    sinapse sina1 = {.peso = 1, .habilitada = true};
    sinapse sina2 = {.peso = sina->peso, .habilitada = true};
    int pos_meio = rede_insere_neurônio(self->rede, neuro_meio);
    rede_insere_sinapse(self->rede, pos_origem, pos_meio, sina1);
    rede_insere_sinapse(self->rede, pos_meio, pos_destino, sina2);
    return true;
  }
  return false;
}

// altera os pesos das sinapses da rede
static void mutação_altera_valor_sinapses(Indivíduo self)
{
  bool bagual = probabilidade(PROB_MUTAR_PESOS_BAGUALMENTE);
  int prob_relativo = PROB_VAL_RELATIVO;
  int prob_absoluto = PROB_VAL_ABSOLUTO;
  if (bagual) {
    prob_relativo = PROB_VAL_RELATIVO_BAGUAL;
    prob_absoluto = PROB_VAL_ABSOLUTO_BAGUAL;
  }
  int nneurônios = rede_num_neurônios(self->rede);
  for (int pos_origem = 0; pos_origem < nneurônios; pos_origem++) {
    int pos_destino;
    sinapse *sina;
    rede_sinapses_que_partem(self->rede, pos_origem);
    while ((sina = rede_próxima_sinapse(self->rede, &pos_destino)) != NULL) {
      int aleat = aleatório(100);
      if (aleat >= prob_relativo + prob_absoluto) continue;
      if (aleat < prob_relativo) {
        sina->peso = altera_peso(sina->peso, bagual);
      } else {
        sina->peso = peso_aleatório(bagual);
      }
    }
  }
}

// desativa alguma sinapse ativa ou ativa uma inativa
static void mutação_inverte_habilitação_sinapse(Indivíduo self)
{
  for (int tentativa = 0; tentativa < 50; tentativa++) {
    int pos_origem = pos_neurônio_não_saída(self);
    int pos_destino = pos_neurônio_não_entrada(self);

    sinapse *sina = rede_sinapse(self->rede, pos_origem, pos_destino);
    if (sina == NULL) continue;

    if (sina->habilitada) {
      // não desativa se for a única sinapse ativa saindo do neurônio
      bool achou_vizinho = false;
      int pos_vizinho;
      rede_sinapses_que_partem(self->rede, pos_origem);
      while (true) {
        sinapse *sina_vizinho = rede_próxima_sinapse(self->rede, &pos_vizinho);
        if (sina_vizinho != NULL) break;
        if (pos_vizinho != pos_destino && sina_vizinho->habilitada) {
          achou_vizinho = true;
          break;
        }
      }
      if (!achou_vizinho) continue;
    }
    sina->habilitada = !sina->habilitada;
    break;
  }
}

// faz mutações variadas em um indivíduo
extern void indivíduo_muta(Indivíduo self)
{
  // esse encadeamento de probabilidades é meio estranho...
  bool mutação_estrutural = false;
  if (probabilidade(PROB_CRIAR_NEURÔNIO)) {
    mutação_estrutural = mutação_divide_sinapse(self);
  } else if (probabilidade(PROB_CRIAR_SINAPSE)) {
    mutação_estrutural = mutação_cria_sinapse(self);
  } else {
    if (probabilidade(PROB_MUTAR_PESOS)) {
      mutação_altera_valor_sinapses(self);
    }
    if (probabilidade(PROB_INV_HABILITAÇÃO)) {
      mutação_inverte_habilitação_sinapse(self);
    }
  }
  // se mudou a estrutura (criou ou destruiu sinapse ou neurônio),
  //   tem que rever a ordem de cálculo da rede
  if (mutação_estrutural) {
    rede_calcula_ordem(self->rede);
  }
}

// compara dois indivíduos para ordenação por pontuação, com qsort
// pontuação maior fica antes (se p1 tem mais pontos que p2, retorna neg)
extern int indivíduo_compara_por_pontuação(const void *p1, const void *p2)
{
  // p1 e p2 são ponteiros para posições em um vetor de Indivíduo
  Indivíduo *p_ind1 = (Indivíduo *)p1;
  Indivíduo *p_ind2 = (Indivíduo *)p2;
  Indivíduo indivíduo1 = *p_ind1;
  Indivíduo indivíduo2 = *p_ind2;
  return indivíduo_pontuação(indivíduo2) - indivíduo_pontuação(indivíduo1);
}
