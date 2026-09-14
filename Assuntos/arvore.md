
# Árvores

Uma árvore é uma estrutura de que organiza dados de uma forma hierárquica.
Um exemplo de tal hierarquia seria a organização de diretórios e arquivos em um disco.

Cada dado em uma árvore é armazenado em um "nó".
Uma árvore é um conjunto de nós, com as seguintes características:
- se o conjunto de nós está vazio, a árvore é vazia;
- senão, um dos nós do conjunto é chamado de "raiz" da árvore, e os demais são divididos em subconjuntos disjuntos, cada um deles uma árvore, chamadas de subárvores da raiz.

Existem várias formas de se representar graficamente uma árvore, a mais comum é representar um nó com um círculo, dentro do qual é colocado o valor do nó.
O círculo que corresponde à raiz acima dos que correspondem às raízes de suas subárvores, com setas (também chamadas arestas) ligando a raiz da árvore às raízes das subárvores.

Alguns conceitos de árvores:
- filhos de um nó: raízes das subárvores da árvore que tem esse nó como raiz;
- nó raiz: nó da árvore que não tem pai (uma árvore tem um único nó raiz);
- nó folha ou nó externo: nó da árvore que não tem filho;
- nó interno: nó que tem filho;
- grau de um nó: número de filhos que o nó tem;
- grau de uma árvore: maior grau encontrado em um nó da árvore;
- caminho em uma árvore: sequência de nós (não repetidos) da árvore, onde dois nós adjacentes têm a relação pai-filho;
- comprimento de um caminho: número de arestas em um caminho, um a menos que o número de nós do caminho (alguns autores consideram que o comprimento de um caminho é o número de nós do caminho, mas aparentemente esses autores são minoritários);
- profundidade de um nó: comprimento do caminho entre a raiz da árvore e esse nó;
- nível de uma árvore: conjunto de nós que estão na mesma profundidade; a raiz está no nível 0, seus filhos no nível 1, os filhos deles (netos da raiz) no nível 2, etc;
- altura de uma árvore: comprimento do maior caminho entre a raiz e alguma folha da árvore ou profundidade do nó mais profundo da árvore; uma árvore somente com o nó raiz tem altura 0; uma árvore vazia tem altura -1.

Quando a ordem dos filhos de um nó é importante, diz-se que a árvore é **ordenada**. Nesse caso, a árvore que tem a raiz A e os filhos B e C é considerada diferente da árvore que tem raiz A e filhos C e B. Se as árvores forem não ordenadas, essas duas árvores são consideradas iguais.

Nesta disciplina, nos concentraremos em um tipo especial de árvores, as árvores de grau 2, ou *árvores binárias*.

## Árvores binárias

Em uma árvore binária, cada nó tem no máximo 2 filhos. Nos restringiremos ao estudo de árvores ordenadas, então a ordem entre esses dois filhos é uma informação importante. Como são só dois filhos, eles são comumente chamados de filho esquerdo e direito.

Árvores binárias são estruturas definidas de forma recursiva, uma árvore binária pode ser:
- vazia; ou
- um nó, chamado de raiz e dois filhos, esquerdo e direito, que são árvores binárias.

Uma forma comum de implementação de uma árvore é como um ponteiro para um nó, e esse nó contém o valor associado ao nó e dois ponteiros para os nós raiz das árvores filhas. Uma árvore vazia pode ser implementada como um ponteiro nulo ou como um nó cujo valor é ignorado e tem os dois ponteiros para nulo. Na primeira implementação, será alocado um nó para cada valor armazenado na árvore. Na segunda, será alocado, além desses, um nó a mais para cada filho vazio, duplicando o número de nós ocupados pela árvore, mas simplificando a interface de funções que alteram a árvore (veremos mais tarde na discussão sobre inserção e remoção). Uma forma alternativa é reutilizar o mesmo nó para representar qualquer árvore vazia.

```c
   typedef struct nó nó;
   struct nó {
     dado_t val;
     nó *esq;
     nó *dir;
   };
   typedef nó árvore;  // um sinônimo, uma árvore é representada por um ponteiro para um nó (raiz)
```

Grande parte dos algoritmos que operam sobre árvores são mais facilmente implementados em forma recursiva.
Por exemplo, para calcular quantos nós tem uma árvore, pode-se usar uma definição recursiva:
   - uma árvore vazia tem 0 nós
   - uma árvore não vazia tem um nó a mais que a soma dos nós de suas duas subárvores.
Em código C:
```c
   int arv_num_nós(árvore *a)
   {
      if (arv_é_vazia(a)) return 0;
      return 1 + arv_num_nós(a->esq) + arv_num_nós(a->dir);
   }
```
Para calcular a altura de uma árvore:
   - uma árvore vazia tem altura -1
   - uma árvore não vazia tem altura que é um além da maior altura entre as árvores filhas.
Em código C:
```c
   int altura(árvore *a)
   {
      if (arv_é_vazia(a)) return -1;
      return 1 + maior(altura(a->esq), altura(a->dir));
   }
```
A implementação dessas funções de forma não recursiva é um tanto mais trabalhosa.

A função que testa se uma árvore é vazia testa se o ponteiro é NULL no caso de se implementar ávore vazia como ponteiro NULL ou testa se os ponteiros esq e dir são NULL, se for essa a implementação escolhida, ou se é igual à árvore vazia única.

### Exercícios

Considere nós que têm valores do tipo int onde for necessário.
1. Faça uma função que retorna a soma de todos os valores da árvore.
1. Faça uma função que retorna o maior entre os valores da árvore.
1. Faça uma função que recebe uma árvore e retorna quantos nós folha ela tem.
2. Faça uma função que recebe uma árvore e retorna quantos nós não folha ela tem.
3. Faça uma função que recebe uma árvore e um valor e retorna um booleano que diz se a árvore contém ou não um nó com esse valor.
3. Faça uma função que recebe uma árvore e um valor e retorna um int que diz quantas vezes esse valor aparece na árvore.
4. Faça uma função que recebe uma árvore e um valor e retorna o nível nessa árvore que está o nó que contém esse valor, ou -1 se o valor não estiver em nenhum nó da árvore. A raiz de uma árvore está no nível 0, os filhos da raiz no nível 1 etc. Se o valor estiver em mais de um local da árvore, qualquer deles serve.
5. Faça uma função que recebe uma árvore e retorna o "fator de equilíbrio" do nó raiz dessa árvore. O "fator de equilíbrio" a ser considerado é a diferença entre a altura da subárvore esquerda e a da direita.

### Dicas

Uma função que pode auxiliar na depuração é uma que imprime uma árvore. Por exemplo:
```c
void imprime(árvore *a, int nivel)
{
   if (arv_é_vazia(a)) return;
   printf("%*s", nivel * 3, ""); // imprime 3*nivel espacos
   printf("%d\n", a->dado);
   imprime(a->esq, nivel + 1);
   imprime(a->dir, nivel + 1);
}
```

Uma forma de criar uma árvore é com uma função que cria um nó:
```c
árvore *arv_cria(dado_t dado)
{
   árvore *a = malloc(sizeof(*a));
   assert(a != NULL);
   a->dado = dado;
   a->esq = NULL;
   a->dir = NULL;
   return a;
}
//...
   árvore *a = arv_cria(10);
   a->esq = arv_cria(2);
   a->esq->dir = arv_cria(5);
   a->dir = arv_cria(17);
   imprime(a, 0);
//...
```

Outra forma:
```c
   árvore *no(dado_t d, árvore *esq, árvore *dir)
   {
      árvore *n = malloc(sizeof(árvore));
      assert(n != NULL);
      n->val = d;
      n->esq = esq;
      n->dir = dir;
      return n;
   }
   //...
   árvore *a = no(1, no(2, NULL, NULL), no(3, NULL, NULL));
```

Ainda outra forma, com a árvore vazia representada por um nó com ponteiros NULL:
```c
   bool arv_é_vazia(árvore *a)
   {
      assert(a != NULL);
      return a->esq == NULL && a->dir == NULL;
      // ou return a == vazia global
   }

   árvore *arv_cria(void)
   {
      árvore *n = malloc(sizeof(árvore));
      assert(n != NULL);
      // o valor nunca vai ser usado
      n->esq = NULL;
      n->dir = NULL;
      return n;
      // ou só return a árvore vazia global
   }
   
   void arv_troca_valor(árvore *a, dado_t d)
   {
      if (arv_é_vazia(a)) {
         // vai deixar de ser uma árvore vazia, coloca árvores vazias como filhos
         a->esq = arv_cria();
         a->dir = arv_cria();
      }
      a->val = d;
   }
   //...
   árvore *a = arv_cria();
   arv_troca_valor(a, 1);
   arv_troca_valor(a->esq, 2);
   arv_troca_valor(a->dir, 3);
```

### Funções recursivas

Como ler: pense no que a função vai fazer se receber uma árvore vazia, depois se receber um nó folha, depois um nó com um filho folha, etc, até se convencer que funciona.

Como escrever: pense no que deve ser feito se a árvore for vazia. Faça de conta que a função já existe, e que obtém o que deve ser obtido das subárvores. Se com esses valores mais o valor do nó se consegue calcular o que deve ser obtido com a árvore inteira, não deve ser difícil de escrever a função. Se com esses valores não tem como calcular o que é necessário, não é uma função recursiva que vai resolver o problema.
