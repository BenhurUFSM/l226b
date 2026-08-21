## Trabalho 2 - implementação de TAD - string

### Alterações
- 19ago - versão inicial
- 20ago - alteração na interface:
   - funções que retornam uma nova string iniciam com `s_cria`.
   - inclusão do tipo `Str_c`, para declarar parâmetros que são strings que não serão alteradas pela função.
   - alterações na interface para usar `Str_c` para parâmetros não alteráveis.
   - inclusão de `s_insere_c` e `s_anexa_c`, para inclusão de caracteres individuais em uma string.
   - inclusão de sugestão de uso de `fread` e `fwrite`.

### Descrição

Implemente o tipo abstrato de dados *string* como descrito abaixo.

São fornecidos os seguintes arquivos:
- `utf8.h` - contém a declaração e descrição de funções para facilitar o manuseio de caracteres unicode codificados em UTF8. Contém também a declaração dos tipos `byte` e `unichar`, para representar o conteúdo de um byte e o código de um caractere unicode.
- `utf8.c` - contém a implementação das funções descritas em `utf8.h`.
- `str.h` - contém a declaração do tipo `Str` que será usado para representar um dado do TAD string. Contém também a descrição e declaração das funções que implementam as operações possíveis com dados desse tipo.
- `str.c` - contém uma implementação completamente incompleta das funções descritas em `str.h`.
- `teste_str.c` - contém um programa incompleto para testar o TAD.

Sua missão é completar o arquivo `str.c`, de forma a que o TAD funcione como descrito.
Complemente o programa de testes, para realizar mais testes no TAD.

Não deve haver erros de memória, como liberação de mais ou menos memória do que foi alocada, ou acesso a região de memória inválida.

### Dicas

- Para compilar e executar o programa de teste, dá para usar:
```
   gcc -g -o teste_str teste_str.c utf8.c str.c
   ./teste_str
```
A opção `-g` serve para o compilador colocar algumas informações adicionais no executável, que facilita o comportamento de programas de depuração como gdb e valgrind.
- Os conjunto de programas valgrind podem ajudar a detectar erros de memória. Por exemplo, o comando abaixo executa o programa acima e produz um relatório com erros de alocação (remova alguma chamada a s_destroi para ver a diferença):
```
   valgrind ./teste_str
```
- Para leitura e escrita de strings, use as funções `fread` e `fwrite`.


### RAP

- A função s_ok verifica se a string está "de acordo com a especificação", quais são as especificações da string?

   É para ser uma função de auxílio à depuração, para verificar se a struct que implementa a string não tem algum problema fácil de ser detectado, que não deveria existir, e com o qual o programa não deve continuar.

   Por exemplo, tem a informação de que a string tem tantos caracteres, mas o ponteiro para os dados é NULL, ou que a string é vazia e o ponteiro não é NULL.
   Ou tem mais caracteres que bytes. Depende do que for colocado na struct que implementa a string.
 
- Quando `str.c` mexe nas strings, as strings já foram convertidas para UTF8?

   Todas as funções que mexem internamente nas strings estão em `str.c`. Não existe string que seja mexida por `str.c` que não tenha sido criada e alterada exclusivamente pelas funções em `str.c`. Elas não devem permitir que a string seja inválida.

- Tem certeza que é "Rica" e não "áica"?

   Ops!

