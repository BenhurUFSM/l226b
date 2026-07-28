## Vetor

Um **vetor** é uma variável que permite o armazenamento de vários valores independentes entre si, todos do mesmo tipo.
A declaração de um vetor é feita com o tipo dos dados, o nome do vetor e o número de elementos entre colchetes.
Por exemplo, para declarar o vetor `a`, capaz de conter 50 valores inteiros:
```c
  int a[50];
```
Os elementos de um vetor só podem ser acessados um por vez, com um **índice** inteiro que o identifica:
```c
  a[0] = 30;
  for (int j = 0; j < 10; j++) {
    a[j] = a[j + 10];
  }
```
A linguagem C não faz verificação dos índices para garantir que sejam válidos.
É responsabilidade do programador garantir que seu programa não faz acesso a um índice inválido (menor que 0 ou maior que N-1).

Em C, não existe atribuição de vetores, somente de elementos de vetor. 
O nome de um vetor tem o valor de uma referência ao seu primeiro elemento.

Na declaração, os valores de um vetor podem ser inicializados:
```c
  int v[5] = {6, 5, 7, 9, 2};
  int w[6] = { 6, [3] = 2, 7 }; // inicializado com 6,0,0,2,7,0
  int z[] = {6, 5, 7, 9, 2};    // o tamanho de z é 5
  char a[] = "bla";             // o tamanho de a é 4 (para conter o \0)
```
Caso tenha menos valores que o tamanho do vetor, os demais valores serão inicializados em 0. Não pode ter mais elementos na lista de inicialização que o número de elementos do vetor.


### Vetores como parâmetros de funções

Quando se passa um vetor para uma função, se está passando uma referência para o primeiro elemento dele:
```c
   int v[] = {1, 2, 3};
   f(v);
```
A função `f` acima receberá uma referência o elemento com valor `1` de `v`, e deve ser declarada de acordo, com uma das possibilidades:
```c
   void f(int x[]) ...
   void f
```
Independentemente da declaração de `f`, ela pode acessar os elementos do vetor com `x[i]`.
A função `f` não tem acesso direto ao tamanho do vetor, esse tamanho, se necessário, tem que ser gerenciado de alguma forma pelo programador.

