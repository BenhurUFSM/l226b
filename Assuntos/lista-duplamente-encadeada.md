
### Listas duplamente encadeadas

Por vezes (em uma fila do tipo deque, por exemplo), necessitamos de operações de inclusão e remoção em ambas as pontas da lista, e a lista simplesmente encadeada não tem um desempenho bom o suficiente para todas essas operações. Não é muito difícil de se otimizar para se ter um bom desempenho em três dessas operações, mas não nas quatro.
A adição de um segundo encadeamento na lista resolve essa limitação, ao custo de mais memória para armazenar mais um ponteiro em cada nó da lista.
Cada nó passa a ter dois ponteiros: um que aponta para o próximo elemento da lista (como em uma lista simplesmente encadeada) e um outro que aponta para o elemento anterior.
Em todas as operações que alteram a lista (inclusão e remoção de elementos), temos que manter atualizados esses dois ponteiros, o que torma a implementação um pouco mais complexa.
No descritor da lista, se coloca um ponteiro para o último elemento da lista, além do primeiro, para permitir que a inclusão e remoção no final da lista seja tão fácil quanto no início. Outra alternativa seria fazer a lista circular (em que o último é o anterior do primeiro) ou mesmo com um nó sentinela (uma lista circular com um nó permanente, sem dado útil, que fica entre o último e o primeiro).

#### Implementação

Suponha que o descritor da lista e o nó sejam declarados como abaixo:
```c
   typedef struct lista *Lista;
   typedef struct nó nó;
   struct lista {
     int num;    // número de elementos na lista
     nó *prim;   // nó que contém o primeiro elemento da lista
     nó *ult;    // nó que contém o último elemento da lista
   };
   struct nó {
     dado_t info; // a informação que é mantida por este nó
     nó *prox;    // ponteiro para o nó seguinte
     nó *ant;     // ponteiro para o nó anterior
   };
```

A operação de remoção de um nó qualquer da lista seria simples, tendo um ponteiro para o nó a remover:
```c
   ///
   nó *vitima;
   // ... (faz 'vitima' apontar para o nó a remover)
   // cria variáveis auxiliares para apontar para os nós envolvidos
   nó *seguinte = vitima->prox;  // o nó que está depois do nó a ser removido
   nó *anterior = vitima->ant;   // o nó que está antes do nó a ser removido
   // remove o nó dos encadeamentos
   anterior->prox = seguinte;    // o nó que segue o nó anterior ao removido é o que está depois do removido
   seguinte->ant = anterior;     // o nó que antecede o nó que está após o removido é o que está antes do removido
   // libera a memória do nó removido
   free(vitima);
   l->num--;
```
Abaixo está uma "animação" desse código, com o estado das variáveis após a execução de cada linha.
```
   // estado inicial, a lista tam 3 elementos, A, B, C
   prim pA (o campo prim do descritor da lista tem um ponteiro para o nó que tem A)
   ult  pC
   num  3  (o campo num da lista tem 3)

        |A|--|pB|   (um nó que tem o dado A, o ponteiro ant é NULL, prox é pB)
        |B|pA|pC|
        |C|pB|--|
```
```
   nó *vitima; // ... (faz 'vitima' apontar para o nó a remover)

   prim pA
   ult  pC
   num  3

   vitima pB  (o nó que vai ser removido é o que contém o B)

        |A|--|pB|
        |B|pA|pC|
        |C|pB|--|
```
```
   nó *seguinte = vitima->prox;

   prim pA
   ult  pC
   num  3

   vitima pB
   seguinte pC

        |A|--|pB|
        |B|pA|pC|
        |C|pB|--|
```
```
   nó *anterior = vitima->ant;   // o nó que está antes do nó a ser removido

   prim pA
   ult  pC
   num  3

   vitima pB
   seguinte pC
   anterior pA

        |A|--|pB|
        |B|pA|pC|
        |C|pB|--|
```
```
   anterior->prox = seguinte;

   prim pA
   ult  pC
   num  3

   vitima pB
   seguinte pC
   anterior pA

        |A|--|pC|
        |B|pA|pC|
        |C|pB|--|
```
```
   seguinte->ant = anterior;

   prim pA
   ult  pC
   num  3

   vitima pB
   seguinte pC
   anterior pA

        |A|--|pC|
        |B|pA|pC|
        |C|pA|--|
```
```
   free(vitima);

   prim pA
   ult  pC
   num  3

   vitima pB
   seguinte pC
   anterior pA

        |A|--|pC|
        |C|pA|--|
```
```
   l->num--;

   prim pA
   ult  pC
   num  2

   vitima pB
   seguinte pC
   anterior pA

        |A|--|pC|
        |C|pA|--|
```
```
   return;

   prim pA
   ult  pC
   num  2

        |A|--|pC|
        |C|pA|--|
```
Esse código funciona quando se está removendo um nó intermediário, mas teremos problemas com o primeiro e o último nós (os ponteiros `antes` ou `depois` podem ser `NULL` (e não podem ser dereferenciados), e devemos alterar o descritor. Devemos tomar alguns cuidados especiais:
```c
   /// ...
   // remove o nó dos encadeamentos
   if (anterior != NULL) {
     anterior->prox = seguinte;   // o nó que segue o nó anterior ao removido é o que está depois do removido
   } else {
     l->prim = seguinte;       // o nó removido era o primeiro da lista, o que segue é o novo primeiro
   }
   if (seguinte != NULL) {
     seguinte->ant = anterior;    // o nó que antecede o nó que está após o removido é o que está antes do removido
   } else {
     l->ult = anterior;         // o nó removido era o último da lista, o antecessor dele é o novo último
   }
   /// ...
```
Esse código funciona para a remoção quando a lista tem um só elemento?

Na inserção, tem que tomar cuidados semelhantes.

Tendo os dois encadeamentos e o número de elementos, pode-se otimizar um pouco a busca de um elemento por posição: se a posição buscada for mais próxima do início que do final da lista, realiza-se a busca a partir do primeiro elemento em direção ao final da lista, e se for mais próxima do final, busca-se a partir do último elemento em direção ao início.

### Outras alternativas de implementação de listas

#### Ponteiro para o último elemento no descritor da lista simplesmente encadeada

Dessa forma, a operação de inclusão no final da lista pode ser otimizada, sem ser necessário percorrer a lista toda até o final. A remoção no final não é afetada, necessitando o percurso de toda a lista.

#### Lista circular

Ao invés de o último elemento do encadeamento ter um ponteiro em NULL, ele aponta para o primeiro elemento.
Em uma lista simplesmente encadeada, dá para nesse caso se manter no descritor somente um ponteiro para o último elemento e não para o primeiro, e ainda assim não necessitar percorrer a lista toda para inserção e remoção no início e inserção no final.

#### Uso de nó sentinela

Usa-se um nó suplementar, que é alocado na inicialização da lista e somente removido na sua destruição. Esse nó não contém um dado válido, serve simplesmente para garantir que sempre existe um nó no encadeamento, mesmo que a lista esteja vazia, e é para esse nó que o descritor aponta.
Geralmente é usado com a implementação circular de lista duplamente encadeada.
Torna mais simples as operações de inserção e remoção, porque não existem mais os casos em que se necessite alterar o ponteiro do descritor, porque ele aponta sempre para o sentinela.

#### Não uso do descritor

Nesse caso, usa-se um ponteiro para o primeiro nó para se representar a lista.
Economiza-se memória, mas a interface de acesso à lista fica menos clara, porque operações que alteram a lista (inserção e remoção) podem ter que alterar o ponteiro que representa a lista, que pertence ao usuário.
O uso de um nó sentinela pode evitar esse problema.

### Outros aspectos de listas (e de outras estruturas de dados)

#### Listas genéricas

Nas implementações de listas acima, o dado armazenado na lista tem o tipo 'dado_t'.
Como fazer se o programa necessita listas de mais de um tipo de dados?

Uma possibilidade é fazer com que a lista armazene ponteiros para os dados, e a memória para esses dados ser gerenciada pelo usuário da lista. Uma vantagem dessa forma é que um dado pode facilmente estar em mais de uma lista ao mesmo tempo, sem precisar ser copiado.
Como a lista não vai tocar nos dados, eles podem ter quelquer tipo, e os ponteiros armazenados pela lista podem ser do tipo `void *`.

Outra possibilidade é a lista saber quantos bytes cada dado tem (informado na inicialização da lista ou a cada inserção e remoção), alocar memória para o nó com espaço suficiente para esse dado, e copiar o dado para o nó a partir de um ponteiro fornecido pelo usuário, usando uma função de cópia de memória.

A linguagem C oferece uma "facilidade" para a declaração de uma estrutura que será alocada com espaço extra para isso: o último campo de uma estrutura pode ser um vetor sem tamanho. Por exemplo, o nó de uma lista duplamente encadeada poderia ser declarado assim:
```c
struct nó {
    nó *prox;
    nó *ant;
    char dado[];
};
```
O dado é declarado como um vetor de `char`, sem tamanho. O tipo é `char` porque não é possível declarar um vetor de `void`.
Uma operação `sizeof` nessa `struct` dá o tamanho de toda a estrutura exceto o vetor.
Se sabemos quantos bytes tem o dado, podemos alocar memória para a estrutura com o `sizeof` do nó somado ao número de bytes para o dado, e podemos copiar o dado do usuário, apontado por `pdado`, usando uma função de cópia de memória:
```c
  nó *n = malloc(sizeof(nó) + tamanho_do_dado);
  memcpy(n->dado, pdado, tamanho_do_dado);
```
