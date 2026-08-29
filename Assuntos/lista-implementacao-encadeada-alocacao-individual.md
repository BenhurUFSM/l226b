### Listas encadeadas com alocação individual

Quando a lista é encadeada, não necessitamos movimentar fisicamente na memória os dados que já estão na lista, mesmo na implementação de operações que alterem a posição desses dados na lista.
Com a alocação contígua dos dados, entretanto, ainda resta o problema de se manter alocada memória nos vetores além das necessidades da lista.
É muito caro realocar os vetores para terem o tamanho exato necessário cada vez que o número de elementos na lista é alterado. Geralmente se aloca os vetores com folga para evitar excesso de operações de realocação e cópia de dados.

Uma outra possibilidade de alocação de memória é alocar cada dado individualmente, e só manter alocados os que estão efetivamente em uso pela lista. Quando um dado é removido, a memória ocupada por ele é liberada. O gerenciamento de memória fica com o sistema de alocação de memória e não mais com a lista. Nesse caso, um dado pode estar em qualquer posição da memória, e sua posição é mantida por um ponteiro e não por um índice.
Para manter a correspondência entre um dado na lista e o próximo (o encadeamento), não temos mais o suporte do vetor.
Essas duas informações são mantidas em um registro (um nó da lista), e essa é a unidade de alocação.

Nossa lista passa a ter então dois tipos de registros: uma estrutura que mantém informações gerais da lista (chamada de descritor da lista) e uma estrutura para cada dado (um nó).

Nessa implementação, não vamos ter memória desperdiçada com espaço não usado pela lista, mas em compensação tem memória extra gasta pelo sistema de gerenciamento de memória, que vai gerenciar um número maior de alocações.

Uma implementação parcial de uma lista encadeada com alocação individual dos nós está em [lista_ep.c](../Complementos/lista_ep.c).
O que antes era implementado como inteiros que representavam índices do vetor agora é implementado como ponteiro para a estrutura *nó*.
O que antes era representado como índice -1, agora é um ponteiro NULL.
Não existe mais a lista de livres, os nós são alocados e liberados com funções de alocação de memória.
Compare com a implementação anterior de [lista_evr.c](../Complementos/lista_evr.c).

#### Exercícios

1. Implementa as funções de remoção da lista acima
2. Teste a lista
3. Implementa uma função para inverter a ordem dos nós de uma lista encadeada.

