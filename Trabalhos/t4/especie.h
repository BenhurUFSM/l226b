#ifndef _ESPECIE_H_
#define _ESPECIE_H_

// uma espécie contém indivíduos com redes neurais semelhantes
// o primeiro indivíduo define a espécie, os demais são considerados
//   semelhantes a ele
// os indivíduos são ordenados pela pontuação que fizeram no jogo
// o primeiro indivíduo é quem consegue mais pontos na espécie, por isso
//   é chamado de "campeão"
// quanto mais pontos os indivíduos da espécie conseguem, e quanto
//   mais essa pontuação evolui, maiores as chances de gerar mais
//   descendentes para a geração seguinte

#include "individuo.h"

typedef struct espécie *Espécie;

// cria uma nova espécie
// 'indivíduo' é o Indivíduo que define a espécie
// 'id_geração' é a geração à que a nova espécie pertence
Espécie espécie_cria(Indivíduo indivíduo, int id_geração);

// retorna uma nova espécie que é uma cópia de 'espécie'
// a nova espécie passa a conter um único indivíduo, uma cópia do indivíduo
//   campeão da espécie velha
Espécie espécie_cria_cópia(Espécie espécie);

// libera a memória ocupada pela espécie
// não destrói os indivíduos, o gerenciamento de memória deles é feito
//   fora da espécie
void espécie_destrói(Espécie espécie);

// retorna o indivíduo definidor da espécie
Indivíduo espécie_campeão(Espécie espécie);

// acrescenta o indivíduo na espécie, se for compatível com o definidor da
//   espécie
// retorna true se ok
bool espécie_acrescenta_indivíduo(Espécie espécie, Indivíduo indivíduo);

// retorna um indivíduo qualquer da espécie
// maior probabilidade de retornar indivíduos de maior pontuação
Indivíduo espécie_indivíduo_aleatório(Espécie espécie);

// ordena os indivíduos por pontuação, seleciona o campeão, e calcula a
//   pontuação média da espécie
// a média é aumentada ou diminuída, para dar mais chance a espécies novas
//   e menos chances a espécies estagnadas
// retorna a pontuação média da espécie
float espécie_totaliza(Espécie espécie, int id_geração);

// retorna true se faz muito tempo que a espécie não melhora a pontuação
bool espécie_estagnada(Espécie espécie);

// altera a identificação da geração em que houve a última alteração
//   na melhor pontuação da espécie
void espécie_atualiza_última_melhora(Espécie espécie, int última_melhora);

// calcula o número de sobreviventes da espécie
// esses indivíduos poderão ser usados na criação de descendentes desta espécie
void espécie_calcula_sobreviventes(Espécie espécie);

// calcula o número de descendentes que se espera que a espécie tenha
// é proporcional à fatia da pontuação da geração que foi obtida pela espécie
void espécie_calcula_descendentes(Espécie espécie, float pontuação_média_geração);

// soma mais alguns descendentes na espécie, para corrigir arredondamentos e
//   garantir o tamanho da próxima geração
void espécie_soma_descendentes(Espécie espécie, int n);

// retorna o número de descendentes da espécie
int espécie_num_descendentes(Espécie espécie);

// compara duas espécies para ordenação para uso com qsort
// a comparação é feita pela pontuação do campeão de cada espécie
// pontuação maior fica antes (se p1 tem mais pontos que p2, retorna neg)
int espécie_compara_por_pontuação_individual(const void *p1, const void *p2);

// compara duas espécies para ordenação para uso com qsort
// a comparação é feita pela média de pontos (alterados) da espécie
// pontuação maior fica antes (se p1 tem mais pontos que p2, retorna neg)
int espécie_compara_por_pontuação_média(const void *p1, const void *p2);

#endif // _ESPECIE_H_
