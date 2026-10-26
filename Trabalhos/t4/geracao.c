#include "geracao.h"
#include "auxiliar.h"
#include "config.h"
#include "especie.h"
#include "individuo.h"
#include "main.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// para simplificar a implementação, tem um vetor com todos os indivíduos,
//   que também estão nas espécies
// deveria usar uma estrutura mais dinâmica para guardar as espécies e os
//   indivíduos

struct geração {
  int id;
  int n_indivíduos;
  Indivíduo indivíduos[N_INDIVÍDUOS];
  int n_espécies;
  Espécie espécies[MAX_ESPÉCIES];
  float pontuação_média;
};

static Geração geração_cria()
{
  Geração geração = malloc(sizeof(*geração));
  assert(geração != NULL);
  geração->n_espécies = 0;
  geração->n_indivíduos = 0;
  return geração;
}

// funções auxiliares {{{1

// cria uma nova espécie na geração, com um indivíduo
static void geração_cria_espécie(Geração geração, Indivíduo indivíduo);

// insere uma cópia da espécie na geração
// 'espécie' e de outra geração
// se 'reinicializa_última_melhora' for true, faz de conta que a espécie
//   melhorou sua pontuação agora
static void geração_copia_espécie(Geração geração, Espécie espécie,
                                  bool reinicializa_última_melhora);

// copia cada espécie com descendentes para a nova geração,
//   junto com seu campeão
static void geração_copia_espécies(Geração nova, Geração velha);

// (re)classifica todos os indivíduos da geração em espécies.
static void geração_separa_espécies(Geração geração);

// insere o indivíduo na geração e em alguma espécie
static void geração_insere_indivíduo(Geração geração, Indivíduo indivíduo);

// insere o indivíduo na geração, mas não em uma espécie
static void geração_insere_indivíduo_sem_especiar(Geração geração,
                                                  Indivíduo indivíduo);

// coloca o indivíduo em alguma espécie da geração
// cria nova espécie, se nenhuma for compatível
// indivíduo deve estar no vetor de indivíduos mas não em alguma espécie
static void geração_especia_indivíduo(Geração geração, Indivíduo indivíduo);

// cria a nova geração inteira a partir do melhor indivíduo de cada uma
//   das duas melhores espécies
static void geração_reproduz_estagnada(Geração nova, Geração velha);

// para cada espécie da geração, calcula o número de indivíduos sobreviventes,
//   que participarão da construção da próxima geração
static void geração_calcula_sobreviventes(Geração geração);

// retorna o melhor indivíduo de alguma espécie da geração
// dá preferência para espécies com maior pontuação
static Indivíduo geração_campeão_aleatório(Geração geração);

// funções auxiliares

static void geração_cria_espécie(Geração geração, Indivíduo indivíduo)
{
  assert(geração->n_espécies < MAX_ESPÉCIES);
  Espécie espécie_nova = espécie_cria(indivíduo, geração->id);
  geração->espécies[geração->n_espécies++] = espécie_nova;
}

static void geração_copia_espécie(Geração geração, Espécie espécie,
                                  bool reinicializa_última_melhora)
{
  assert(geração->n_espécies < MAX_ESPÉCIES);
  Espécie espécie_nova = espécie_cria_cópia(espécie);
  geração->espécies[geração->n_espécies++] = espécie_nova;
  Indivíduo campeão_novo = espécie_campeão(espécie_nova);
  geração_insere_indivíduo_sem_especiar(geração, campeão_novo);
  if (reinicializa_última_melhora) {
    espécie_atualiza_última_melhora(espécie_nova, geração->id);
  }
}

static void geração_copia_espécies(Geração nova, Geração velha)
{
  for (int e = 0; e < velha->n_espécies; e++) {
    Espécie espécie_velha = velha->espécies[e];
    if (espécie_num_descendentes(espécie_velha) < 1) continue;
    geração_copia_espécie(nova, espécie_velha, false);
  }
}

static void geração_separa_espécies(Geração geração)
{
  // um indivíduo pertence a uma espécie se for considerado da mesma espécie
  //   que o primeiro dessa espécie
  // especia os indivíduos por ordem decrescente de pontuação, para que o
  //   primeiro de cada espécie seja o de maior pontuação
  qsort(geração->indivíduos, N_INDIVÍDUOS, sizeof(geração->indivíduos[0]),
        indivíduo_compara_por_pontuação);
  geração->n_espécies = 0;
  for (int i = 0; i < N_INDIVÍDUOS; i++) {
    Indivíduo indivíduo = geração->indivíduos[i];
    geração_especia_indivíduo(geração, indivíduo);
  }
}

static void geração_insere_indivíduo(Geração geração, Indivíduo indivíduo)
{
  geração_insere_indivíduo_sem_especiar(geração, indivíduo);
  geração_especia_indivíduo(geração, indivíduo);
}

static void geração_insere_indivíduo_sem_especiar(Geração geração,
                                                  Indivíduo indivíduo)
{
  assert(geração->n_indivíduos < N_INDIVÍDUOS);
  geração->indivíduos[geração->n_indivíduos++] = indivíduo;
}

static void geração_especia_indivíduo(Geração geração, Indivíduo indivíduo)
{
  Espécie espécie;
  // tenta inserir o indivíduo em uma espécie existente
  for (int e = 0; e < geração->n_espécies; e++) {
    espécie = geração->espécies[e];
    if (espécie_acrescenta_indivíduo(espécie, indivíduo)) return;
  }
  // o indivíduo não é compatível com ninguém, cria uma nova espécie
  geração_cria_espécie(geração, indivíduo);
}

static void geração_reproduz_estagnada(Geração nova, Geração velha)
{
  printf("Estagnou\n");
  Espécie espécie1 = velha->espécies[0];
  geração_copia_espécie(nova, espécie1, true);
  Indivíduo campeão1 = espécie_campeão(espécie1);
  Indivíduo campeão2 = campeão1;
  if (velha->n_espécies > 1) {
    Espécie espécie2 = velha->espécies[1];
    if (espécie_num_descendentes(espécie2) > 0) {
      geração_copia_espécie(nova, espécie2, true);
      campeão2 = espécie_campeão(espécie2);
    }
  }
  while (nova->n_indivíduos < N_INDIVÍDUOS) {
    Indivíduo filhote;
    int prob = aleatório(100);
    if (prob < 5) {
      filhote = indivíduo_cria_mistura(campeão2, campeão2);
    } else if (prob < 15) {
      filhote = indivíduo_cria_mistura(campeão1, campeão2);
    } else {
      filhote = indivíduo_cria_mistura(campeão1, campeão1);
    }
    for (int i = aleatório(MAX_MUT_ESTAGNADA) + 1; i > 0; i--) {
      indivíduo_muta(filhote);
    }
    geração_insere_indivíduo(nova, filhote);
  }
}

static void geração_calcula_sobreviventes(Geração geração)
{
  for (int e = 0; e < geração->n_espécies; e++) {
    Espécie espécie = geração->espécies[e];
    espécie_calcula_sobreviventes(espécie);
  }
}

static Indivíduo geração_campeão_aleatório(Geração geração)
{
  Indivíduo campeão;
  for (int tentativas = 0; tentativas < 100; tentativas++) {
    int id = aleatório_p(geração->n_espécies);
    Espécie espécie_aleatória = geração->espécies[id];
    campeão = espécie_campeão(espécie_aleatória);
    if (campeão != NULL) break;
  }
  if (campeão == NULL) campeão = espécie_campeão(geração->espécies[0]);
  return campeão;
}

// funções da interface do TAD {{{1

extern Geração geração_cria_aleatório()
{
  Geração geração = geração_cria();
  geração->id = 0;
  for (int i = 0; i < N_INDIVÍDUOS; i++) {
    Indivíduo indivíduo = indivíduo_cria();
    indivíduo_muta(indivíduo);
    geração->indivíduos[i] = indivíduo;
  }
  geração->n_indivíduos = N_INDIVÍDUOS;
  geração_separa_espécies(geração);
  return geração;
}

extern void geração_destrói(Geração geração)
{
  for (int i = 0; i < geração->n_espécies; i++) {
    Espécie espécie = geração->espécies[i];
    espécie_destrói(espécie);
  }
  for (int i = 0; i < geração->n_indivíduos; i++) {
    Indivíduo indivíduo = geração->indivíduos[i];
    indivíduo_destrói(indivíduo);
  }
  free(geração);
}

extern void geração_joga(Geração geração, Jogo jogo)
{
  for (int i = 0; i < N_INDIVÍDUOS; i++) {
    Indivíduo jogador = geração->indivíduos[i];
    srand(0); // para ter partidas iguais para todos
    float soma = 0;
    for (int ip = 0; ip < N_PARTIDAS; ip++) {
      jogo_inicia(jogo, MAX_PASSOS);
      soma += joga_sem_tela(jogo, indivíduo_rede(jogador));
    }
    indivíduo_altera_pontuação(jogador, soma / N_PARTIDAS);
  }
  qsort(geração->indivíduos, N_INDIVÍDUOS, sizeof(geração->indivíduos[0]),
        indivíduo_compara_por_pontuação);
}

extern void geração_calcula_descendentes(Geração geração)
{
  float pontuação_total = 0;
  for (int e = 0; e < geração->n_espécies; e++) {
    Espécie espécie = geração->espécies[e];
    pontuação_total += espécie_totaliza(espécie, geração->id);
  }
  if (pontuação_total <= 1) pontuação_total = 1;
  geração->pontuação_média = pontuação_total / N_INDIVÍDUOS;
  // ordena as espécies pela pontuação alterada, escolhe o campeão
  qsort(geração->espécies, geração->n_espécies, sizeof(geração->espécies[0]),
        espécie_compara_por_pontuação_média);
  // calcula o número de descendentes para cada espécie, e o total
  int soma_descendentes = 0;
  for (int e = 0; e < geração->n_espécies; e++) {
    Espécie espécie = geração->espécies[e];
    espécie_calcula_descendentes(espécie, geração->pontuação_média);
    soma_descendentes += espécie_num_descendentes(espécie);
  }
  // entrega as sobras para a espécie campeã
  //   (pode ter erro de arredondamento nos cálculos das espécies)
  espécie_soma_descendentes(geração->espécies[0],
                            N_INDIVÍDUOS - soma_descendentes);
}

// insere novos indivíduos na geração nova, descendentes de cada espécie da
//   geração velha
// cada novo indivíduo é o resultado da mistura de dois velhos, que
//   potencialmente sofre mutações
static void geração_gera_descendentes(Geração nova, Geração velha)
{
  for (int e = 0; e < velha->n_espécies; e++) {
    Espécie espécie = velha->espécies[e];
    int n_descendentes = espécie_num_descendentes(espécie);
    for (int i = 0; i < n_descendentes; i++) {
      Indivíduo mamãe, papai, filhote;
      mamãe = espécie_indivíduo_aleatório(espécie);
      if (probabilidade(PROB_MESMA_ESPÉCIE)) {
        papai = espécie_indivíduo_aleatório(espécie);
      } else {
        papai = geração_campeão_aleatório(velha);
      }
      filhote = indivíduo_cria_mistura(mamãe, papai);
      if (mamãe == papai || probabilidade(PROB_MUTAÇÃO)) {
        indivíduo_muta(filhote);
      }
      geração_insere_indivíduo(nova, filhote);
    }
  }
}

// cria os indivíduos de uma nova geração à partir da geração precedente
extern Geração geração_reproduz(Geração velha)
{
  Geração nova = geração_cria();
  // ordena as espécies por pontuação
  qsort(velha->espécies, velha->n_espécies, sizeof(velha->espécies[0]),
        espécie_compara_por_pontuação_individual);
  nova->id = velha->id + 1;
  // se a espécie com melhor pontuação estagnou, toma ações mais drásticas
  if (espécie_estagnada(velha->espécies[0])) {
    geração_reproduz_estagnada(nova, velha);
  } else {
    // mata os piores indivíduos de cada espécie
    geração_calcula_sobreviventes(velha);
    // copia as espécies com indivíduos remanescentes
    geração_copia_espécies(nova, velha);
    // gera o número de descendentes de cada espécie
    geração_gera_descendentes(nova, velha);
  }

  return nova;
}

extern void geração_grava(Geração geração)
{
  char nome[30];
  system("mkdir G 2> /dev/null"); // cria o diretório "G" se não existir
  for (int i = 0; i < 1 /*N_INDIVÍDUOS*/; i++) {
    Indivíduo indivíduo = geração->indivíduos[i];
    sprintf(nome, "G/%04d-%03d", geração->id, i);
    indivíduo_grava(indivíduo, nome);
  }
}

extern void geração_imprime(Geração geração)
{
  printf("geração=%d melhor_pontuação=%.0f espécie=%d nespécies=%d\n",
         geração->id, indivíduo_pontuação(geração->indivíduos[0]),
         indivíduo_espécie(geração->indivíduos[0]), geração->n_espécies);
}
