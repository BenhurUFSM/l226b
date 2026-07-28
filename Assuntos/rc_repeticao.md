## Comandos de repetição

Os comandos de repetição permitem repetir a execução de um bloco.
A decisão de executar ou não os comandos do bloco é tomada a partir do cálculo de uma expressão lógica: se o valor da expressão for verdadeira, o bloco é executado novamente, e se for falsa o comando de repetição termina.

A linguagem C tem 3 comandos de repetição: `while`, `do while` e `for`.

### Comando `while`

O comando `while` tem o seguinte formato:
```c
   while (expressão)
     bloco
```
Inicialmente, calcula o valor da *expressão*, que deve produzir um valor lógico. Se for falso, o comando `while` termina. Se for verdadeiro, os comandos do *bloco* são executados e a expressão é recalculada, repetindo o ciclo.

### Comando `do while`

O comando `do while` tem o seguinte formato:
```c
   do {
     comandos
   } while(expressão);
```
Executa os *comandos*, e depois calcula o valor da *expressão*. Se for falso, o comando `do` termina; se for verdadeiro, os comandos são reexecutados, repetindo o ciclo.

### Comando `for`

O comando `for` tem o seguinte formato:
```c
   for (inicialização; expressão; incremento)
     bloco
```
Inicialmente, executa a *inicialização*, que tipicamente declara e inicializa uma variável, chamada de *variável de controle*. Então, calcula o valor da *expressão*, tipicamente um teste sobre a variável de controle. Se o valor da expressão for falso termina o comando `for` e se for verdadeiro executa os comandos do *bloco*. Após a execução do bloco, executa o *incremento*, que tipicamente altera o valor da variável de controle, e volta a calcular o valor da expressão, repetindo o ciclo.

* * *

Em um bloco de um comando de repetição, além dos demais comandos da linguagem, pode-se usar dois comandos de controle:
- `break` — se for executado, causa o encerramento do comando de repetição;
- `continue` — se for executado, causa o encerramento da repetição atual, como se tivesse executado o último comando do bloco.
