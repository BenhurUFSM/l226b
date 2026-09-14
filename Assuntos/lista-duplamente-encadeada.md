
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

#### Percurso da lista

Um percurso em uma lista (ou em outra estrutura que é um container) é a realização de uma "visita" a cada um dos dados armazazenados na lista.
O que é feito nessa visita depende da necessidade do usuário, pode ser para E/S, ou para pesquisa, ou para filtragem, ou para alteração, etc.
Existem várias formas de se implementar um percurso. Pode ser um percurso dedicado (uma função para imprimir o conteúdo da lista, por exemplo), ou um percurso que dê ao usuário a possibilidade de escolher o que fazer com cada dado vistado, que é bem mais útil.

Uma forma é fornecendo ao usuário uma forma de acesso que permite o acesso a qualquer dado (por exemplo, por posição), e uma forma de saber quantos e quais dados são acessáveis, e o usuário implementar um laço. Por exemplo:
```c
   Lista l;
   // ... manipula a lista, de forma que ela contenha dados interessantes
   int n = l_num_dados(l);
   for (int i = 0; i < n; n++) {
       dado_t d = l_dado_na_posição(l, i);
       faz_algo_com_o_dado(d);
   }
```
Essa solução tem a desvantagem de colocar no código do usuário a mecânica do percurso, e de dificultar uma implementação mais otimizada do percurso, que seria mais facilmente implementada com acesso à estrutura interna da lista. Em uma lista encadeada, por exemplo, é muito fácil obter o dado seguinte tendo o dado anterior, mas é bem mais caro obter um dado qualquer por posição.

Uma outra implementação possível é passar todo o controle para a lista, em uma função que faz todo o percurso. Para que o usuário tenha controle sobre o que se quer fazer com cada dado, é usual passar para a lista uma função que ela deve chamar para cada dado. Algo assim:
```c
   Lista l;
   // ... manipula a lista, de forma que ela contenha dados interessantes
   l_para_todos(l, faz_algo_com_o_dado);
```
Essa forma de percurso tem algumas dificuldades operacionais: tem que tem uma forma de se passar uma função para a lista poder chamá-la (para não ser uma só coisa a fazer com os dados e para o usuário ter controle sobre o que será feito), e em casos menos simples, essa função pode ter necessidade de alguma informação adicional para realizar o que tem que fazer, além do dado. Por exemplo, poderia ser uma função que grava os dados em um arquivo, ela teria que saber que arquivo é esse. Isso poderia ser resolvido usando variáveis globais ou passando mais uma informação para a lista, para ela repassar para a função. Algo como:
```c
   Lista l;
   // ... manipula a lista, de forma que ela contenha dados interessantes
   l_para_todos(l, grava_o_dado, arquivo);
```
A lista nesse caso, chamaria, para cada dado, `grava_o_dado(dado, arquivo);`.

Uma terceira possibilidade é ter uma estrutura auxiliar que permite a iteração da lista (um iterador). Essa estrutura é definida pela lista, e contém as informações necessárias para saber em que ponto da lista está o percurso e para obter o próximo elemento da lista a ser visitado. Essa estrutura pode ser mantida internamente na lista ou fornecida ao usuário. O percurso ficaria algo assim:
```c
   Lista l;
   // ... manipula a lista, de forma que ela contenha dados interessantes
   Iterador it = l_percurso(l);
   while ((d = it_próximo(it)) != NÃO_TEM_MAIS_DADOS) {
       faz_algo_com_o_dado(d);
   }
```
ou, com o iterador gerenciado internamente:
```c
   Lista l;
   // ... manipula a lista, de forma que ela contenha dados interessantes
   l_inicia_percurso(l);
   while ((d = l_próximo(it)) != NÃO_TEM_MAIS_DADOS) {
       faz_algo_com_o_dado(d);
   }
```

##### Ponteiros para função

A declaração abaixo diz que `fun` é uma função que recebe um `int` e não retorna nada:
```c
void fun(int);
```
Colocando um `typedef` na frente, se está declarando um tipo (o tipo que representa uma função que recebe um `int` e não retorna nada), e `fun` poderia ser declarada, de forma equivalente, assim:
```c
typedef void tf(int);
tf fun;
```
Não dá para declarar uma variável que é uma função, mas pode-se declarar uma variável que é um ponteiro para uma função. Essa variável pode ser inicializada para apontar para qualquer função compatível (no caso, que recebe um `int` e não retorna nada). Para chamar a função apontada por um ponteiro, usa-se o nome do ponteiro como se ele fosse o nome da função apontada. O código abaixo executará a função `fun` com argumento `52`:
```c
#include <stdio.h>

typedef void tf(int);
tf fun;

int main()
{
  tf *ff = &fun;
  ff(52);
}

void fun(int x)
{
  printf("%d\n", x);
}
```
O nome de uma função, sem parênteses, representa uma referência para a função, não sendo necessário usar o operador `&`.
Como não se pode declarar uma variável que é uma função e só variáveis que são ponteiros para função, é mais comum codificar o programa acima de forma equivalente, mas ainda menos clara:
```c
#include <stdio.h>

typedef void (*tpf)(int);
void fun(int);

int main()
{
  tpf ff = fun;
  ff(52);
}

void fun(int x)
{
  printf("%d\n", x);
}
```
Um ponteiro para função pode ser passado para outra função:
```c
#include <stdio.h>

typedef void (*tpf)(int);
void fun(int);

void processa_vetor(int n, int v[n], tpf função)
{
    for (int i = 0; i < n; i++) {
        função(v[i]);
    }
}

int main()
{
    int vet[] = { 1, 7, 15, 3, 18 };
    processa_vetor(5, v, fun);
}

void fun(int x)
{
  printf("%d\n", x);
}
```
