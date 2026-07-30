## Ponteiros e referências

Uma referência é uma posição de memória, geralmente onde está uma variável. Pode-se obter uma referência a uma variável com o operador unário `&`.

Um ponteiro é uma variável cujo valor é uma referência. Por meio de um ponteiro, pode-se acessar o valor da variável referenciada (essa operação se chama dereferenciação, e é realizada pelo operador unário `*`).
Só se pode dereferenciar um ponteiro que referencie uma posição válida de memória (uma variável ativa ou uma região de memória alocada dinamicamente e ainda não liberada).
Com referências e ponteiros, pode-se acessar em uma função variáveis às quais essa função não tem acesso diretamente (por serem locais a outra função).

As referências e os ponteiros são tipados, isto é, eles referenciam dados de determinado tipo. Só de pode atribuir a um ponteiro de certo tipo uma referência a esse mesmo tipo.

Exemplo:
```c
   void troca(int *p1, int *p2)
   {
       int t = *p1;  // coloca em t o valor referenciado por p1
       *p1 = *p2;    // coloca na posição ref. por p1 o valor ref. por p2
       *p2 = t;      // coloca na posição ref. por p2 o valor de t
   }

   //... em outra função
   int a = 10;
   int b = 25;

   troca(&b, &a); // passa referências para os inteiros a e b, que serão
                  //   atribuídas aos parâmetros p1 e p2 (ponteiros para
                  //   int) da função troca
   // agora a vale 25 e b vale 10
```
