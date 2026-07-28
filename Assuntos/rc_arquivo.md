## Arquivos

Existem situações em que os dados devem sobreviver aos programas.
Para garantir a sobrevida dos dados, eles são armazenados em unidades de memória externa aos programas, em dispositivos de armazenamento, na forma de **arquivos**.
Um arquivo é uma região de um dispositivo de armazenamento, que é identificado por um nome, e contém uma quantidade qualquer de bytes (limitado, claro, à capacidade do dispositivo).
É responsabilidade do sistema operacional gerenciar os blocos dos dispositivos de armazenamento para implementar a abstração de arquivos.

Para que o conteúdo de um arquivo possa ser manipulado por um programa, ele deve ser copiado para a memória principal do computador, em uma operação de **leitura**.
Para que dados da memória principal sejam colocados em arquivos, eles devem ser transferidos para esses arquivos, em uma operação de **escrita**.

O acesso a um arquivo por um programa em C é feito por meio de uma variável que representa esse arquivo.
Essa variável deve ser inicializada com informações que permitem ao programa realizar o acesso desejado.
A criação e inicialização dessa variável, bem como todo tipo de acesso ao arquivo representado por ela é feita com o uso de funções.
As que veremos estão definidas com a inclusão de `<stdio.h>`.

A inicialização da variável que representa um arquivo é chamada de "**abertura**" do arquivo.
Para se poder realizar uma operação de leitura ou de escrita sobre um arquivo, ele inicialmente deve ser "aberto" -- o programa deve indicar o nome do arquivo que quer acessar e o tipo de operação que pretende realizar.
Após a realização das operações de leitura ou escrita sobre o arquivo, ele deve ser **fechado**, para que o sistema atualize corretamente as informações sobre o dispositivo de armazenamento.

Resumindo, são 4 as operações básicas sobre arquivos: abertura, leitura, escrita e fechamento. As operações de leitura, escrita e fechamento só podem ser realizadas sobre um arquivo que tenha sido previamente aberto e ainda não fechado.

Em um programa, um arquivo é representado por uma variável do tipo `FILE *`, um ponteiro para um registro do tipo `FILE`, cujo conteúdo não temos o direito de acessar (os campos desse registro não são padronizados, e cada biblioteca / sistema define como quiser).
Essa variável deve ser inicializada na abertura do arquivo e deve ser fornecida para as demais funções de manipulação do arquivo, para identificar sobre qual arquivo se quer realizar tal operação.
As principais funções são:
- `fopen` - serve para abrir um arquivo. Recebe dois argumentos: o *nome do arquivo* (uma string) e o *modo de abertura* (outra string). Dois dos modos de abertura possíveis nos interessam por enquanto: `"r"`, para abrir o arquivo para leitura e `"w"`, para abri-lo para gravação.

   Só se pode abrir para leitura um arquivo que já exista e sobre o qual o programa tenha direito de leitura.

   Só se pode abrir para gravação um arquivo que já existe sobre o qual o programa tenha direito de gravação ou um arquivo que não exista, que será criado pela operação de abertura, se o programa tiver o direito de criar arquivos no local pedido.
   Quando o arquivo é aberto para gravação, qualquer conteúdo que o mesmo tenha é descartado.

   A função `fopen` returna um `FILE *` que identifica o arquivo aberto.
   Caso a operação de abertura não tenha sido possível, o valor `NULL` é retornado, e não se pode realizar nenhuma outra operação sobre o arquivo.
- `fclose` - serve para informar o sistema que não vamos mais realizar operações sobre esse arquivo. Recebe como argumento um `FILE *`, que deve corresponder a um arquivo aberto.
- `fscanf` - serve para realizar leitura de dados sobre um arquivo aberto para leitura.
   Tem funcionamento similar à função `scanf`, exceto que em vez de ler os dados do teclado, lê do arquivo.
   Os argumentos da função são os mesmos de `scanf`, acrescidos de um primeiro argumento, um `FILE *` que identifica um arquivo que foi previamente aberto para leitura, do qual serão lidos os dados necessários para o scanf.

   Da mesma forma que `scanf`, `fscanf` retorna o número de conversões bem sucedidas que conseguiu realizar, ou um número negativo em caso de erro.
- `fprintf` - serve para gravar dados em um arquivo.
   Funciona como o `printf`, e tem os mesmos argumentos, acrescidos de um argumento inicial, um `FILE *` que corresponde a um arquivo que tenha sido aberto para escrita, no qual serão escritos os dados formatados por `fprintf`.
- `feof` - serve para saber se já foi realizada uma operação de leitura que tenha tentado ler além do final dos dados do arquivo.
   Recebe como argumento um `FILE *` que corresponde a um arquivo aberto para leitura e retorna um valor que pode ser testado como verdadeiro ou falso. Ele retorna verdadeiro somente após se realizar uma operação mal sucedida (por ir além do final do arquivo) de leitura -- retorna falso mesmo que se tenha lido o último byte do arquivo, se não se tentou ler além dele.

Exemplo de um programa que lê números de um arquivo e informa a soma dos números lidos:

```c
#include <stdio.h>

int main()
{
  FILE *arq;
  char nome[] = "dados";

  // abre o arquivo para leitura
  arq = fopen(nome, "r");
  // verifica se a abertura do arquivo foi bem sucedida
  if (arq == NULL) {
    printf("Não foi possível abrir o arquivo '%s' para leitura\n", nome);
    return 1;
  }

  long soma = 0;
  int nnum = 0;
  for (;;) {
    int val;
    // lê um inteiro do arquivo e abandona o laço se a leitura não foi bem sucedida
    if (fscanf(arq, "%d", &val) != 1) {
      break;
    }
    // acumula o valor lido à soma e incrementa o contador de números lidos
    soma += val;
    nnum++;
  }
  // fecha o arquivo
  fclose(arq);

  printf("Foram lidos %d números do arquivo, que somam %ld\n", nnum, soma);
}
```

Outro exemplo, um programa que cria um arquivo com dados interessantes para o programa anterior:
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  // inicializa o gerador de números aleatórios
  srand(time(0));

  // variável que referenciará o descritor do arquivo
  FILE *arq;

  // abre o arquivo para escrita
  arq = fopen("dados", "w");
  // verifica se a abertura foi bem sucedida
  if (arq == NULL) {
    printf("Não foi possível abrir o arquivo 'dados' para escrita.\n");
    return 1;
  }

  for (int i = 0; i < 100; i++) {
    // escreve uma linha no arquivo com um valor inteiro aleatório
    fprintf(arq, "%d\n", rand());
  }

  // fecha o arquivo
  fclose(arq);
}
```

Outro exemplo, um programa que copia um arquivo para outro, retirando espaços duplicados:
```c
#include <stdio.h>
#include <stdbool.h>

int main()
{
  char nome_entrada[30], nome_saida[30];
  FILE *entrada;
  FILE *saida;

  printf("Programa de cópia de arquivos.\n");
  printf("Digite o nome do arquivo de entrada: ");
  scanf("%s", nome_entrada);
  entrada = fopen(nome_entrada, "r");
  if (entrada == NULL) {
    printf("Não foi possível abrir o arquivo '%s'.\n", nome_entrada);
    return 1;
  }

  printf("Digite o nome do arquivo de saída: ");
  scanf("%s", nome_saida);
  saida = fopen(nome_saida, "w");
  if (saida == NULL) {
    printf("Nao foi possível abrir o arquivo '%s'.\n", nome_saida);
    return 2;
  }

  int bytes_lidos = 0, bytes_escritos = 0;
  bool anterior_foi_espaco = false; // true se o caractere anterior foi um espaço
  for (;;) {
    char c;
    fscanf(entrada, "%c", &c);
    if (feof(entrada)) { // poderia também testar o retorno do fscanf
      break;
    }
    bytes_lidos++;
    if (c != ' ' || !anterior_foi_espaco) {
      fprintf(saida, "%c", c);
      bytes_escritos++;
    }
    anterior_foi_espaco = (c == ' ');
  }

  fclose(entrada);
  fclose(saida);
  printf("Foram lidos %d bytes e escritos %d.\n", bytes_lidos, bytes_escritos);
}
```
Mais um exemplo, que mostra na tela o conteúdo de um arquivo
```c
#include <stdio.h>

int main(int argc, char *argv[argc])
{
  if (argc != 2) {
    fprintf(stderr, "Uso: %s nome_do_arquivo", argv[0]);
    return -1;
  }
  FILE *descritor = fopen(argv[1], "r");
  if (descritor != NULL) {
    fprintf(stderr, "Erro no acesso ao arquivo '%s'\n", argv[1]);
    return -2;
  }
  // lê o arquivo byte a byte -- seria melhor ler mais por vez (linha, por ex.)
  for (;;) {
    int c = fgetc(descritor); // fgetc é equivalente ao getchar
    if (feof(descritor)) break;
    putchar(c);
  }
  fclose(c);
}
```

##### Algumas outras funções de acesso a arquivos

- `ftell()` - recebe como argumento um arquivo (um `FILE *` que corresponde a um arquivo aberto) e retorna um `long` em que posição do arquivo se está. Essa posição é o número de bytes que tem no arquivo antes da posição atual. A posição atual é a posição onde será gravado o primeiro byte de uma operação de escrita ou lido o primeiro byte em uma operação de leitura.
- `fseek` - muda a posição do próximo acesso ao arquivo. Recebe 3 argumentos: um arquivo, um `long` que identifica a posição e um valor que diz de onde essa posição será contada. Se o terceiro valor for `SEEK_SET`, a posição conta à partir do início do arquivo; se for `SEEK_CUR`, conta à partir da posição corrente; se for `SEEK_END`, conta à partir do final do arquivo.
- `fgetc` - lê e retorna um `int` com o próximo byte do arquivo, ou um valor negativo caso a leitura não seja possível. Tem o arquivo como argumento. É equivalente ao `getchar`.
- `fputc` - escreve um byte no arquivo. Tem como argumentos o valor do byte e o arquivo. É como o `putchar`.
- `fgets` - lê uma linha do arquivo (ou menos). Tem como argumentos um vetor de `char`, o número máximo de bytes que podem ser escritos nesse vetor (tipicamente o tamanho do vetor) e o arquivo. Lê no máximo um byte a menos do que cabe no vetor, e coloca um `\0` no vetor, após o último byte lido. Para de ler se terminar o arquivo ou se ler um `\n` ou se ler o número máximo de bytes. O `\n`, se lido, é colocado no vetor antes do `\0`.
