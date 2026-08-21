## Listas

### Implementação de listas

Uma lista pode ser implementada de diversas formas. É comum classificar essas formas de acordo como a memória é alocada e como a ordem dos dados é implementada. A alocação pode ser estática ou dinâmica, a ordem dos dados pode ser mantida por contiguidade ou por encadeamento.

#### Alocação contígua

Os dados são mantidos em uma região contígua de memória, em um vetor ou em uma região alocada dinamicamente (que pode ser tratada como um vetor).
O primeiro elemento da lista é colocado na primeira posição desse vetor, o segundo na segunda posição etc.
O número de elementos que se pode colocar na lista corresponde ao tamanho do vetor.
Para se saber quantos elementos existem na lista (que geralmente é inferior ao tamanho do vetor), em geral se usa ou um elemento demarcador de fim (como em uma string em C) ou uma variável auxiliar que contém o número de elementos (dentro de uma struct que descreve a lista).
Na maioria dos casos se usa a segunda opção, por ser mais econômica (é necessário um inteiro e não o espaço de um item) e mais rápida (a operação de se obter o número de elementos na lista é mais simples).

A implementação das operações básicas:
- o número de elementos é obtido diretamente, já que tem uma variável com essa informação
- a obtenção do dado em uma posição da lista é realizada com a operação a acesso a elemento do vetor nessa mesma posição
- a remoção de um dado em determinada posição implica no deslocamento físico de todos os dados da lista, desde a posição seguinte até o final da lista; a remoção do último elemento não necessita movimentação alguma, mas a do primeiro necessita movimentação de todos os dados restantes na lista
- a inserção de um elemento em uma posição da lista necessita da movimentação de todos os dados da lista a partir dessa posição até o final, para gerar espaço para o elemento inserido; a inserção no final da lista não necessita de movimentação, mas no início implica na movimentação da lista inteira

Esse tipo de lista pode também ser implementado com alocação dinâmica, sem se limitar o número máximo de itens que a lista pode conter.
Nesse caso, em uma inserção em uma lista que já ocupa toda a memória disponível, aloca-se uma região maior, copia-se o conteúdo da lista para a nova região e libera-se a região anterior. Deve ser mantida uma nova variável com a capacidade atual da memória alocada para a lista.
No caso de ser ter uma lista em que o número de elementos pode diminuir bastante depois de ter sido grande, para não se manter o consumo de memória muito alto, a operação de remoção pode também realocar a memória para uma capacidade inferior quando a capacidade e a utilização forem muito diferentes.

Uma pilha pode ser implementada dessa forma a um custo baixo, porque as operações principais, de remoção e inserção podem ser realizadas sem movimentação de dados, se não houver necessidade de realocar o vetor.

Já a implementação de uma fila terá uma das operações principais (inserção ou remoção) com baixo custo, mas a outra necessitará a movimentação de todos os dados presentes na fila.

##### Implementação circular

Uma outra forma semelhante de se implementar uma lista contígua é em uma implementação **circular**, onde o primeiro elemento da lista não necessariamente é colocado na primeira posição do vetor (mas os elementos continuam contíguos em memória, o segundo elemento está logo depois do primeiro etc).
Uma nova variável é necessária, para informar em que posição do vetor está o primeiro elemento da lista.
Uma remoção do primeiro elemento da lista implica somente na alteração da variável que diz onde está o primeiro elemento, não necessita deslocar fisicamente os elementos dentro do vetor (claro, tem que alterar também a informação sobre o número de elementos, como antes).
O espaço não usado no início do vetor é usado quando a posição final do vetor já está usada e há uma nova inserção -- o elemento após o que está na última posição do vetor é colocado na primeira posição, e é essa característica que dá o nome de circular.
Por exemplo, as variáveis representadas abaixo armazenam uma lista com os valores `3 8 9`, os valores 1 e 2 no vetor não estão na lista. `cap` é a capacidade (quantos elementos cabem no vetor), `pri` é o índice no vetor onde está o primeiro elemento da lista, `num` é o número de elementos na lista, `dad` é o vetor de dados.
```
   cap 5
   pri 3
   num 3
   dad 9 1 2 3 8
```
A inserção de um valor no início dessa lista pode ser colocado sobre o valor `2`, sem necessitar deslocar nenhum dado; a inserção no final da lista pode ser colocado sobre o `1`, sem necessitar deslocar. 
A inserção do valor 4 no início da lista acima (que passa a conter os valores `4 3 8 9`) resultaria em:
```
   cap 5
   pri 2
   num 4
   dad 9 1 4 3 8
```
A inserção do valor 0 no final da lista original (que passa a conter os valores `3 8 9 0`) resultaria em:
```
   cap 5
   pri 3
   num 4
   dad 9 0 2 3 8
```
A remoção do valor inicial dessa última lista (que passa a conter os valores `8 9 0`) resultaria em:
```
   cap 5
   pri 4
   num 3
   dad 9 0 2 3 8
```
A posição `pv` no vetor onde está o item na posição `pl` da lista pode ser obtida por `pv = (pri + pl) % cap`.

As 4 operações de inserção e remoção nas extremidades da lista são implementadas de forma simples, e seu tempo de execução independe do número de elementos contidos na lista.
A inserção em alguma outra posição irá implicar em deslocamento de dados na memória. Pode-se diminuir um pouco esse deslocamento escolhendo-se deslocar o início da lista para posições anteriores do vetor ou o final da lista para posições posteriores, o que for menor.

A alocação dinâmica de memória para um vetor circular é um pouco mais trabalhosa, porque tem que deslocar as duas porções da lista para regiões diferentes do novo espaço.

Por exemplo, se se duplicasse a memória da última versão da lista acima, para que ela continuasse corretamente possuindo os mesmos elementos (`8 9 0`), a lista poderia ficar assim: (`x` é usado para indicar posições alocadas e não inicializadas)
```
   cap 10
   pri 9
   num 3
   dad 9 0 2 3 8 x x x x 8
```
o dado `8` teve que ser copiado para o final da nova área para manter a integridade da lista. Outra possibilidade, desta vez copiando `9 0` para o início da nova área:
```
   cap 10
   pri 4
   num 3
   dad 9 0 2 3 8 9 0 x x x
```

Em outro exemplo de realocação, considere a lista abaixo, que está cheia, com os valores `4 5 1 2 3`.
```
   cap 5
   pri 3
   num 5
   dad 1 2 3 4 5
       2 3 4 0 1 (índices na lista)
```
Após uma realocação que aumenta a capacidade para 10, ele pode ficar em uma das configurações abaixo, ainda com os mesmos valores `4 5 1 2 3`. A segunda configuração é melhor, porque envolve a cópia de 2 dados (iniciais, `4 5`) em vez de 3 (finais, `1 2 3`) da primeira.
```
   cap 10
   pri 3
   num 5
   dad 1 2 3 4 5 1 2 3 x x
             0 1 2 3 4     (índices na lista)
```
```
   cap 10
   pri 8
   num 5
   dad 1 2 3 4 5 x x x 4 5
       2 3 4           0 1 (índices na lista)
```

