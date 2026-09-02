## Trabalho 3 - calculadora (e lista duplamente encadeada)

### Descrição

Implemente uma calculadora, como descrito abaixo.

São fornecidos os seguintes arquivos:
- `str.h` - interface do TAD Str, igual o do t2, com a adição de algumas funções
- `lista.h` - interface do TAD Lista, que deve ter uma lista de Str
- `calc.h` - interface da função que deve implementar a calculadora
- `utf8.h`, `utf8.c` - iguais aos do t2

Você deve completar o arquivo `str.c` do t2, para incluir as funções adicionadas em `str.h`.
Você deve implementar o TAD Lista em `lista.c`, como uma lista duplamente encadeada, com nó sentinela.
Você deve implementar em `calc.c` as funções cujas interfaces estão em `calc.h`.

A função `calculadora` deve usar a função `tokeniza` para separar a string em uma lista de substrings chamadas *tokens*.
A função `calculadora` deve ter mais duas listas, que implementam pilhas, uma para operandos e outra para operadores.
Essa lista deve ser acessada um *token* por vez, e classificá-los em operador (se for um entre `+-*/^()=` ou operando (se for uma sequência de dígitos talvez com um ponto) ou erro.
Se o token for operando, deve empilhá-lo na pilha de operandos.
Se o token for operador, deve compará-lo com o topo da pilha de operadores, e realizar uma das operações:
- Acusa erro de falta de `(`
- Acusa erro de falta de `)`
- Empilha o operador na pilha de operadores
- Descarta o operador
- Opera — desempilha um operador e executa uma função correspondente a esse operador, passando a pilha de operandos para ela. Depois, volta a comparar o operador lido da entrada com o do topo da pilha.
- Termina o cálculo

A decisão sobre qual dessas opções executar está codificada na tabela abaixo.
A tabela mostra na vertical o operador do topo da pilha e na horizontal o operador da entrada.
O operador V representa a pilha vazia e o operador F representa o final da entrada.

Em caso de erro, o cálculo é interrompido, e a função retorna uma string informando o erro.

No término do cálculo, deve sobrar somente um elemento na lista de operandos, e é esse o valor a retornar. Caso contrário, é erro.

A função que realiza uma operação deve retirar os operandos necessários do topo da pilha e colocar o resultado também no topo.
O valor de um operando deve ser obtido por uma função. Essa função chama a função necessária de Str. Na fase 2 essa função será aumentada.
Em uma operação, se não houver operandos suficientes na pilha, é erro.

Os dados temporários usados para o cálculo devem ser liberados (lista, pilhas).

Tabela de operações:
p\\e    |  F | +- |\*/ |  ^ |  ( |  )
--      | -- | -- | -- | -- | -- | --
**V**   |  T |  E |  E |  E |  E | Er
**+-**  |  O |  O |  E |  E |  E |  O
**\*/** |  O |  O |  O |  E |  E |  O
**^**   |  O |  O |  O |  E |  E |  O
**(**   | Er |  E |  E |  E |  E |  D

O programa principal deve ler um arquivo de entrada, separá-lo em uma lista de linhas, chamar a função de cálcula para cada linha da lista, colocar cada resultado em uma lista de saída, gravar um arquivo de saída com uma resposta por linha.

### Parte II

Inclua suporte a variáveis na calculadora.

*descrição breve*
