## Tipos de dados

A linguagem C tem um grande repertório de tipos básicos de dados.
Uma divisão forte é entre tipos inteiros e de ponto flutuante.

Em ordem não decrescente, os **tipos inteiros** são:
- bool
- char
- short (ou short int)
- int
- long (ou long int)
- long long (ou long long int)

O tipo `bool` só tem dois valores possíveis, 0 e 1, normalmente chamados por nome, `false` e `true`. Esse tipo é novo na linguagem, foi introduzido em C23. Em compiladores mais antigos, pode ser necessário incluir `stdbool.h` para ter acesso aos nomes `bool`, `false` e `true`.

Todos esses tipos exceto `bool` podem ser antecedidos por `signed` ou `unsigned`, para serem considerados com ou sem sinal.

O tipo `char` é usado para representar pequenos inteiros, tipicamente o código ASCII de um caractere (por isso o nome). Tipicamente é implementado com 8 bits. O padrão da linguagem não define se esses bits são interpretados com ou sem sinal.

O tipo `short` tem pelo menos 16 bits, o tipo `long` tem pelo menos 32.
Os demais tipos tem um tamanho que não é menor que o tipo anterior.
Em geral, em um computador "normal" atual, `short` tem 16, `int` tem 32, `long` tem 64 e `long long` tem 64 também (às vezes 128).

Caso se necessite de tamanhos específicos, incluindo `stdint.h` tem-se tipos como `int8_t`, `uint32_t` etc. C23 incluiu ainda tipos como `_BitInt(8)`

Os tipos de **ponto flutuante** são:
- float
- double
- long double

O tipo `float` é implementado com 32 bits, e pode representar aproximadamente valores entre 10⁻³⁰ e 10³⁰, positivos e negativos, com aproximadamente 7 casas decimais de precisão. O tipo `double` é normalmente implementado com 64 bits, podendo representar valores aproximadamente entre 10⁻³⁰⁰ e 10³⁰⁰, com aproximadamente 16 casas decimais de precisão. O tipo `long double` é às vezes maior.

