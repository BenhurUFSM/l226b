### Listas encadeadas

#### Listas encadeadas com alocação contígua

Nas implementações contíguas, a ordem física em que os dados da lista são mantidos em memória corresponde à ordem lógica desses dados na lista.
Sabendo a posição de um dos dados, com um cálculo simples se obtém a posição de qualquer outro.
Em compensação, se um dado muda sua posição lógica na lista (devido a inclusões ou remoções), os dados devem ser fisicamente movimentados na memória para que a correspondência seja mantida.

Uma outra forma de se organizar uma lista em memória é quebrando essa correspondência, e usando uma outra forma de se encontrar os dados da lista em vez de sua posição relativa na memória.
A forma mais comum de se fazer isso é manter, para cada dado na lista, uma informação que permite chegar ao dado que o sucede na lista.
Por exemplo, se os dados da lista são colocados em um vetor, a informação que permite chegar a um dado é a posição no vetor.
Uma forma de se manter essa posição é colocando a lista em dois vetores de mesmo tamanho, um contendo espaço para os dados e outro contendo a posição do sucessor de cada dado. A posição onde está o dado que é o sucessor do dado na posição 3 do vetor de dados é colocada na posição 3 do vetor de posições. 

Cada dado da lista pode estar em qualquer posição do vetor, então necessitamos saber a posição do primeiro dado da lista.
Quando uma lista é organizada dessa forma, com um dado levando à informação do seguinte e assim por diante, dizemos que a lista é "encadeada".

Por exemplo, nesse esquema, a lista com os dados `X Y Z` poderia ser armazenada como abaixo:
```
   cap 5
   pri 2
   num 3
   dad  Z ? X Y ?
   suc -1 ? 3 0 ?
        0 1 2 3 4   (índices nos vetores)
        2   0 1     (índices na lista)
```
Os sinais `?` marcam valores nos vetores que não estão sendo ocupadas pela lista.
O primeiro dado da lista está na posição 2, informação dada por "pri". Nessa posição está o dado `X`.
No vetor de sucessores ("suc"), na mesma posição do dado (2), tem o valor 3, que é a posição do dado seguinte (o segundo dado da lista).
Na posição 3, está o valor `Y` e o sucessor é 0.
Na posição 0 está o terceiro dado da lista, `Z` e o sucessor, -1. O valor -1, que não representa um índice válido nos vetores é um valor especial usado para representar que esse dado não tem sucessor, é o último da lista.

Se dessa lista fosse removido o segundo dado (na posição 1 da lista e 3 do vetor), a lista ficaria assim:
```
   cap 5
   pri 2
   num 2
   dad  Z ? X Y ?
   suc -1 ? 0 0 ?
        0 1 2 3 4   (índices nos vetores)
        1   0       (índices na lista)
```

Se na lista original fosse inserido o dado W na segunda posição da lista (entre o X e o Y), a lista poderia ficar assim:
```
   cap 5
   pri 2
   num 4
   dad  Z W X Y ?
   suc -1 3 1 0 ?
        0 1 2 3 4   (índices nos vetores)
        3 1 0 2     (índices na lista)
```
ou assim:
```
   cap 5
   pri 2
   num 4
   dad  Z ? X Y W
   suc -1 ? 4 0 3
        0 1 2 3 4   (índices nos vetores)
        3   0 2 1   (índices na lista)
```
Tanto na inserção quanto na remoção, nenhum dado presente na lista precisou ser movimentado, o que é um dos motivos para se usar o encadeamento.

Claro, tem algumas desvantagens nessa implementação:
- é necessário mais memória, para colocar informação sobre os sucessores
- é necessário encontrar os espaços livres quando se tem uma inclusão. Uma forma de se fazer isso é organizar uma segunda lista no mesmo vetor, contendo os elementos livres, e retirar dela nas inserções, colocar nela nas remoções
- para implementar inclusão e remoção, tem que alterar a informação de sucessor do dado na posição anterior ao ponto de inserção/remoção; dependendo da posição, encontrar esse dado pode levar a percorrer a lista desde o início.
- caso se use realocação de memória que libera espaço quando a área ocupada pela lista é muito inferior à capacidade, pode ser necessário deslocar dados que estejam na área que será liberada (já que um dado pode estar em qualquer lugar, não se tem garantia de que não tenha dado no final da área alocada), além de rearranjar a lista de livres.

#### Código exemplo

No diretório "Complementos", tem uma interface para um TAD lista no arquivo lista.h, uma implementação exemplo parcial dessa interface, para lista encadeada em vetor em lista_ev.c e um programa simples de teste em testa_lista.c.
Nos vetores são mantidas duas listas, a que contém os dados e a que contém as posições livres do vetor.


#### Exercícios

1. Implemente as funções faltantes na em lista_ev.c. Complemente os testes em lista_ev.c.

Algumas operações na lista ficam mais rápidas colocando o número de elementos atual no descritor da lista, e o índice do último elemento.
Compare [lista_ev.c](../Complementos/lista_ev.c) com [lista_ev_n.c](../Complementos/lista_ev_n.c).
Aproveita e ajeita as funções de remoção...

Uma outra forma de organizar as informações da lista é trocando os dois vetores, um de dados e um de índices por um só vetor, contendo registros (chamados nós da lista).
Cada nó contém o dado e o índice do próximo nó.
Dessa forma, fica mais claro que essas duas informações são interligadas.
Compare [lista_ev_n.c](../Complementos/lista_ev_n.c) com [lista_evr.c](../Complementos/lista_evr.c).
