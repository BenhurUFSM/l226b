/*
 * programa simples de teste do grafo.
 * cria um grafo com um exemplo visto em aula, calcula a ordem topológica.
 * imprime os nós do grafo, as arestas que partem de cada nó, as arestas que chegam
 *   em cada nó, a ordem topológica dos nós.
 * abaixo está um exemplo de saída do programa (a saída pode ser diferente, porque as
 *   arestas podem ser entregues em outra ordem, e existem várias ordens topológicas
 *   válidas para esse grafo).
7 vértices:
0: 'sapato'
1: 'meia'
2: 'calça'
3: 'cinto'
4: 'casaco'
5: 'cueca'
6: 'camisa'
arestas (partindo):
origem 0('sapato')
origem 1('meia')
  destino 0(sapato): 'meia->sapato'
origem 2('calça')
  destino 0(sapato): 'calça->sapato'
  destino 3(cinto): 'calça->cinto'
origem 3('cinto')
origem 4('casaco')
origem 5('cueca')
  destino 2(calça): 'cueca->calça'
origem 6('camisa')
  destino 3(cinto): 'camisa->cinto'
  destino 4(casaco): 'camisa->casaco'
arestas (chegando):
destino 0('sapato')
  origem 1(meia): 'meia->sapato'
  origem 2(calça): 'calça->sapato'
destino 1('meia')
destino 2('calça')
  origem 5(cueca): 'cueca->calça'
destino 3('cinto')
  origem 2(calça): 'calça->cinto'
  origem 6(camisa): 'camisa->cinto'
destino 4('casaco')
  origem 6(camisa): 'camisa->casaco'
destino 5('cueca')
destino 6('camisa')
Ordem topológica:
1º nó 1('meia')
2º nó 5('cueca')
3º nó 6('camisa')
4º nó 2('calça')
5º nó 4('casaco')
6º nó 0('sapato')
7º nó 3('cinto')
*/

#include "grafo.h"
#include "fila.h"
#include <stdio.h>
#include <string.h>

Grafo cria_grafo()
{
  Grafo g = g_cria(7, 15);
  char s[20];
  strcpy(s, "sapato");
  g_insere_nó(g, s);
  strcpy(s, "meia");
  g_insere_nó(g, s);
  strcpy(s, "calça");
  g_insere_nó(g, s);
  g_insere_nó(g, "cinto");
  g_insere_nó(g, "casaco");
  g_insere_nó(g, "cueca");
  g_insere_nó(g, "camisa");
  strcpy(s, "meia->sapato");
  g_altera_valor_da_aresta(g, 1, 0, s);
  strcpy(s, "calça->sapato");
  g_altera_valor_da_aresta(g, 2, 0, s);
  g_altera_valor_da_aresta(g, 5, 2, "cueca->calça");
  g_altera_valor_da_aresta(g, 2, 3, "calça->cinto");
  g_altera_valor_da_aresta(g, 6, 4, "camisa->casaco");
  g_altera_valor_da_aresta(g, 6, 3, "camisa->cinto");
  return g;
}

void imprime_grafo(Grafo g)
{
  printf("%d vértices:\n", g_num_nós(g));
  for (int n = 0; n < g_num_nós(g); n++) {
    char *val_nó = g_valor_do_nó(g, n);
    printf("%d: '%s'\n", n, val_nó);
  }
  printf("arestas (partindo):\n");
  for (int n = 0; n < g_num_nós(g); n++) {
    char *val_nó;
    int n2;
    char *val_nó2;
    char *val_aresta;
    val_nó = g_valor_do_nó(g, n);
    printf("origem %d('%s')\n", n, val_nó);
    g_arestas_que_partem(g, n);
    while ((val_aresta = g_próxima_aresta(g, &n2)) != NULL) {
      val_nó2 = g_valor_do_nó(g, n2);
      printf("  destino %d(%s): '%s'\n", n2, val_nó2, val_aresta);
    }
  }
  printf("arestas (chegando):\n");
  for (int n = 0; n < g_num_nós(g); n++) {
    char *val_nó;
    int n2;
    char *val_nó2;
    char *val_aresta;
    val_nó = g_valor_do_nó(g, n);
    printf("destino %d('%s')\n", n, val_nó);
    g_arestas_que_chegam(g, n);
    while ((val_aresta = g_próxima_aresta(g, &n2)) != NULL) {
      val_nó2 = g_valor_do_nó(g, n2);
      printf("  origem %d(%s): '%s'\n", n2, val_nó2, val_aresta);
    }
  }
}

void imprime_fila(Fila f, Grafo g)
{
  int nó;
  int ordem = 1;
  printf("Ordem topológica:\n");
  f_inicia_percurso(f, 0);
  while (f_próximo(f, &nó)) {
    char *val_nó;
    val_nó = g_valor_do_nó(g, nó);
    printf("%dº nó %d('%s')\n", ordem, nó, val_nó);
    ordem++;
  }
}

int main()
{
  Grafo g = cria_grafo();
  imprime_grafo(g);
  Fila f = g_ordem_topológica(g);
  imprime_fila(f, g);
  f_destrói(f);
  g_destrói(g);
  return 0;
}
