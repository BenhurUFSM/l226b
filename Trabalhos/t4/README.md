## Trabalho 4

Uma rede neural para jogar o jogo da cobrinha.

Você deve implementar algumas estruturas de dados necessárias para o treinamento e execução de uma rede neural:
- uma fila que suporta dados genéricos;
- um grafo que suporta dados genéricos tanto nos vértices quanto nas arestas;
- alguns algoritmos nesses grafos.

### Parte I — o jogo da cobrinha (implementação de fila genérica)

Os arquivos fornecidos implementam o jogo da cobrinha, onde o jogador consegue controlar a direção de uma cobra, tentando alimentá-la e não deixando que ela bata em obstáculos.
Os arquivos são:
- jogo.c e jogo.h: implementa a principal do funcionamento do jogo;
- terminal.c e terminal.h: implementa acesso básico ao terminal, tanto para entrada pelo teclado quando saída no vídeo;
- main.c: implementa uma partida do jogo, controlada pelo teclado e mostrando no vídeo;
- fila.h: interface do TAD fila, usado pelo jogo.

Você deve implementar o TAD fila em um arquivo chamado fila.c, de acordo com fila.h.
O ponto chave da fila é que ela não sabe o tipo de dados que devem ser enfileirados, somente o número de bytes que cada um ocupa.
Nas operações de inserção e remoção, a fila recebe um ponteiro, onde o dado está ou onde ele deve ser colocado.
A fila deve copiar o dado, e não simplesmente guardar o valor do ponteiro.
O código abaixo deve imprimir "teste 1 2 3 4 fim":
```c
  void teste()
  {
      Fila f1 = f_cria(sizeof(int));
      Fila f2 = f_cria(10);
      char s[10];
      for (int i = 1; i <= 4; i++) {
          f_insere(f1, &i);
      }
      strcpy(s, "teste");
      f_insere(f2, s);
      strcpy(s, "fim\n");
      f_insere(f2, s);
      f_remove(f2, s);
      printf("%s ", s);
      while (!f_tá_vazia(f1)) {
          int a;
          f_remove(f1, &a);
          printf("%d ", a);
      }
      f_remove(f2, s);
      printf("%s", s);
      f_destrói(f2);
      f_destrói(f1);
  }
```

Com a fila implementada, deve ser possível compilar um programa para jogar o jogo da cobrinha com:
```sh
  gcc -o cobra jogo.c fila.c terminal.c main.c
```

### Dicas

- não precisa esperar a parte II para implementar a parte I.
- use `memcpy` para copiar dado de/para o usuário.
- pode implementar a fila como lista encadeada ou como vetor, mas tem que ter alocação dinâmica, não pode ter um número máximo predefinido de elementos.

### Parte II — a cobrinha joga sozinha (implementação de grafo)

Com essa parte, o jogo pode ser comandado por uma rede neural.
No laço principal do jogo, além da possibilidade de se ler o teclado e fornecer informações de entrada para o comando da cobrinha, foi acrescentado o comando gerado por uma rede neural.
Para isso, foi incluída uma função que calcula 7 valores para descrever o estado do joga para a rede: dois que dizem a que distância a fruta está, dois que dizem a distância do obstáculo mais próximo e três que dizem se tem um obstáculo encostado na cobrinha, em uma das direções que ela pode continuar.

A rede neural é implementada com um grafo, cuja interface está em `grafo.h`, e deve ser implementada.
A implementação da rede está em `rede.h` e `rede.c`.

Para usar uma rede, o programa deve ser executado com `./cobra arquivo_com_a_rede`. Dois exemplos de arquivos contendo uma rede são fornecidos.

Os arquivos `main.c` e `jogo.c` foram alterados para incorporar as novas funcionalidades.

A função `g_ordem_topológica` pode ser implementada colocando os números dos nós em uma fila, em ordem crescente, que será uma ordem correta para os arquivos de rede fornecidos. A função `g_tem_ciclo` pode ser implementada retornanto `false`. Posteriormente (parte III), será necessário que essas funções sejam corretamente implementadas. 

