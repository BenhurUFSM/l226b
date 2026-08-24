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

Abaixo está uma possível implementação de operações de inicialização e inserção em uma lista encadeada com dois vetores, nos quais são mantidas duas listas, a lista propriamente dita e a lista de locais livres.

```c
// no .h tem typedef struct lista *Lista;
struct lista {
  int prim;         // índice do vetor onde está o primeiro dado da lista
  int livre;        // índice do vetor onde está o primeira pos. livre
  dado_t dados[N];  // vetor que contém os dados da lista
  int prox[N];      // vetor que contém os encadeamentos
};

// prox[i] contém o índice do vetor onde está o dado que segue o
//         dado que está no índice i do vetor

// inicializa a lista.
// a memória para a lista já foi alocada.
// a lista fica vazia (prim é -1, dizendo que não tem um índice válido
//   que armazene o primeiro dado da lista)
// todos as posições do vetor são colocadas na lista de livres
void l_init(Lista l)
{
  l->prim = -1;

  l->livre = 0;
  for (int i = 0; i < N - 1; i++) l->prox[i] = i + 1;
  l->prox[N - 1] = -1;
}

// encontra um índice livre, e retira da lista de livres
static int ind_livre(Lista l)
{
  // o teste de cheia pode ser l->livre == -1
  if (l_cheia(l)) erro(); // poderia realocar...
  // pega o índice do primeiro da lista de livres
  int ind = l->livre;
  // o primeiro livre passa a ser o seguinte
  l->livre = l->prox[ind];
  return ind;
}

// insere o dado d no início da lista l
void l_insere_inicio(Lista l, dado_t d)
{
  // acha um índice livre
  int novo = ind_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // o dado que segue o novo primeiro dado é o antigo primeiro
  // se a lista estava vazia, l->prim é -1, que é o que queremos como prox
  l->prox[novo] = l->prim;
  // atualiza a nova primeira posição
  l->prim = novo;
}

// insere o dado d no final da lista l
void l_insere_fim(Lista l, dado_t d)
{
  // se a lista estiver vazia, é mais fácil inserir no início...
  if (vazia(l)) { l_insere_inicio(l, d); return; }
  // acha um índice livre
  int novo = ind_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // ele é o novo último
  l->prox[novo] = -1;
  // acha a posição do último (vai virar penúltimo)
  // sabemos que a lista não está vazia, então l->prim é um índice válido
  // poderia poupar essa busca colocando a posição do último no descritor
  int ult;
  for (ult = l->prim; l->prox[ult] != -1; ult = l->prox[ult])
    /*nada*/;
  // o dado que segue o antigo último é o novo último
  l->prox[ult] = novo;
}

// insere o dado d na lista l, de forma que ele fique na posição p
void l_insere_pos(Lista l, dado_t d, int p)
{
  // se a inserção for no início, temos uma função pronta...
  if (p == 0 || l_vazia(l)) { insere_inicio(l, d); return; }
  // acha um índice livre
  int novo = ind_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // acha a posição do dado que deve anteceder o novo dado
  // caso a lista não tenha elementos suficientes, acha o último
  int anterior = l->prim;  // índice de um dado no vetor
  int pos_anterior = 0;    // posição na lista correspondente a esse dado
  while (pos_anterior < p - 1) {
    if (l->prox[anterior] == -1) {
      // chegamos no último da lista!
      break;
    }
    anterior = l->prox[anterior];
    pos_anterior++;
  }
  // o índice do dado seguinte
  int seguinte = l->prox[anterior];
  // o dado novo fica após o anterior
  l->prox[anterior] = novo;
  // depois do novo fica o que estava depois do anterior
  l->prox[novo] = seguinte;
}
```

#### Exercícios

Faça as funções de remoção, teste a lista.

