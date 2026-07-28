## Variáveis

Uma variável representa uma região de memória, que contém um valor de determinado tipo. Uma variável tem um nome, que é como se refere a essa região de memória no programa, tanto para obter o valor ali armazenado quanto para alterá-lo.

```c
    int a; // declara a variável chamada a, que conterá um valor do tipo int
    a = 25 + a; // altera o valor de a para 25 além do valor que ela já possui
    float b = 45.2; // declara e inicializa a variável b

```
Uma variável pode ser declarada fora de funções, sendo chamada de 'global'. Nesse caso, ela é acessável por qualquer função, e existe durante toda a execução do programa.

Uma variável é mais comumente declarada em um bloco de alguma função, sendo chamada de local à esse bloco. Nesse caso, a memória para essa variável só é alocada cada vez que seu bloco inicia a execução e liberada quando o bloco termina sua execução. Essa variável só é visível no interior desse bloco.

Duas variáveis não podem ter o mesmo nome se estiverem no mesmo bloco ou se forem ambas globais. Um nome sempre se refere à variável declarada no bloco mais interno que contém o comando que a referencia, quando mais de uma variável tem o mesmo nome.
