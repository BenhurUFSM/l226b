## Funções

Funções são as unidades executáveis de um programa em C.
Comandos executáveis só podem ser colocados dentro de funções.
Para ser executada, uma função deve ser chamada.
A única função que é chamada automaticamente é a que se chama `main`.
Uma função pode receber valores da função que a chama, em variáveis especiais chamadas parâmetros. A função chamadora deve prover esses valores, chamados argumentos.
Uma função pode ter variáveis internas, chamadas locais, que são criadas quando a função é chamada e destruídas quando a função termina sua execução e retorna à função que a chamou.
A função pode produzir um valor de retorno, que é entregue à função chamadora.

Uma função tem um tipo (que é o tipo do valor de retorno), um nome, e 0 ou mais parâmetros. A função tem também um corpo, que é onde estão codificados os comandos que a função deve executar quando for chamada.
```c
   // a função f recebe um parâmetro (x, inteiro) e retorna um int
   int f(int x)
   { // o corpo da função é colocado entre chaves
       int r = x * x; // variável local à função f
       return r;  // retorna o valor de r à função que chamou
   }

   //... no corpo de outra função
   int a;
   a = f(50 + 2); // o valor 50+2 é passado para o parâmetro de f,
                  // e o valor de retorno de f é colocado em a
```
A organização de um programa em funções é essencial para a modularização e legibilidade de um programa. Uma função deve fazer uma tarefa bem definida, e sempre que em algum lugar do programa se necessita que essa tarefa seja feita, essa função deve ser chamada. O nome de uma função deve levar facilmente à identificação dessa tarefa.
