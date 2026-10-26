#ifndef _CONFIG_H_
#define _CONFIG_H_

// Constantes para configurar o treinamento das redes neuronais

// número de gerações de redes treinadas
#define N_GERAÇÕES 8000
// número de indivíduos em uma geração
#define N_INDIVÍDUOS 500

// máximo de espécies em uma geração
#define MAX_ESPÉCIES N_INDIVÍDUOS
// máximo de indivíduos em uma espécie
#define MAX_IND_ESP N_INDIVÍDUOS
// percentual de sobreviventes de uma espécie, para a geração seguinte
#define PERC_SOBREVIVENTES 30
// geração estagna com N gerações sem melhora
#define N_ESTAGNA 30
// espécie estagna com N gerações sem melhora
#define N_ESTAGNA_ESP 25
// espécie é jovem se viva há < N gerações
#define N_NOVINHA 9

// número máximo de passos em uma partida
#define MAX_PASSOS 3000
// número de partidas que cada indivíduo joga
#define N_PARTIDAS 3

// probabilidade (%) do novo indivíduo ser cria de dois da mesma espécie
#define PROB_MESMA_ESPÉCIE 80
// probabilidade (%) do novo indivíduo sofrer alguma mutação
#define PROB_MUTAÇÃO 80

// máximo de mutações por indivíduo estagnado
#define MAX_MUT_ESTAGNADA 80

// probabilidades na mutação
#define PROB_CRIAR_NEURÔNIO 10
#define PROB_CRIAR_SINAPSE 10
#define PROB_MUTAR_PESOS 30
#define PROB_MUTAR_PESOS_BAGUALMENTE 10
#define PROB_INV_HABILITAÇÃO 2

// probabilidade de alteração do valor de uma sinapse,
//   com um valor relativo ao valor atual ou de forma absoluta
// somando os dois, o que sobra para 100 é a probabilidade de não alterar
#define PROB_VAL_RELATIVO 50
#define PROB_VAL_ABSOLUTO 5
#define PROB_VAL_RELATIVO_BAGUAL 20
#define PROB_VAL_ABSOLUTO_BAGUAL 75

// desvio padrão para valores aleatórios para o peso de uma sinapse
// de um valor absoluto
#define DP_PESO 2
// de uma alteração relativa
#define DP_VAR_PESO 0.5
// de um valor absoluto
#define DP_PESO_BAGUAL 6
// de uma alteração relativa
#define DP_VAR_PESO_BAGUAL 2

#endif // _CONFIG_H_
