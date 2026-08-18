# Tipos abstratos de dados (TAD)

Um tipo abstrado de dados permite ao programador aumentar o nível de abstração de seus programas, definindo claramente os valores que dados de um tipo criado podem ter e que operações exclusivas podem ser realizadas sobre dados desse tipo.

Os livros da bibliografia têm, quase todos, capítulo sobre TAD...

## Implementação de TAD em C

Um tipo abstrato de dados é constituído por:
- descrição do conjunto de valores que um dado desse tipo pode ter
- as operações que podem ser realizadas sobre dados desse tipo.

É importante ter uma separação entre a implementação de um TAD e seu uso.
Para que essa separação seja possível, se necessita de uma boa definição da interface entre essas partes.

Na disciplina, iremos implementar TADs de acordo com a descrição abaixo.

Deve ser escolhido um nome que identifique o TAD (aqui referenciado como *tad*), e um prefixo para facilitar a identificação das funções que implementam suas operações (aqui referenciado como *tad_*).
Cada TAD será implementado em dois arquivos que só serão usados para esse fim.
A interface pública do TAD (a declaração do tipo, a declaração das funções que implementam as operações públicas sobre dados desse tipo, constantes necessárias para uso desse tipo etc) será colocada em um arquivo de cabeçalho chamado "*tad*.h".

Todo TAD será implementado como uma struct, de conteúdo só conhecido pela implementação do TAD. Os clientes do TAD (programas que usam o TAD) sempre acessam o TAD através de ponteiros.
Para que o tipo de dados seja opaco aos clientes (eles não tenham acesso aos dados internos que constituem o TAD), nosso TAD será declarado no .h como
```c
   typedef struct tad *Tad;
```
O tipo *Tad* é um ponteiro para uma estrutura do tipo *struct tad*.
Os clientes usarão o tipo *Tad* (e não a *struct tad*) para declarar suas variáveis.

Todos os nomes de funções que implementam as operações do tipo *tad* serão prefixadas com *tad_*.
Com exceção de funções de construção (com nome *tad_cria\**), todas têm um primeiro argumento do tipo *Tad*, que é um ponteiro para o TAD que será operado pela função.

A implementação do TAD será feita em um arquivo chamado "*tad*.c". Esse arquivo deve incluir o arquivo "*tad*.h" no seu início.
Esse arquivo deve definir a *struct tad* e implementar todas as funções declaradas em *tad*.h.
Caso implemente alguma outra função (em geral alguma função auxiliar), essas devem ser declaradas como *static*, para que não seja exportada e com isso não seja usada em outro arquivo.

Em um .c que use o TAD, deve haver a inclusão do arquivo *tad*.h, e o cliente só tem o direito de usar os tipos e funções ali declarados para operar sobre um dado desse tipo.

Todos os TADs terão funções para a criação e destruição (construtores e destrutores) de uma variável do tipo especificado, com os nomes *tad_cria()* e *tad_destroi()*.
A primeira recebe argumentos necessários à criação de uma variável, aloca memória para essa variável, inicializa essa memória e retorna um ponteiro para a variável criada.
A segunda recebe um ponteiro para a variável a ser destruída, realiza alguma operação de finalização do dado se for necessário, libera a memória ocupada pelo dado.

## Exemplo de implementação de um TAD

Para exemplificar, vamos ver como poderia ser a implementação de um TAD para representar datas, de acordo com essas considerações.
O nome escolhido para o TAD é *data*, e o prefixo é *dt_*.
As operações a realizar sobre dados desse tipo (é só um exemplo, poderia ter sido escolhido outro conjunto de operações):
- criação de uma data, fornecendo dia, mês, ano
- destruição de uma data
- obtenção do dia da data
- obtenção do mês da data
- obtenção do ano da data
- cálculo do número de dias entre duas datas
- obtenção de uma data a tantos dias de distância da outra

O arquivo `data.h`:
```c
   #ifndef _DATA_H_    // para evitar a inclusão múltipla deste arquivo
   #define _DATA_H_

   // o tipo de dados Data
   typedef struct data *Data;

   // criação de uma data
   Data dt_cria(int dia, int mes, int ano);

   // destruição de uma data
   void dt_destroi(Data d);

   // obtenção do dia da data
   int dt_dia(Data d);

   // obtenção do mês da data
   int dt_mes(Data d);

   // obtenção do ano da data
   int dt_ano(Data d);

   // cálculo do número de dias entre duas datas
   int dt_dias_ate(Data d, Data d2);

   // imprime a data
   void dt_print(Data d);

   // obtenção de uma data a tantos dias de distância da outra
   Data dt_cria_apos_dias(Data d, int dias);

   #endif  // _DATA_H_
```
O arquivo `data.c`
```c
   #include "data.h"
   #include <stdbool.h>
   #include <stdlib.h>

   typedef struct data {
     int dia;
     int mes;
     int ano;
   };

   // função auxiliar que retorna true se for data válida
   static bool dt_ok(int d, int m, int a)
   {
     // testes para ver se d/m/a constituem uma data válida
     // ...
     return false; // ou nao
   }

   Data dt_cria(int dia, int mes, int ano)
   {
     if (!dt_ok(dia, mes, ano)) return NULL;
     Data d = malloc(sizeof(struct data));
     if (d != NULL) {
       d->dia = dia;
       d->mes = mes;
       d->ano = ano;
     }
     return d;
   }

   // destruição de uma data
   void dt_destroi(Data d)
   {
     free(d);
   }

   // obtenção do dia da data
   int dt_dia(Data d)
   {
     return d->dia;
   }

   // obtenção do mês da data
   int dt_mes(Data d)
   {
     return d->mes;
   }

   // obtenção do ano da data
   int dt_ano(Data d)
   {
     return d->ano;
   }

   // cálculo do número de dias entre duas datas
   int dt_dias_ate(Data d, Data d2)
   {
     if (d->mes == d2->mes && d->ano == d2->ano) {
       return d2->dia - d->dia;
     }
     // ...
   }

   // obtenção de uma data a tantos dias de distância da outra
   Data dt_cria_apos_dias(Data d, int dias)
   {
     return dt_cria(d->dia + dias, d->mes, d->ano); // talvez necessite refinamentos
   }
```
Um possível cliente:
```c
   #include "data.h"
   #include <stdio.h>

   int main()
   {
     Data hoje, amanha;

     hoje = dt_cria(31, 7, 2026);
     amanha = dt_cria_apos_dias(hoje, 1);
     printf("Amanhã será ");
     dt_print(amanha);
     printf(".\n");

     dt_destroi(hoje);
     dt_destroi(amanha);
   }
```

