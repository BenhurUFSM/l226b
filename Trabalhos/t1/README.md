# Trabalho 1 — Invasores do Espaço

Trabalho inicial de lab2, para revisão da linguagem C.

Você deve implementar um programa que permite jogar "Invasores do Espaço", como descrito abaixo.


## Descrição do jogo

O planeta está sendo invadido por seres extra-terrestres, que estão dizimando os humanos, por motivos ainda não completamente desvendados.
Você é responsável pela defesa do último bastião (pelo menos nas redondezas), e os inimigos são incansáveis.
Os inimigos têm vários tipos de ataques, e os humanos já conseguiram desenvolver armas capazes de destruir todos esses tipos.
Infelizmente, não se conseguiu integrar os vários tipos de defesas, o que torna o trabalho árduo, porque envolve identificar o tipo de ataque que está sendo usado, selecionar a arma adequada e só então realizar a destruição do atacante.
Um ataque inimigo só é bem sucedido caso consiga encostar no posto do jogador.
Além das armas, você tem uma defesa estática, que são escudos capazes de aguentar um ataque inimigo.
Caso um ataque inimigo encoste em um escudo, tanto o ataque quanto o escudo são destruídos.

O inimigo ataca em ondas, com uma onda contendo certo número de ataques.
Os ataques são desferidos um após outro, em intervalo constante, mas o tipo de ataque é imprevisível.
Entre uma onda e outra, você consegue recarregar sua arma, que fica com capacidade para certo número de tiros.
Durante uma onda de ataque, o recarregamento não é possível.
O inimigo se aperfeiçoa entre uma e outra onda, conseguindo aumentar a frequência dos ataques.

Inicialmente, o inimigo só atacava durante o dia, mas houve uma evolução recente que permite que ele realize também ataques noturnos.
Nos ataques noturnos, parte dos tipos de ataques não ficam disponíveis, e os ataques são mais lentos que os diurnos.
Nos ataques noturnos, você não vê os tipos de ataques que estão sendo usados pelo inimigo, o que torma mais difícil a escolha da arma certa a utilizar.
Você é obrigado a usar a audição nesses ataques, porque cada tipo de ataque inimigo tem um som característico.
Você também tem à sua disposição um sonar, que identifica todos os ataques ativos (de forma sonora).

Os inimigos vão acabar vencendo, o seu objetivo é atrasá-los o máximo, esperando que novas armas sejam desenvolvidas (e acumular pontos enquanto isso).

## Visual do jogo

Em um ataque noturno, o visual do jogo é extremamente minimalista, porque não se vê nada. É apresentada uma só informação, o número de pontos em uma linha de texto que vai sendo reescrita enquanto o jogo se desenvolve.

Quando o ataque é diurno, o estado do jogo também é mostrado em uma linha de texto que é constantemente reescrita, composta por:
- o número de pontos obtidos até agora
- o número de tiros restantes
- o tipo de armamento selecionado
- os escudos (ocupando 3 posições)
- os ataques inimigos (ocupando 10 posições)

### Exemplo 1

Abaixo está uma possível linha apresentada pelo jogo:
```
 14 10 5)))      1071
```
Ela informa que você tem 14 pontos, restam 10 tiros, a arma selecionada pode destruir ataques inimigos do tipo `5`, existem 3 escudos, existem 4 ataques inimigos ativos (de tipo `1`, `0`, `7` e `1`).

### Exemplo 2
```
 15 5 n))  2 N  1   
```
Você tem 15 pontos, restam 5 tiros, a arma selecionada pode destruir ataques inimigos do tipo `N` (ou `n`), restam 2 escudos, existem 3 ataques inimigos ativos (de tipo `2`, `N` e `1`).

## Dinâmica do jogo

O jogo se desenvolve em ondas de ataque.
No início de uma onda, o número de tiros do jogador é recarregado, bem como o número de ataques inativos do inimigo.

Dois eventos podem alterar o estado do jogo durante uma onda:
- a passagem do tempo
- a execução de um comando pelo jogador.

Os eventos temporais têm uma frequência constante, que depende da fase do jogo (o número da onda de ataques atual).
A cada intervalo:
- se houver algum ataque inimigo ativo, ele é movido para a esquerda, e
  - caso um ataque colida com um escudo, tanto o escudo quando o ataque são destruídos
  - caso um ataque colida com a base (a arma do jogador), o jogo termina
- se ainda houver algum ataque não ativo na onda atual, é colocado na última posição dos ataques (mais à direita)
- se não houver mais ataque, ativo ou não, na onda atual, a onda termina

O jogador executa comandos pressionando teclas. As teclas válidas são:
- `Esc` - termina o jogo
- `Tab` - altera a arma selecionada para a próxima arma disponível. No caso de ataque diurno, a sequência de armas é `0123456789n`; no caso de ataque noturno, é `02468n`.
- `Enter` - atira. Caso haja tiro disponível, o número de tiros é decrementado, e, caso houver um inimigo ativo de mesmo tipo da arma selecionada, o inimigo desse tipo mais à esquerda é destruído e o jogador ganha pontos. Caso o tipo de arma seja `n` e haja um inimigo do tipo `N`, o inimigo do tipo `N` mais à esquerda tem seu tipo alterado para `n` e não é destruído (para destruir um ataque tipo `N` são necessários dois tiros). Caso um ataque seja destruído, os pontos do jogador são alterados.
- `Espaço` - causa uma atuação do sonar. É emitida uma sequência de sons, correspondentes a cada escudo, a cada espaço e a cada ataque ativo, na ordem em que se encontram.

No final de uma onda (quando não houver mais ataques inativos e os ativos tiverem sido destruídos), os pontos são atualizados, e é apresentado um resumo do estado do jogo.
Aguarda-se que o usuário digite `r`, e o número de tiros, ataques inimigos são reinicializados, o intervalo dos eventos temporais é atualizado, sorteia-se o tipo da nova onda (se diurna ou noturna) e a nova onda é iniciada.

A partida termina quando um ataque for bem sucedido (chegar até a base do jogador), ou quando o jogador desistir teclando `Esc`.
No final da partida, é apresentado um resumo da pontuação, se ela está entre as 3 maiores, e pergunta-se se se quer jogar novamente (e inicia-se um novo jogo caso a resposta seja positiva).

### Exemplo 1

No primeiro exemplo de tela acima, caso restem 2 ataques inativos de tipos `9` e `1`, a sequência de telas a cada intervalo de tempo, caso o jogador não faça nada será como abaixo(cada linha abaixo representa uma alteração da tela; no programa, elas seriam escritas uma sobre a outra). O texto à direita diz o que aconteceu logo antes dessa linha ser desenhada.
```
 14 10 5)))      1071
 14 10 5)))     10719       moveu para a esquerda e apareceu o 9
 14 10 5)))    107191       moveu para a esquerda e apareceu o 1
 14 10 5)))   107191        moveu para a esquerda e não tinha mais inativos
 14 10 5)))  107191
 14 10 5))) 107191
 14 10 5)))107191
 14 10 5)) 07191            o 1 destruiu o 3º escudo e se destruiu
 14 10 5))07191
 14 10 5) 7191              o 0 destruiu o 2º escudo
 14 10 5)7191
 14 10 5 191                o 7 destruiu o 1º escudo
 14 10 5191
 14 10  91                  o 1 destruiu a base — invasão bem sucedida — fim da partida
```

### Exemplo 2

No segundo exemplo acima, a sequência de telas poderia ser (o texto à direita diz o que aconteceu depois da linha ser desenhada):
```
 15 5 n))  2 N  1           o jogador aperta enter
 15 4 n))  2 n  1           o tempo passa
 15 4 n)) 2 n  1            o jogador aperta enter (matou 'n', 2*8 pontos)
 31 3 n)) 2    1            o tempo passa
 31 3 n))2    1             o jogador aperta tab
 31 3 0))2    1             o jogador aperta tab
 31 3 1))2    1             o tempo passa
 31 3 1)     1              o jogador aperta enter (matou '1', 7 pontos)
 38 2 1)                    fim da onda/fase (2*2 pontos pelos tiros, 10 pelo escudo)
 52 2 1)
```

## Pontuação

Destruir um ataque inimigo com um tiro vale um ponto se o inimigo estiver na posição mais à direita (o local de nascimento dos ataques), e um ponto a mais para cada posição que o ataque tenha sido deslocado. O inimigo do tipo `n` vale o dobro.
No final de uma onda, cada tiro não disparado vale dois pontos e cada escudo não destruído vale 10.

No ataque noturno, os pontos são dobrados.

## Sons

Deve existir um som diferente para cada tipo ataque, e o do ataque 'N' ou 'n' deve ser mais distinto.
Deve existir ainda um som para o escudo, um som para o espaço (local onde pode ter um ataque mas não tem) e para um tiro errado (que pode ser o mesmo do espaço). 13 ou 14 sons no total.

No momento da ativação de um ataque, o som correspondente ao ataque é tocado.
No momento da troca de arma, o som correspondente à nova arma é tocado.
No momento do tiro, é tocado o som correspondente à arma em caso de acerto ou o som de tiro errado caso o tiro tenha sido desperdiçado.

Deve também ser tocado algum som que identifique o final de uma onda e da partida.

## Valores diferentes entre ataques diurnos e noturnos

### Para ataques diurnos

- tempo entre movimentações dos aliens - 2s na primeira onda, diminui 10% a cada onda
- número de tiros disponíveis para o jogador - 30
- número de ataques do inimigo - 20
- 11 tipos de ataques (e armas): `0` a `9` mais `N` ou `n`
- 13 posições, 3 com escudos e mais 10. Os ataques aparecem na 13ª posição.

### Para ataques noturnos

- tempo de movimentação é o triplo do diurno
- número de tiros disponíveis para o jogador - 30
- número de ataques do inimigo - 15
- 6 tipos de ataques (e armas): `0`, `2`, `4`, `6`, `8` mais `N` ou `n`
- 8 posições, 3 com escudos e mais 5. Os ataques aparecem na 8ª posição.

### Como decidir se um ataque é noturno ou diurno

O primeira onda tem 100% de chance de ser diurna. Essa chance diminui para 80% na segunda onda, 60% na terceira, 40% na quarta e 20% nas demais.

## Implementação

O programa deve ser implementado em C, pelo aluno, individualmente.
Pode pedir ajuda, mas não implementação.

Caso não tenha entendido algo, pergunte antes de implementar.
As respostas a essas perguntas serão colocadas no final deste texto.

O programa deve manter um arquivo contendo as três pontuações mais altas já vistas pelo programa.
O programa não deve faniquitar se esse arquivo não existir ou se não puder ser criado ou alterado.

O programa deve ser bem dividido em funções.
Cada função deve realizar uma tarefa bem definida, podendo usar outras para alguma subtarefa.
A tarefa deve estar descrita em um comentário logo antes da função.

### Restrições de formatação

- uma função não deve ter mais de 25 linhas.
- uma linha não deve ter mais de 75 colunas.
- uma linha não deve ter mais de um `;` (os `;` do `for` ou em uma string não contam).
- comandos compostos com mais de uma linha devem usar chaves.
- uma linha não deve ter um `{` sozinho, e deve ter um espaço antes dele. Essa restrição não é necessária para o primeiro `{` de uma função.
- uma linha não deve ter um `}` acompanhado, a não ser que seja `} else {`.
- o `(` é precedido de espaço se for de um comando (como `if ()`), mas não se for de uma função (como `printf()`).
- operadores binários (e ternário) são circundados por espaço (`a + j` e não `a+j` ou `a +j`).
- não tem espaço entre operador unário e seu operando (`a++ * -b` e não `a ++*- b`).
- use indentação (escolha 2 ou 4 espaços).

A E/S no terminal deve ser feita considerando o que está nas seções abaixo.

### Leitura do teclado

Para poder ler as teclas individualmente, coloque o terminal em modo cru no início da execução do programa, executando o programa `stty`, com o comando:
```c
   system("stty raw -echo min 0 time 1 opost");
```
Nesse comando: 
- `system` é uma função (incluir `stdlib.h`) que pede para o sistema executar um shell para executar o comando contido na string.
- `stty` é o programa a ser executado. É um programa que serve para configurar o terminal. Deve estar instalado no sistema.
- `raw` argumento para o stty. Pede para colocar o terminal em modo "cru" (os caracteres são entregues ao programa conforme são digitados)
- `-echo` argumento para o stty. Pede para não ecoar (imprimir na tela) o que for digitado
- `min 0` argumento para o stty. Permite que sejam lidos 0 caracteres (senão um pedido de leitura fica esperando até que seja digitado algo)
- `time 1` argumento para o stty. Diz para esperar até um décimo de segundo para retornar caso nada seja digitado
- `opost` argumento para o stty. Diz para realizar o processamento dos caracteres na saída (sem isso, o '\n' não retorna ao início da linha).

No final do programa, devolva o terminal para o modo normal com:
```c
   system("stty sane");
```
Para poder usar a função `system`, inclua `stdlib.h` no seu programa.

A configuração acima altera o funcionamento do terminal no nível do sistema operacional. É necessário alterar também o funcionamento da biblioteca `stdio`, para que ela não acumule os caracteres lidos em um *buffer* interno antes de entregá-los para o programa. A função `setvbuf` serve para fazer isso:
```c
    setvbuf(stdin, NULL, _IONBF, 0);
```

Com o terminal em modo cru, várias funções de entrada não operam normalmente, como `getchar` e `scanf`. Para a leitura de um caractere da entrada, dá para usar a função `fread`, de mais baixo nível. Use a função abaixo, que usa `fread` para ler um caractere. Ela retorna o código do caractere lido ou 0 caso nada tenha sido digitado. Ela também chama `fflush`, para garantir que todos os caracteres escritos anteriormente sejam efetivamente enviados ao SO.
```c
char lechar()
{
    fflush(stdout);
    char c;
    if (fread(&c, 1, 1, stdin) == 1) return c;
    return 0;
}
```

Teste com o programa abaixo, que deve ficar imprimindo 10 linhas por segundo com `0`, ou o código do caractere digitado, se for digitado algo. Termine o programa digitando `q`.
```c
// programa para testar o terminal em modo "cru"

#include <stdio.h>
#include <stdlib.h>

// configura o terminal para o modo "cru", para permitir a leitura
//   de cada caractere digitado sem esperar pelo "enter".
void configura_terminal()
{
    if (system("stty raw opost -echo min 0 time 1") != 0) {
        perror("erro na execução de system(\"stty\")");
        fprintf(stderr, "você tem o programa stty instalado?\n");
        exit(1);
    };
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        perror("erro na execução de setvbuf()");
        exit(1);
    }
}

// configura o terminal para o modo normal, com bufferização por linha.
void normaliza_terminal()
{
    system("stty sane");
}

// lê um caractere do teclado.
// retorna o código do caractere lido ou 0 casa nada tenha sido digitado.
// só funciona corretamente se o terminal estiver em modo "cru".
char lechar()
{
    fflush(stdout);
    char c;
    if (fread(&c, 1, 1, stdin) == 1) return c;
    return 0;
}

int main()
{
    configura_terminal();
    for (;;) {
        int c = lechar();
        printf("%d\n", c);
        if (c == 'q') break;
    }
    normaliza_terminal();
}
```

### Escrita na tela

A apresentação do programa acontece em uma linha única, que é constantemente redesenhada.
Para isso, após desenhar a linha, imprima o caractere `'\r'` (retorno de carro), que retorna o cursor para a primeira coluna da linha.
Quando a próxima linha for escrita, será escrita sobre a anterior.

Troque a `main` do programa acima pelo código abaixo para um exemplo:
```c
int main()
{
    configura_terminal();
    char u = 0;
    int n = 0;
    do {
        char c = lechar();
        if (c != 0) {
            u = c;
            n = 0;
        }
        n++;
        printf("leituras: %d  ult ch: %d   \r", n, c);
    } while (u != 'q');
    normaliza_terminal();
}
```

### Estruturação do programa

As variáveis que definem o estado do programa (número de pontos, fase do jogo, arma atual, posição e tipo dos ataques, etc) devem estar em um registro (`struct`), declarado na `main` e passado por referência pelas demais funções que necessitem acesso a esse estado.
Para que essas funções possam receber um ponteiro para esse registro, o tipo do registro deve ser conhecido por elas. Para isso, declare esse tipo com um `typedef` no início do programa.

O programa deve ter um laço principal para realizar as operações globais relacionadas a cada partida (chama a função que implementa uma partida, vê se precisa repetir e executar outra, etc).

O laço de execução de uma partida deve estar em outra função.
Essa função inicializa cada onda da partida, e chama a função que executa uma onda, até que a partida termine.

A função que executa uma onda tem um laço que chama funções para desenhar a tela com o estado do jogo, para processar as entradas (teclado e tempo), e repete enquando a onda não terminar.

### Temporização

Para o controle do tempo, use a função `clock_gettime`.
Ela preenche um registro do tipo `struct timespec` com o tempo transcorrido desde algum evento no passado.
Realizando duas chamadas a essa função dá para calcular quanto tempo transcorreu entre essas duas chamadas.
Com isso dá para saber se está na hora de movimentar os aliens ou não.
Inclua `time.h`.

O exemplo abaixo implementa um cronômetro com o uso dessa função, e usa um para calcular o tempo de execução da função `f`:
```c
#include <stdio.h>
#include <time.h>

// implementação de um cronômetro
typedef struct timespec crono;

// inicializa um cronômetro com a hora atual
void crono_inicia(crono *c)
{
    clock_gettime(CLOCK_MONOTONIC, c);
}

// retorna o tempo passado desde que o cronômetro *c foi iniciado, em segundos
double crono_parcial(crono *c)
{
    crono agora;
    clock_gettime(CLOCK_MONOTONIC, &agora);

    double segundos = agora.tv_sec - c->tv_sec;
    double nanosegundos = agora.tv_nsec - c->tv_nsec;
    return segundos + 1e-9 * nanosegundos;
}

int f(int x)
{
    if (x <= 0) return 0;
    return f(x - 1);
}

int main()
{
    crono c1;
    crono_inicia(&c);
    f(1000);
    double t1 = crono_parcial(&c);
    f(2000);
    double t2 = crono_parcial(&c);
    printf("f(1000) demorou %f segundos.\n", t1);
    printf("f(2000) demorou %f segundos.\n", t2 - t1);
}
```

## Som

O diretório "Sons" contém uma coleção de sons curtos, em formato `wav`.
Cada som tem 3 versões, com duração de 2, 3 ou 4 décimos de segundo.
3 décimos de segundo parecem ser uma bom equilíbrio entre não demorar demais e ser distinguível.
Os sons `0` a `9` são para ser os sons dos ataques.
O som `x` é para ser o som do espaço.
O som `11` é para ser o som da nave (`N` ou `n`).
O som `12` é para ser o som do escudo.
Dá para tocar alguns em sequência para marcar o final de uma onda.
Caso queira, pode usar outros sons.

Para tocar os sons, pode ser usado um programa externo.
Eu usei o programa `aplay`. Por exemplo, para tocar o som `x` seguido do som `2`:
```c
   system("aplay -q x.3.wav 2.3.wav");
```
A opção `-q` do `aplay` serve para que ele não escreva nada na tela.
A função `system` aguarda até que o programa `aplay` termine, o que faz com que o programa fique parado enquanto o som toca. Acrescentando um `&` no final do comando, ele retorna imediatamente, e o programa pode continuar executando enquanto o som toca:
```c
   system("aplay -q x.3.wav 2.3.wav &");
```

## Opcionais

- Se você não vê, a implementação do modo diurno é opcional.
- Se você não ouve, a implementação de som é opcional.
- Se você vê mas tem uma enorme falta de tempo ou de coragem, a implementação do modo noturno é opcional.
- Se você tem alguma outra restrição, fale com o professor.

## Dicas + Esclarecimentos

Se surgirem perguntas, aparecerão respostas, dicas ou esclarecimentos aqui.
