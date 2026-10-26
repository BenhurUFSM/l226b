
#include "especie.h"
#include "auxiliar.h"
#include "config.h"

#include <assert.h>
#include <stdlib.h>

// Espécie {{{1

struct espécie {
  int id;                  // identificador da espécie
  int geração_criação;     // em que geração foi criada esta espécie
  int geração_atual;       // em que geração foi totalizado pela última vez
  float pontuação_média;   // média da pontuação dos indivíduos da espécie
  float pontuação_máxima;  // maior pontuação obtida por algum desta espécie
  int geração_últ_melhora; // em que geração foi o último aumento da máxima
  int n_descendentes;      // quantos descendentes irão para a próxima geração
  int n_indivíduos;        // número de indivíduos na espécie
  Indivíduo indivíduos[MAX_IND_ESP];
};

extern Espécie espécie_cria(Indivíduo indivíduo, int id_geração)
{
  // as espécies são identificadas por um "número de série" inteiro
  // esta variável static mantém esse número
  static int prox_id = 0;
  Espécie espécie = malloc(sizeof(*espécie));
  assert(espécie != NULL);
  espécie->id = prox_id++;
  espécie->geração_criação = id_geração;
  espécie->pontuação_máxima = 0;
  espécie->geração_últ_melhora = id_geração;
  espécie->indivíduos[0] = indivíduo;
  espécie->n_indivíduos = 1;
  indivíduo_altera_espécie(indivíduo, espécie->id);
  return espécie;
}

extern Espécie espécie_cria_cópia(Espécie espécie)
{
  // não usa 'espécie_cria' para criar a nova espécie, porque sendo
  //   uma cópia, a nova tem a mesma id
  Espécie espécie_nova = malloc(sizeof(*espécie_nova));
  assert(espécie_nova != NULL);
  Indivíduo campeão = espécie_campeão(espécie);
  assert(campeão != NULL);
  Indivíduo cópia_do_campeão = indivíduo_cria_mistura(campeão, campeão);
  espécie_nova->id = espécie->id;
  espécie_nova->geração_criação = espécie->geração_criação;
  espécie_nova->pontuação_máxima = espécie->pontuação_máxima;
  espécie_nova->geração_últ_melhora = espécie->geração_últ_melhora;
  espécie_nova->indivíduos[0] = cópia_do_campeão;
  espécie_nova->n_indivíduos = 1;
  espécie->n_descendentes--;
  indivíduo_altera_espécie(cópia_do_campeão, espécie->id);
  return espécie_nova;
}

extern void espécie_destrói(Espécie espécie)
{
  // o gerenciamento da memória dos indivíduos é feito fora da espécie
  free(espécie);
}

extern Indivíduo espécie_campeão(Espécie espécie)
{
  return espécie->indivíduos[0];
}

extern bool espécie_acrescenta_indivíduo(Espécie espécie, Indivíduo indivíduo)
{
  if (!indivíduo_mesma_espécie(espécie->indivíduos[0], indivíduo)) return false;
  assert(espécie->n_indivíduos < MAX_IND_ESP);
  espécie->indivíduos[espécie->n_indivíduos++] = indivíduo;
  indivíduo_altera_espécie(indivíduo, espécie->id);
  return true;
}

extern Indivíduo espécie_indivíduo_aleatório(Espécie espécie)
{
  return espécie->indivíduos[aleatório_p(espécie->n_indivíduos)];
}

static void espécie_verifica_pontuação_máxima(Espécie espécie)
{
  // ordena indivíduos por pontuação
  qsort(espécie->indivíduos, espécie->n_indivíduos,
        sizeof(espécie->indivíduos[0]), indivíduo_compara_por_pontuação);
  // vê se a espécie evoluiu
  float pontuação_máxima = indivíduo_pontuação(espécie->indivíduos[0]);
  if (pontuação_máxima > espécie->pontuação_máxima) {
    espécie->pontuação_máxima = pontuação_máxima;
    espécie->geração_últ_melhora = espécie->geração_atual;
  }
}

extern float espécie_totaliza(Espécie espécie, int id_geração)
{
  espécie->geração_atual = id_geração;
  espécie_verifica_pontuação_máxima(espécie);
  // calcula o somatório das pontuações dos indivíduos
  float pontuação_total = 0;
  for (int i = 0; i < espécie->n_indivíduos; i++) {
    Indivíduo indivíduo = espécie->indivíduos[i];
    pontuação_total += indivíduo_pontuação(indivíduo);
  }
  // dá uma turbinada se espécie for nova e detona se tiver estagnada
  bool novinha = (id_geração - espécie->geração_criação) < N_NOVINHA;
  bool estagnada = (id_geração - espécie->geração_últ_melhora) > N_ESTAGNA_ESP;
  if (novinha) pontuação_total *= 2;
  if (estagnada) pontuação_total *= 0.5;
  // calcula e retorna a média
  espécie->pontuação_média = pontuação_total / espécie->n_indivíduos;
  return espécie->pontuação_média;
}

extern bool espécie_estagnada(Espécie espécie)
{
  return espécie->geração_atual - espécie->geração_últ_melhora >= N_ESTAGNA;
}

void espécie_atualiza_última_melhora(Espécie espécie, int última_melhora)
{
  espécie->geração_últ_melhora = última_melhora;
}

extern void espécie_calcula_sobreviventes(Espécie espécie)
{
  if (espécie->n_descendentes == 0) {
    espécie->n_indivíduos = 0;
  } else {
    // oh, mundo cruel!
    int n_sobreviventes = espécie->n_indivíduos * PERC_SOBREVIVENTES / 100;
    if (n_sobreviventes < 1) n_sobreviventes = 1;
    espécie->n_indivíduos = n_sobreviventes;
  }
}

extern void espécie_calcula_descendentes(Espécie espécie,
                                         float pontuação_média_geração)
{
  espécie->n_descendentes = espécie->pontuação_média / pontuação_média_geração;
}

extern void espécie_soma_descendentes(Espécie espécie, int n)
{
  espécie->n_descendentes += n;
}

extern int espécie_num_descendentes(Espécie espécie)
{
  return espécie->n_descendentes;
}

int espécie_compara_por_pontuação_individual(const void *p1, const void *p2)
{
  // p1 e p2 são ponteiros para posições em um vetor de Espécie
  Espécie *p_esp1 = (Espécie *)p1;
  Espécie *p_esp2 = (Espécie *)p2;
  Espécie espécie1 = *p_esp1;
  Espécie espécie2 = *p_esp2;
  // compara a pontuação do indivíduo de maior pontuação da espécie
  Indivíduo indivíduo1 = espécie_campeão(espécie1);
  Indivíduo indivíduo2 = espécie_campeão(espécie2);
  return indivíduo_pontuação(indivíduo2) - indivíduo_pontuação(indivíduo1);
}

int espécie_compara_por_pontuação_média(const void *p1, const void *p2)
{
  // p1 e p2 são ponteiros para posições em um vetor de Espécie
  Espécie *p_esp1 = (Espécie *)p1;
  Espécie *p_esp2 = (Espécie *)p2;
  Espécie espécie1 = *p_esp1;
  Espécie espécie2 = *p_esp2;
  // compara média das duas espécies
  return espécie2->pontuação_média - espécie1->pontuação_média;
}
